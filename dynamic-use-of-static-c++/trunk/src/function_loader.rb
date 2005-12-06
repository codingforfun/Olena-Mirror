require 'pathname'

DYN_DATADIR = Pathname.new(__FILE__).dirname.parent + '_build/data'


class FunctionLoader
  attr_reader :identifier, :name, :include_dirs,
              :cflags, :ldflags
  @@default_cflags ||= []
  @@default_ldflags ||= []
  @@default_include_dirs ||= []
  def initialize ( cxx, identifier, aFunctionName )
    @identifier = identifier
    @include_dirs = @@default_include_dirs.dup
    @cflags = @@default_cflags.dup
    @ldflags = @@default_ldflags.dup
    @name = aFunctionName
    @cxx = cxx
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
        f.puts @cxx
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
      STDERR.puts 'JIT: Error when compiling this code'
      STDERR.puts @cxx
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
    def call ( cxx, identifier, aFunctionName )
      fun = new(cxx, identifier, aFunctionName)
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
