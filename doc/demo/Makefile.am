## Process this file with Automake to produce Makefile.in -*-Makefile-*-

AM_CPPFLAGS = $(OLN_CPPFLAGS) -DIMGDIR=\"$(OLN_IMG_DIR)\" -DIMGDIR2=\"$(OLN_IMG_AUX_DIR)\"
AM_CXXFLAGS = -Wall -W -ansi -pedantic $(OLN_CXXFLAGS)

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
morpholena_SOURCES = morpholena.cc
test_reconstruction_SOURCES = test-reconstruction.cc
