require 'pathname'
require 'dl'
require 'yaml'
require 'cxx_symbols'
load Pathname.new(__FILE__).dirname.parent + 'configure'

class Array
  alias_method :top, :last
  def stringify
    map { |x| x.to_s }
  end
end # class Array

class Cache
  def initialize ( pathname )
    @pathname = pathname
    @cache = (@pathname.exist?)? YAML.load(@pathname.read) : {}
    @local = {}
  end
  def []= ( key, value )
    sym, lib_path = value
    @cache[key] = lib_path
    @pathname.open('w') { |f| f.puts @cache.to_yaml }
    @local[key] = sym
  end
  def [] ( key )
    lib_path = @cache[key]
    sym = @local[key]
    return nil unless lib_path and lib_path.exist?
    return lib_path if sym.nil?
    sym
  end
end

class FunctionLoader
  attr_reader :identifier, :paths, :name, :args, :includes, :include_dirs,
              :lib_path, :sym, :options, :cflags, :ldflags
  @@default_includes ||= []
  @@default_post_includes ||= []
  @@default_cflags ||= []
  @@default_ldflags ||= []
  @@default_include_dirs ||= [Pathname.new(__FILE__).dirname.parent]
  def initialize ( identifier, arguments=[], options={} )
    kind, paths, name, args = identifier.split '__'
    @options = options
    @includes = @@default_includes.dup
    @post_includes = @@default_post_includes.dup
    @include_dirs = @@default_include_dirs.dup
    @cflags = @@default_cflags.dup
    @ldflags = @@default_ldflags.dup
    @identifier = identifier
    @kind = kind.gsub('_U_', '_').to_sym
    @@cache ||= Cache.new(repository + 'cache.yml')
    @args = arguments
    self.paths = paths
    self.name = name
  end
  def split ( aString )
    aString.split('_').map do |x|
      s = EncodedSymbol[x]
      (s.nil?)? SimpleSymbol.new(x) : s
    end
  end
  def paths= ( paths )
    @paths = paths.split('_P_').map { |path| Pathname.new(split(path).stringify.join) }
    @paths.delete_if { |x| x.to_s.empty? }
  end
  def name= ( name )
    @name = split(name).stringify.join
  end
  def kind
    @kind
  end
  def to_cxx
    call_args = []
    arguments = []
    vars      = []
    first_type_is_ptr = nil
    args.each_with_index do |a, i|
      arg = "arg#{i}"
      type = a.gsub(/&*$/, '') # remove references cause they are forbidden on lhs
      first_type_is_ptr ||= type =~ /\*\s*(const)?\s*>\s*$/
      arguments << "const data& #{arg}"
      call_args << "#{arg}_reinterpret_cast_ptr->obj()"
      vars << "#{type}* #{arg}_reinterpret_cast_ptr = " +
              "reinterpret_cast<#{type}* >(#{arg}.proxy());"
      vars << "assert(#{arg}_reinterpret_cast_ptr);"
    end
    if options[:method]
      indirection = (first_type_is_ptr)? '->' : '.'
      call = "(#{call_args.shift})#{indirection}#@name(#{call_args.join(', ')})"
    else
      call = "#@name(#{call_args.join(', ')})"
    end
    case kind
      when :fun, :op
        if kind == :op
          @name.gsub!('operator', '')
          call =
            case call_args.size
            when 1 then "#{@name}(#{call_args.first})"
            when 2 then "(#{call_args.shift}) #{@name} (#{call_args.shift})"
            else raise
            end
        end
        vars << "policy::receiver<select_dyn_policy((#{call}))> receiver;"
        vars << "(receiver(), #{call});"
        vars << "data ret(receiver.proxy(), (proxy_tag*)0);"
        vars << 'return ret;'
      when :ctor
        vars << "typedef #@name T;"
        vars << "T* ptr = new T(#{call_args.join(', ')});"
        vars << "abstract_data* proxy = new data_proxy_by_ptr<T>(ptr);"
        vars << "data ret(proxy, (proxy_tag*)0);"
        vars << 'return ret;'
      else raise "Unknown kind: #{kind}"
    end
    str = ''
    (@includes | [Pathname.new('policy.hh')] | paths | @post_includes).each do |path|
      next if path.to_s.empty?
      inc = path.to_s
      if inc !~ /["<]/
        inc = (path.to_s =~ /\.hh$/)? %Q{"#{path}"} : %Q{<#{path}>}
      end
      str << "#include #{inc}\n"
    end
    str << "
     |extern \"C\" {
     |  namespace dyn {
     |    namespace generated {
     |      data
     |      #@identifier(#{arguments.join(', ')})
     |      {
     |        #{vars.join("\n" + ' ' * 8)}
     |      }
     |    }
     |  }
     |};".gsub(/^\s*\|/, '')
  end
  def compile
    @basename = @identifier.gsub(/_[SN]?_/, '/').gsub('_D_', '.').
                  gsub('_U_', '_').gsub(/\/$/, '__').gsub(/^\//, '__')
    (repository + @basename).dirname.mkpath
    file = repository + "#@basename.cc"
    file.open('w') do |f|
      f.puts to_cxx
    end
    lib_ext = 'so'
    @lib_path = repository + "#@basename.#{lib_ext}"
    includes_opts = include_dirs.map { |x| "-I#{x}" }.join ' '
    cflags, ldflags = @cflags.join(' '), @ldflags.join(' ')
    out = repository + 'g++.out'
    object_file = file.to_s.gsub('.cc', '.o')
    cmd = "(#{COMPILE} #{cflags} -c #{includes_opts} #{file} -o #{object_file} && " +
          "#{LINK} #{SHARED} #{object_file} #{LIBDYNARG} #{ldflags} -o #{lib_path}) 2> #{out}"
    if system cmd
      out.unlink if out.exist?
    else
      out_s = out.read
      short_name = 'dynamic_function_wrapper'
      STDERR.puts cmd
      STDERR.puts out_s.
        sub(/^#{Regexp.quote(file)}:/, "[[#@name as #{short_name}]]:").
        gsub(/^#{Regexp.quote(file)}:/, "[[#{short_name}]]:").
        gsub(/dyn::generated::#{Regexp.quote(@identifier)}/, short_name)
      exit! 1
    end
  end
  def load_lib
    lib = DL.dlopen(lib_path.to_s)
    @sym = lib[identifier, '0' + 'P'*arity]
    @@cache[identifier] = [sym, lib_path]
    sym
  end
  def get_function
    case cached = @@cache[identifier]
    when nil
      STDERR.puts "\e[36mJIT: \e[31mMISS: compile:\e[0m #{self}"
      compile
      load_lib
    when Pathname
      STDERR.puts "\e[36mJIT: \e[33mHIT: dlopen:\e[0m #{self}"
      @lib_path = cached
      load_lib
    else
      STDERR.puts "\e[36mJIT: \e[32mHIT: dlsym:\e[0m #{self}"
      cached
    end
  end
  def arity
    args.size
  end
  def to_s
    "#@name(#{@args.join(', ')})" + ((paths.empty?)? '' : " in #{paths.join(', ')}")
  end
  def repository
    repository = Pathname.new('repository')
    repository.mkpath unless repository.exist?
    repository
  end
  class << self
    def call ( kind, aFunctionName, arguments=[], somePaths='', options='' )
      identifier = mangle(kind.to_s) + '__' + mangle(somePaths) + '__' + mangle(aFunctionName) + '__'
      identifier << arguments.map { |arg| mangle(arg) }.join('_C_')
      opts = {}
      options.split(/\s*,\s*/).each do |x|
        next if x.empty?
        opts[x.to_sym] = true
      end
      arguments.delete_if { |x| x.empty? }
      new(identifier, arguments, opts).get_function
    end
    def include_dir ( path )
      register(path, @@default_include_dirs, true)
    end
    def cflags ( elt )
      @@default_cflags << elt unless @@default_cflags.include? elt
    end
    def ldflags ( elt )
      @@default_ldflags << elt unless @@default_ldflags.include? elt
    end
    def include ( path )
      register(path, @@default_includes)
    end
    def post_include ( path )
      register(path, @@default_post_includes)
    end
    def register ( paths, set, expand=false )
      paths.split(':').each do |path|
        x = Pathname.new(path)
        x = x.expand_path if expand
        set << x unless set.include? x
      end
    end
    private :register
  end

end # class FunctionLoader
