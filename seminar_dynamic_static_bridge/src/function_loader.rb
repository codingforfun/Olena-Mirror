require 'pathname'
require 'dl'
require 'yaml'
require 'set'
require 'cxx_symbols'

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
  attr_reader :identifier, :path, :name, :args, :includes, :include_dirs, :lib_path, :sym
  @@default_includes ||= SortedSet.new
  @@default_post_includes ||= SortedSet.new
  @@default_include_dirs ||= SortedSet[Pathname.new(__FILE__).dirname.parent]
  def initialize ( identifier, kind=:fun )
    path, name, args = identifier.split '__'
    @includes = @@default_includes.dup
    @post_includes = @@default_post_includes.dup
    @include_dirs = @@default_include_dirs.dup
    @identifier = identifier
    @kind = kind
    @@cache ||= Cache.new(repository + 'cache.yml')
    self.path = path
    self.name = name
    self.args = args || ''
  end
  def split ( aString )
    aString.split('_').map do |x|
      s = EncodedSymbol[x]
      (s.nil?)? SimpleSymbol.new(x) : s
    end
  end
  def path= ( path )
    @path = Pathname.new(split(path).stringify.join)
  end
  def name= ( name )
    @name = split(name).stringify.join.to_sym
  end
  def args= ( args )
    @args = ArgParser.new(split(args).reverse).parse
  end
  def kind
    @kind
  end
  def to_cxx
    call_args = []
    arguments = []
    vars      = []
    args.each_with_index do |a, i|
      arg = "arg#{i}"
      type = a.gsub(/&*$/, '') # remove references (XXX)
      arguments << "const data& #{arg}"
      call_args << "*(#{arg}_value->obj())"
      vars << "data_proxy< #{type} >* #{arg}_value = " +
              "reinterpret_cast<data_proxy< #{type} >* >(#{arg}.proxy());"
      vars << "assert(#{arg}_value != !\"reinterpret_cast failed\");"
    end
    call = "#@name(#{call_args.join(', ')})"
    case kind
      when :fun
        vars << "data ret(#{call}, (dyn::by_cpy*)0);"
        vars << 'return ret;'
      when :proc
        vars << call + ';' << 'return nil;'
      when :method_proc, :method_proc2
        obj = "(#{call_args.shift})"
        call = "#{obj}.#@name(#{call_args.join(', ')})"
        vars << call + ';' << 'return nil;'
      when :method_fun, :method_fun2
        obj = "(#{call_args.shift})"
        call = "#{obj}.#@name(#{call_args.join(', ')})"
        vars << "data ret(#{call}, (dyn::by_cpy*)0);"
        vars << 'return ret;'
#       when :method_proc2
#         obj = "(#{call_args.shift})"
#         call = "#{obj}.#@name(#{call_args.join(', ')})"
#         vars << call + ';' << 'return nil;'
#       when :method_fun2
#         obj = "(#{call_args.shift})"
#         call = "#{obj}.#@name(#{call_args.join(', ')})"
#         vars << "return data(#{call});"
      when :ctor
#        long_name = "::dyn::#@name::#{@name}__class".gsub('::::', '::')
        # ret_type = "::dyn::Object"
        vars << "return *(new #@name(#{call_args.join(', ')}));"
      when :ctor2
        vars << "#@name ret(#{call_args.join(', ')});"
        vars << 'return ret;'
      else raise
    end
    str = ''
    [@includes.to_a, Pathname.new('data.hh'), path, @post_includes.to_a].flatten.each do |path|
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
    opts =
      case RUBY_PLATFORM
      when /darwin/ then '-bundle'
      when /linux/  then '-shared'
      end
    includes_opts = include_dirs.map { |x| "-I#{x}" }.join ' '
    out = repository + 'g++.out'
    cmd = "g++ -ggdb -W -Wall #{opts} #{includes_opts} -o #{lib_path} #{file} 2> #{out}"
    if system cmd
      out.unlink if out.exist?
    else
      out_s = out.read
      if kind.to_s =~ /fun/ and out_s =~ gcc_void_error
        STDERR.puts 'Not a function, try to compile a procedure'
        @kind = kind.to_s.gsub('fun', 'proc').to_sym
        return compile
      end
      STDERR.puts cmd
      STDERR.puts out_s.
        gsub(/^#{file}:/, "[[#@name]]:").
        gsub(/dyn::generated::#@identifier/, @name.to_s)
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
      puts "\e[31mMISS: Just In Time: #{self}\e[0m"
      compile
      load_lib
    when Pathname
      puts "\e[33mHIT: Load the library for #{self}\e[0m"
      @lib_path = cached
      load_lib
    else
      puts "\e[32mHIT #{self}\e[0m"
      cached
    end
  end
  def arity
    args.size
  end
  def to_s
    "#@path #@name(#{@args.join(', ')})"
  end
  def gcc_void_error
    /( conversion\sfrom\s[`']void'\sto\snon-scalar\stype\s[`']dyn::data'\s
     | invalid\suse\sof\svoid\sexpression
     )/x
  end
  def repository
    repository = Pathname.new('repository')
    repository.mkpath unless repository.exist?
    repository
  end
  class << self
    def call ( kind, aFunctionName, arguments=[], aPath='' )
      identifier = mangle(aPath) + '__' + mangle(aFunctionName) + '__'
      identifier << arguments.map { |arg| mangle(arg) }.join('_C_')
      new(identifier, kind).get_function
    end
    def include_dir ( path )
      @@default_include_dirs << Pathname.new(path).expand_path
    end
    def include ( path )
      @@default_includes << Pathname.new(path)
    end
    def post_include ( path )
      @@default_post_includes << Pathname.new(path)
    end
  end

  class ArgParser
    attr_reader :stack, :result, :current, :top
    def initialize ( stack )
      @stack = stack
      @current = []
      @result = []
      @top = nil
    end
    def template_args
      current << top
      until stack.empty?
        case (top = stack.pop).code
        when :R
          current << top
          return
        when :L
          template_args
        else
          current << top
        end
      end
      raise
    end
    def args
      until stack.empty?
        case (@top = stack.pop).code
        when :C
          result << current.stringify.join
          current.clear
        when :L
          template_args
        else
          current << top
        end
      end
      result << current.stringify.join unless current.empty?
    end
    def parse
      args
      result
    end
  end

end # class FunctionLoader
