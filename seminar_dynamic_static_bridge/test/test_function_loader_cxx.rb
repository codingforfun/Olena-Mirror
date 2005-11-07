require 'pathname'
require 'yaml'
test = Pathname.new(__FILE__).dirname
root = test.parent
fixtures = test + 'fixtures'
src = root + 'src'
require src + 'function_loader'

FunctionLoader.include_dir fixtures
FunctionLoader.include_dir src

funs = []

funs << FunctionLoader.from_cxx_call('my_lib/lib.hh', 'foo1')
funs << FunctionLoader.from_cxx_call('my_lib/lib.hh', 'foo2', [], 'int*')
funs << FunctionLoader.from_cxx_call('my_lib/lib.hh', 'foo3',
                                     ['double', 'const double', 'const double&'], 'double*')
funs << FunctionLoader.from_cxx_call('my_lib/lib.hh', 'my_lib::x::foo4',
                                     ['u<float>', 't<t<char, char>, u<const int> >'], 'u<char>***')
funs << FunctionLoader.from_cxx_call('my_lib/lib.hh', 'foo3',
                                     ['t<int, char>', 'const t<int, char>*', 'const t<int, char>&'],
                                     't<int, char>*')

# FIXME funs

