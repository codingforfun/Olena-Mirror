require 'pathname'
require 'dl'
require 'pstore'

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
  attr_reader :name, :code, :cxx_char
  @@codes     ||= {}
  def initialize ( name, code, cxx_char=nil )
    @name = name
    @code = (code.is_a? Integer)? code.chr.to_sym : code
    @cxx_char = (cxx_char.is_a? Integer)? cxx_char.chr.to_sym : cxx_char
    raise if @@codes[@code]
    @@codes[@code] = self
  end
  def self.[] ( arg )
    @@codes[arg.to_sym]
  end
  def to_s
    @cxx_char || "#@name "
  end
end # class EncodedSymbol

class Cache
  def initialize ( pathname )
    @pstore = PStore.new pathname.to_s
    @local = {}
  end
  def []= ( key, value )
    sym, lib_path = value
    @pstore.transaction { @pstore[key] = lib_path }
    @local[key] = sym
  end
  def [] ( key )
    lib_path = nil
    @pstore.transaction { lib_path = @pstore[key] }
    sym = @local[key]
    if sym.nil?
      raise NotImplementedError
    end
    [sym, lib_path]
  end
end

class FunctionLoader
  attr_reader :identifier, :path, :name, :args, :ret, :cache, :includes, :include_dirs
  @@default_includes ||= []
  @@default_include_dirs ||= []
  def initialize ( identifier )
    path, name, args, ret = identifier.split '__'
    @includes = @@default_includes.dup
    @include_dirs = @@default_include_dirs.dup
    @identifier = identifier
    self.path = path
    self.name = name
    self.args = args
    self.ret  = ret
    @cache = Cache.new(repository + 'cache.db')
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
#     @args = []
#     str = split(args).stringify.join
#     str.scan(/[^,<]+(?:<[^>]*>)?/) { |x| @args << x }
#     p @args
    @args = []
    stack = split(args).reverse
    arg = []
    until stack.empty?
      case (top = stack.pop).code
      when :C
        @args << arg.stringify.join
        arg = []
      when :L
        arg << top
        until stack.top.code == :R
          arg << stack.pop
        end
      else
        arg << top
      end
    end
    @args << arg.stringify.join unless arg.empty?
  end
  def ret= ( ret )
    @ret = split(ret).stringify.join
  end
  def ret?
    @ret != 'void'
  end
  def to_cxx
    call_args = []
    arguments = []
    vars      = []
    args.each_with_index do |a, i|
      arg = "arg#{i}"
      type = a.gsub(/&*$/, '') # remove references (XXX)
      arguments << "const dyn::data& #{arg}"
      call_args << arg
      vars << "dyn::data_proxy< #{type} >* #{arg}_value = " +
              "dynamic_cast< dyn::data_proxy< #{type} >* >(#{arg}.proxy_);"
    end
    call = "#@name(#{call_args.join(', ')})"
    if ret?
      type = @ret.gsub(/&*$/, '') # remove references (XXX)
      arguments << "dyn::data& ret"
      vars << "dyn::data_proxy< #{type} >* ret_value = " +
              "dynamic_cast< dyn::data_proxy< #{type} >* >(ret.proxy_);"
      vars << "ret_value->assign(#{call});"
    else
      vars << call + ';'
    end
    inc = (path.to_s =~ /\.hh$/)? %Q{"#{path}"} : %Q{<#{path}>}
    "|#include #{inc}
     |#include \"dyn.hh\"
     |extern \"C\" {
     |  namespace dyn {
     |    namespace generated {
     |      void
     |      #@identifier(#{arguments.join(', ')})
     |      {
     |        #{vars.join("\n" + ' ' * 8)}
     |      }
     |    }
     |  }
     |};".gsub(/^\s*\|/, '')
  end
  def get_function
    file = repository + "#@identifier.cc"
    file.open('w') do |f|
      f.puts to_cxx
    end
    lib_ext = 'so'
    lib_path = repository + "#@identifier.#{lib_ext}"
    opts =
      case RUBY_PLATFORM
      when /darwin/ then '-bundle'
      when /linux/  then '-shared'
      end
    includes_opts = include_dirs.map { |x| "-I#{x}" }.join ' '
    out = Pathname.new 'g++.out'
    cmd = "g++ #{opts} #{includes_opts} -o #{lib_path} #{file} 2> #{out}"
    puts cmd
    if system cmd
      out.unlink if out.exist?
      lib = DL.dlopen(lib_path.to_s)
      sym = lib[@identifier, '0' + 'P'*arity]
      cache[@identifier] = [sym, lib_path]
      sym
    else
      STDERR.puts File.read(out).
        gsub(/^#{file}:/, "[[#@name]]:").
        gsub(/dyn::generated::#@identifier/, @name.to_s)
      exit 1
    end
  end
  def arity
    args.size + ((ret?)? 1 : 0)
  end
  def to_s
    "#@path: #@ret #@name(#{@args.join(', ')})"
  end
  def repository
    repository = Pathname.new('repository')
    unless repository.exist?
      repository.mkpath
      dyn_hh = 'dyn.hh'
      (repository + dyn_hh).make_symlink(dyn_dir + dyn_hh)
    end
    repository
  end
  def dyn_dir
    Pathname.new(__FILE__).dirname.parent
  end
  class << self
    def from_mlc_name_of ( aPath, aString )
      puts "from_mlc_name_of('#{aPath}', '#{aString}')"
      identifier = 'my_U_lib_S_lib_D_hh__foo1____void'
      fun = new identifier
#       puts fun
#       puts fun.to_cxx
      fun_ptr = fun.get_function
#       puts fun_ptr
#      fun_ptr.call
      fun_ptr
    end
    def include_dir ( path )
      @@default_include_dirs << Pathname.new(path).expand_path
    end
    def include ( path )
      @@default_includes << Pathname.new(path).expand_path
    end
  end
end # class FunctionLoader
