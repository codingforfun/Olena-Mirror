# Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

# Find Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_builddir)/milena
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)

EXTRA_DIST = lena1024.pgm

noinst_PROGRAMS =				\
  dilation-lena					\
  gradient-lena					\
  gradient-spe-lena

EXTRA_DIST +=					\
  and_not.hh					\
  minus.hh

dilation_lena_SOURCES = dilation-lena.cc
gradient_lena_SOURCES = gradient-lena.cc
gradient_spe_lena_SOURCES = gradient-spe-lena.cc

MOSTLYCLEANFILES =				\
  dilation-lena-out-spe.pgm			\
  dilation-lena-out-gen.pgm			\
  dilation-lena-out-fast.pgm			\
  dilation-lena-out-faster.pgm			\
  dilation-lena-out-fast_static.pgm		\
  gradient-lena-out.pgm				\
  gradient-spe-lena-out-0.pgm			\
  gradient-spe-lena-out-1.pgm			\
  gradient-spe-lena-out-2.pgm			\
  gradient-spe-lena-out-3.pgm			\
  gradient-spe-lena-out-4.pgm
