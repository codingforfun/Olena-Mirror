pyexec_PROGRAMS = _swilena.so _swilena2d.so _swilena1d.so _swilena3d.so \
	_swilena2d_convert.so _swilena1d_convert.so _swilena3d_convert.so \
	_swilena2d_morpho.so _swilena1d_morpho.so _swilena3d_morpho.so

python_PYTHON = swilena.py swilena2d.py swilena3d.py swilena1d.py \
	swilena2d_convert.py swilena1d_convert.py swilena3d_convert.py \
	swilena2d_morpho.py swilena1d_morpho.py swilena3d_morpho.py

INCLUDES = -I/usr/include/python2.2 -I$(srcdir)/../src -I$(OLN_LOCAL_SRC) -I$(OLN_LOCAL_BUILD)
AM_CPPFLAGS = -DOLN_EXCEPTIONS 
AM_CXXFLAGS = $(CXXFLAGS_OPTIMIZE)

_swilena_so_SOURCES = swilena_wrap.cxx
_swilena_so_LDFLAGS = -shared -lswigpy
_swilena2d_so_SOURCES = swilena2d_wrap.cxx
_swilena2d_so_LDFLAGS = -shared -lswigpy
_swilena1d_so_SOURCES = swilena1d_wrap.cxx
_swilena1d_so_LDFLAGS = -shared -lswigpy
_swilena3d_so_SOURCES = swilena3d_wrap.cxx
_swilena3d_so_LDFLAGS = -shared -lswigpy
_swilena2d_convert_so_SOURCES = swilena2d_convert_wrap.cxx
_swilena2d_convert_so_LDFLAGS = -shared -lswigpy
_swilena1d_convert_so_SOURCES = swilena1d_convert_wrap.cxx
_swilena1d_convert_so_LDFLAGS = -shared -lswigpy
_swilena3d_convert_so_SOURCES = swilena3d_convert_wrap.cxx
_swilena3d_convert_so_LDFLAGS = -shared -lswigpy
_swilena2d_morpho_so_SOURCES = swilena2d_morpho_wrap.cxx
_swilena2d_morpho_so_LDFLAGS = -shared -lswigpy
_swilena1d_morpho_so_SOURCES = swilena1d_morpho_wrap.cxx
_swilena1d_morpho_so_LDFLAGS = -shared -lswigpy
_swilena3d_morpho_so_SOURCES = swilena3d_morpho_wrap.cxx
_swilena3d_morpho_so_LDFLAGS = -shared -lswigpy

BUILT_SOURCES = \
	swilena_wrap.cxx \
	swilena1d_wrap.cxx \
	swilena2d_wrap.cxx \
	swilena3d_wrap.cxx \
	swilena1d_convert_wrap.cxx \
	swilena2d_convert_wrap.cxx \
	swilena3d_convert_wrap.cxx \
	swilena2d_morpho_wrap.cxx \
	swilena1d_morpho_wrap.cxx \
	swilena3d_morpho_wrap.cxx \
	swilena.py \
	swilena1d.py \
	swilena2d.py \
	swilena3d.py \
	swilena1d_convert.py \
	swilena2d_convert.py \
	swilena3d_convert.py \
	swilena1d_morpho.py \
	swilena2d_morpho.py \
	swilena3d_morpho.py

%_wrap.cxx %.py: $(srcdir)/../src/%.i
	$(SWIG) -c -c++ -python -I$(srcdir)/../src -o $*_wrap.cxx $(srcdir)/../src/$*.i


