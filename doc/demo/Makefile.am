## Process this file with Automake to produce Makefile.in -*-Makefile-*-

INCLUDES = -I$(OLN_LOCAL_SRC) -I$(OLN_LOCAL_BUILD)
AM_CPPFLAGS = $(OLN_CPPFLAGS) -DIMGDIR=\"$(OLN_LOCAL_IMGS)\" -DIMGDIR2=\"$(OLN_LOCAL_AUX_IMGS)\"
AM_CXXFLAGS = $(OLN_CXXFLAGS) $(CXXFLAGS_STRICT) 
CXXFLAGS += $(CXXFLAGS_OPTIMIZE)

# We used to declare these programs as `noinst_PROGRAMS', however
# building them on each run of `make' turned to
#   1. slowdown development;
#   2. annoy Geoffroy when building the .deb package (he wants to
#      ship the directory unbuilt, but still has to run make for
#      the other directories).
# So for now we'll just build these programs on `make check' and `make demo'.
check_PROGRAMS = tour morpholena test-reconstruction

.PHONY: demo
demo: $(check_PROGRAMS)

tour_SOURCES = tour.cc
morpholena_SOURCES = morpholena.cc \
  morpholena-base.cc \
  morpholena-opening-closing.cc \
  morpholena-fast-hats.cc \
  morpholena-hats.cc \
  morpholena-grad.cc \
  morpholena-gaussian.cc \
  morpholena-minima-imp.cc \
  morpholena-minima.cc \
  morpholena-hit-miss.cc \
  morpholena-ops.cc \
  morpholena-wshed.cc \
  morpholena-wshed-beucher.cc

test_reconstruction_SOURCES = test-reconstruction.cc
