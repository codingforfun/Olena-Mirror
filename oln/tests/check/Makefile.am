## Process this file through Automake to create Makefile.in -*- Makefile -*-

include Makefile.flags

AM_CPPFLAGS += -DOLN_IMG_DIR=\"$(top_srcdir)/oln/img\" \
   -DCXX=\""$(CXX)"\" \
   -DCPPFLAGS=\""$(CPPFLAGS) $(OLN_CPPFLAGS)"\" \
   -DCXXFLAGS=\""$(CXXFLAGS) $(OLN_CXXFLAGS)"\" \
   -DOLN_EXTRA_CPPFLAGS=\""$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) -I$(OLN_LOCAL_SRC) -I$(OLN_LOCAL_BUILD) $(CPPFLAGS)"\" \
   -DCXXFLAGS_OPTIMIZE=\""$(CXXFLAGS_OPTIMIZE)"\" \
   -DCXXFLAGS_STRICT=\""$(CXXFLAGS_STRICT)"\" \
   -DCXXFLAGS_STRICT_ERRORS=\""$(CXXFLAGS_STRICT_ERRORS)"\"

check_LIBRARIES = libcheck.a

libcheck_a_SOURCES =				\
  check.hh					\
  data.cc					\
  data.hh					\
  main.cc					\
  srcdir.cc					\
  srcdir.hh					\
  verbose.cc					\
  verbose.hh					\
  compile.cc					\
  compile.hh

EXTRA_DIST = Makefile.flags Makefile.check
