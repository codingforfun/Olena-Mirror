## Process this file with Automake to produce Makefile.in -*-Makefile-*-

## Copyright (C) 2002, 2003, 2004  EPITA Research and Development Laboratory
## 
## This file is part of the Olena Library.  This library is free
## software; you can redistribute it and/or modify it under the terms
## of the GNU General Public License version 2 as published by the
## Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this library; see the file COPYING.  If not, write to
## the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
## Boston, MA 02110-1301, USA.
## 
## As a special exception, you may use this file as part of a free
## software library without restriction.  Specifically, if other files
## instantiate templates or use macros or inline functions from this
## file, or you compile this file and link it with other files to
## produce an executable, this file does not by itself cause the
## resulting executable to be covered by the GNU General Public
## License.  This exception does not however invalidate any other
## reasons why the executable file might be covered by the GNU General
## Public License.


AM_CPPFLAGS = -DIMGDIR=\"$(top_srcdir)/olena/img\"
AM_CXXFLAGS = $(CXXFLAGS_STRICT_ERRORS) $(CXXFLAGS_OPTIMIZE) $(ZLIB_CXXFLAGS)
AM_LDFLAGS = $(ZLIB_LDFLAGS)

# We used to declare these programs as `noinst_PROGRAMS', however
# building them on each run of `make' turned to
#   1. slowdown development;
#   2. annoy Geoffroy when building the .deb package (he wants to
#      ship the directory unbuilt, but still has to run make for
#      the other directories).
# So for now we'll just build these programs on `make check' and `make demo'.

check_PROGRAMS = image tour morpholena convol-gaussian

.PHONY: demo
demo: $(check_PROGRAMS)

image_SOURCES = image.cc
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
