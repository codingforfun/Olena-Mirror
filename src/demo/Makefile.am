## Process this file with Automake to produce Makefile.in -*-Makefile-*-

AM_CPPFLAGS = -I$(OLN_INCLUDEDIR)
AM_CXXFLAGS = -Wall -W -ansi -pedantic

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
