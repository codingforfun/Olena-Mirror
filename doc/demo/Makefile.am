## Process this file with Automake to produce Makefile.in -*-Makefile-*-

AM_CPPFLAGS = -DIMGDIR=\"$(top_srcdir)/olena/img\"
AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE)

# We used to declare these programs as `noinst_PROGRAMS', however
# building them on each run of `make' turned to
#   1. slowdown development;
#   2. annoy Geoffroy when building the .deb package (he wants to
#      ship the directory unbuilt, but still has to run make for
#      the other directories).
# So for now we'll just build these programs on `make check' and `make demo'.
check_PROGRAMS = tour morpholena test-reconstruction convol-gaussian

.PHONY: demo
demo: $(check_PROGRAMS)

tour_SOURCES = tour.cc
morpholena_SOURCES = morpholena.cc \
  morpholena-base.cc \
  morpholena-hats.cc \
  morpholena-grad.cc \
  morpholena-minima.cc \
  morpholena-hit-miss.cc \
  morpholena-ops.cc \
  morpholena-wshed.cc \
  morpholena-wshed-beucher.cc

test_reconstruction_SOURCES = test-reconstruction.cc
convol_gaussian_SOURCES = convol-gaussian.cc

CLEANFILES = \
	lena-beg-f.pgm				\
	lena-beg.pgm				\
	lena-bth-f.pgm				\
	lena-bth.pgm				\
	lena-clo-f.pgm				\
	lena-clo.pgm				\
	lena-dil-f.pgm				\
	lena-dil.pgm				\
	lena-ero-f.pgm				\
	lena-ero.pgm				\
	lena-exb-f.pgm				\
	lena-exb.pgm				\
	lena-gau.pgm				\
	lena-gd1.pgm				\
	lena-gd2.pgm				\
	lena-hom-f.pgm				\
	lena-hom.pgm				\
	lena-ing-f.pgm				\
	lena-ing.pgm				\
	lena-lap-f.pgm				\
	lena-lap.pgm				\
	lena-myw.pgm				\
	lena-ope-f-face.pgm			\
	lena-ope-f.pgm				\
	lena-ope.pgm				\
	lena-sth-f.pgm				\
	lena-sth.pgm				\
	lena-thc-f.pgm				\
	lena-thc.pgm				\
	lena-thick-f.pgm			\
	lena-thin.pgm				\
	lena-wor.pgm				\
	lena-wsc.pgm				\
	lena-wss.pgm				\
	lena-wth-f.pgm				\
	lena-wth.pgm				\
	minima_imposition_hybrid.pgm		\
	minima_imposition_sequential.pgm	\
	minima_imposition_sure.pgm		\
	regional_min_hybrid.pbm 		\
	regional_min_sequential.pbm		\
	regional_min_sure.pbm			\
	output.pgm
