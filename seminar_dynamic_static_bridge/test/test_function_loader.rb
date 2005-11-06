require 'pathname'
require 'yaml'
test = Pathname.new(__FILE__).dirname
root = test.parent
fixtures = test + 'fixtures'
src = root + 'src'
require src + 'function_loader'

Pathname.glob(fixtures + '*.yml.cc').each do |file|
  YAML.load(file.read).each do |identifier, ref|
    fun = FunctionLoader.new identifier
    fun.includes << fixtures << src
    puts fun
    puts ref
    puts fun.to_cxx
    fun_ptr = fun.get_function
    puts fun_ptr
    puts '-' * 79
  end
end
