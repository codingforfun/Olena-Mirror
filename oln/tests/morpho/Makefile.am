include $(top_srcdir)/check/Makefile.check

# FIXME: set this variable at configure time.
OLN_OPTIMS_CXXFLAGS = "-O3"
AM_CXXFLAGS += $(OLN_OPTIMS_CXXFLAGS)

check_PROGRAMS = \
  edoc \
  extrkiller \
  reconstruction

edoc_SOURCES = edoc.cc
extrkiller_SOURCES = extrkiller.cc
reconstruction_SOURCES = reconstruction.cc
