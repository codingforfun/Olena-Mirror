require 'pathname'
require 'dl'
require 'yaml'
require 'set'

class Array
  alias_method :top, :last
  def stringify
    map { |x| x.to_s }
  end
end # class Array

class SimpleSymbol
  attr_reader :code
  def initialize ( code )
    @code = code
  end
  def to_s
    @code.to_s
  end
end # class SimpleSymbol

class EncodedSymbol
  attr_reader :name, :code, :cxx_char, :patt
  @@codes     ||= {}
  def initialize ( name, code, cxx_char=nil )
    @name = name
    @code = (code.is_a? Integer)? code.chr.to_sym : code
    @cxx_char = (cxx_char.is_a? Integer)? cxx_char.chr.to_sym : cxx_char
    @patt = to_s.strip
    raise if @@codes[@code]
    @@codes[@code] = self
  end
  def self.[] ( arg )
    @@codes[arg.to_sym]
  end
  def to_s
    (@cxx_char || "#@name ").to_s
  end
  def self.encode ( str )
    result = str.gsub '_', '_U_'
    result.gsub!(/\s*/, '')
    @@codes.each_value do |v|
      next if v.code == :U
      result.gsub!(v.patt, "_#{v.code}_")
    end
    result.gsub!(/_+/, '_')
    result.gsub!(/^_/, '')
    result.gsub!(/_$/, '')
    result
  end
end # class EncodedSymbol

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
  @@default_include_dirs ||= SortedSet[Pathname.new(__FILE__).dirname.parent]
  def initialize ( identifier, kind=:fun )
    path, name, args = identifier.split '__'
    @includes = @@default_includes.dup
    @include_dirs = @@default_include_dirs.dup
    @identifier = identifier
    @kind = kind
    @@cache ||= Cache.new(repository + 'cache.yml')
    self.path = path
    self.name = name
    self.args = args || ''
  end
  def self.enc ( name, code, cxx_char=nil )
    EncodedSymbol.new(name, code, cxx_char)
  end
  enc :underscore, :U, ?_
  enc :slash,      :S, ?/
  enc :dot,        :D, ?.
  enc :function,   :F
  enc :left,       :L, '< '
  enc :right,      :R, ' >'
  enc :ref,        :REF, ?&
  enc :const,      :CONST
  enc :ptr,        :PTR, ?*
  enc :namespace,  :N, '::'
  enc :comma,      :C, ', '
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
      arguments << "const data* #{arg}"
      call_args << "*(#{arg}_value->p_obj_)"
      vars << "data_proxy< #{type} >* #{arg}_value = " +
              "reinterpret_cast<data_proxy< #{type} >* >(#{arg}->proxy());"
    end
    call = "#@name(#{call_args.join(', ')})"
    case kind
      when :fun
        vars << "data ret_value(#{call});"
        vars << 'return ret_value;'
      when :proc
        vars << call + ';' << 'return nil;'
      when :method_proc
        obj = "(#{call_args.shift})"
        call = "#{obj}.#@name(#{call_args.join(', ')})"
        vars << call + ';' << 'return nil;'
      when :method_fun
        obj = "(#{call_args.shift})"
        call = "#{obj}.#@name(#{call_args.join(', ')})"
        vars << "data ret_value(#{call});"
        vars << 'return ret_value;'
      else raise
    end
    str = ''
    includes = @includes.dup
    includes << path
    includes.each do |path|
      next if path.to_s.empty?
      inc = (path.to_s =~ /\.hh$/)? %Q{"#{path}"} : %Q{<#{path}>}
      str << "#include #{inc}\n"
    end
    str << "
     |#include \"dyn.hh\"
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
    file = repository + "#@identifier.cc"
    file.open('w') do |f|
      f.puts to_cxx
    end
    lib_ext = 'so'
    @lib_path = repository + "#@identifier.#{lib_ext}"
    opts =
      case RUBY_PLATFORM
      when /darwin/ then '-bundle'
      when /linux/  then '-shared'
      end
    includes_opts = include_dirs.map { |x| "-I#{x}" }.join ' '
    out = repository + 'g++.out'
    cmd = "g++ #{opts} #{includes_opts} -o #{lib_path} #{file} 2> #{out}"
    if system cmd
      out.unlink if out.exist?
    else
      STDERR.puts cmd
      STDERR.puts out.read.
        gsub(/^#{file}:/, "[[#@name]]:").
        gsub(/dyn::generated::#@identifier/, @name.to_s)
      exit 1
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
      puts "MISS: Just In Time: #{self}"
      compile
      load_lib
    when Pathname
      puts "HIT: Load the library for #{self}"
      @lib_path = cached
      load_lib
    else
      puts "HIT"
      cached
    end
  end
  def arity
    args.size
  end
  def to_s
    "#@path: #@name(#{@args.join(', ')})"
  end
  def repository
    repository = Pathname.new('repository')
    repository.mkpath unless repository.exist?
    repository
  end
  class << self
    def mangle ( aString )
      EncodedSymbol.encode(aString)
    end
    private :mangle
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
