AM_CPPFLAGS = -I$(top_builddir) -I$(srcdir) -I. -I@RUBY_topdir@

ERB = ruby $(top_srcdir)/config/erbx

%.hh: %.erb.hh
	$(ERB) -i $< -o $@

%.cc: %.erb.cc
	$(ERB) -i $< -o $@

lib_LTLIBRARIES = libdyn.la

BUILT_libdyn_la_HEADERS_ = \
	all_methods.hh \
	function.hh \
	data.hh

BUILT_libdyn_la_SOURCES = \
	all_methods.cc \
	function.cc

BUILT_SOURCES = $(BUILT_libdyn_la_SOURCES) $(BUILT_libdyn_la_HEADERS_)

include_HEADERS = \
	dyn.hh \
	data.hxx \
	$(BUILT_libdyn_la_HEADERS_) \
	function_loader.hh \
	name_of.hh \
	policy.hh

libdyn_la_SOURCES = \
	ansidecl.h demangle.h config.hh \
	$(include_HEADERS) \
	data.cc \
	$(BUILT_libdyn_la_SOURCES) \
	function_loader.cc \
	name_of.cc \
	policy.cc \
	ruby_stream.hh \
	ruby_stream.cc

libdyn_la_LIBADD = @LIBERTYARG@ @RUBY_LIBRUBYARG_SHARED@

libdyn_la_LDFLAGS = -version-info 0:1:0

CLEANFILES = $(BUILT_libdyn_la_SOURCES)
