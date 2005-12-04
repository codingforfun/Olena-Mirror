require 'pathname'

DYN_DATADIR = Pathname.new(__FILE__).dirname.parent + '_build/data'


class FunctionLoader
  attr_reader :identifier, :paths, :name, :args, :includes, :include_dirs,
              :lib_path, :sym, :options, :cflags, :ldflags
  @@default_includes ||= []
  @@default_post_includes ||= []
  @@default_cflags ||= []
  @@default_ldflags ||= []
  @@default_include_dirs ||= []
  def initialize ( identifier, kind, aFunctionName, arguments=[], somePaths='', options='' )
    @identifier = identifier
    @options = {}
    options.split(/\s*,\s*/).each do |x|
      next if x.empty?
      @options[x.to_sym] = true
    end
    @includes = @@default_includes.dup
    @post_includes = @@default_post_includes.dup
    @include_dirs = @@default_include_dirs.dup
    @cflags = @@default_cflags.dup
    @ldflags = @@default_ldflags.dup
    @kind = kind.to_sym
    @args = arguments
    arguments.delete_if { |x| x.empty? }
    self.paths = somePaths
    @name = aFunctionName
  end
  def paths= ( paths )
    @paths = paths.split(':').map { |path| Pathname.new(path) }
    @paths.delete_if { |x| x.to_s.empty? }
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
    (@includes | [Pathname.new('dyn-light.hh')] | paths | @post_includes).each do |path|
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
     |      dyn_#{identifier}(#{arguments.join(', ')})
     |      {
     |        #{vars.join("\n" + ' ' * 8)}
     |      }
     |    }
     |  }
     |};".gsub(/^\s*\|/, '')
  end
  def compile
    dir = repository + @identifier
    unless dir.exist?
      dir.mkpath
      makefile = (DYN_DATADIR + 'Makefile.template').read
      makefile.gsub!(/libdyn_function\.la/, "libdyn_#{identifier}.la")
      (dir + 'Makefile').open('w') { |f| f.puts makefile }
      file = dir + "function.cc"
      (dir + '.deps').mkpath
      (dir + '.deps' + 'libdyn_function_la-function.Plo').open('w')
      file.open('w') do |f|
        f.puts to_cxx
      end
      includes_opts = include_dirs.map { |x| "-I#{x}" }.join ' '
      cflags, ldflags = @cflags.join(' '), @ldflags.join(' ')
      (dir + 'Makefile').open('a') do |f|
        f.puts "CXXFLAGS += #{includes_opts} #{cflags}"
        f.puts "LDFLAGS += #{ldflags}"
      end
    end
    out = dir + 'make.out'
    cmd = "cd #{dir} && make > make.out 2>&1"
    if system cmd
      out.unlink if out.exist?
    else
      STDERR.puts cmd
      STDERR.puts out.read
      exit! 1
    end
  end
  def repository
    repository = Pathname.new('repository')
    unless repository.exist?
      repository.mkpath
      (repository + 'Makefile').make_symlink(DYN_DATADIR + 'Makefile.repository')
    end
    repository
  end
  class << self
    def call ( identifier, kind, aFunctionName, arguments=[], somePaths='', options='' )
      fun = new(identifier, kind, aFunctionName, arguments, somePaths, options)
      fun.compile
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
