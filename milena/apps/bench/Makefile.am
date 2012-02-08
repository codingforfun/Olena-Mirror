# Copyright (C) 2010, 2011, 2012 EPITA Research and Development
# Laboratory (LRDE).
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

# Benchmarks for the ICIP 2011 submission and the GRETSI 2011 paper.

# Find Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_builddir)/milena
# Produce fast code.
AM_CXXFLAGS = $(APPS_CXXFLAGS_NODEBUG)

EXTRA_DIST = lena1024.pgm lena2048.pgm

## ---------- ##
## Dilation.  ##
## ---------- ##

noinst_PROGRAMS =				\
  dilation-lena					\
  dilation-lena-bench-nongen			\
  dilation-lena-bench-nongen_2ptr		\
  dilation-lena-bench-gen			\
  dilation-lena-bench-fast			\
  dilation-lena-bench-fast_static		\
  dilation-lena-table				\
  gradient-lena					\
  gradient-spe-lena

EXTRA_DIST +=					\
  and_not.hh					\
  minus.hh					\
						\
  static_array.hh				\
  static_dpoints_pixter.hh			\
  static_window.hh				\
  trait.hh

noinst_HEADERS = dilation-lena.hh dilation-lena-bench-macros.hh

dilation_lena_SOURCES = dilation-lena.cc
dilation_lena_bench_nongen_SOURCES = dilation-lena-bench-nongen.cc
dilation_lena_bench_nongen_2ptr_SOURCES = dilation-lena-bench-nongen_2ptr.cc
dilation_lena_bench_gen_SOURCES = dilation-lena-bench-gen.cc
dilation_lena_bench_fast_SOURCES = dilation-lena-bench-fast.cc
dilation_lena_bench_fast_static_SOURCES = dilation-lena-bench-fast_static.cc
dilation_lena_table_SOURCES = dilation-lena-table.cc
gradient_lena_SOURCES = gradient-lena.cc
gradient_spe_lena_SOURCES = gradient-spe-lena.cc

MOSTLYCLEANFILES =				\
  dilation-lena-out-512-nongen.pgm		\
  dilation-lena-out-512-nongen_2ptr.pgm		\
  dilation-lena-out-512-nongen_1ptr.pgm		\
  dilation-lena-out-512-gen.pgm			\
  dilation-lena-out-512-fast.pgm		\
  dilation-lena-out-512-fast_noaccu.pgm		\
  dilation-lena-out-512-faster.pgm		\
  dilation-lena-out-512-faster_noaccu.pgm	\
  dilation-lena-out-512-fast_static.pgm		\
  dilation-lena-out-512-faster_static.pgm	\
  dilation-lena-out-1024-nongen.pgm		\
  dilation-lena-out-1024-nongen_2ptr.pgm	\
  dilation-lena-out-1024-nongen_1ptr.pgm	\
  dilation-lena-out-1024-gen.pgm		\
  dilation-lena-out-1024-fast.pgm		\
  dilation-lena-out-1024-fast_noaccu.pgm	\
  dilation-lena-out-1024-faster.pgm		\
  dilation-lena-out-1024-faster_noaccu.pgm	\
  dilation-lena-out-1024-fast_static.pgm	\
  dilation-lena-out-1024-faster_static.pgm	\
  dilation-lena-out-2048-nongen.pgm		\
  dilation-lena-out-2048-nongen_2ptr.pgm	\
  dilation-lena-out-2048-nongen_1ptr.pgm	\
  dilation-lena-out-2048-gen.pgm		\
  dilation-lena-out-2048-fast.pgm		\
  dilation-lena-out-2048-fast_noaccu.pgm	\
  dilation-lena-out-2048-faster.pgm		\
  dilation-lena-out-2048-faster_noaccu.pgm	\
  dilation-lena-out-2048-fast_static.pgm	\
  dilation-lena-out-2048-faster_static.pgm	\
						\
  dilation-lena-table-out-512-nongen.pgm	\
  dilation-lena-table-out-512-nongen_2ptr.pgm	\
  dilation-lena-table-out-512-gen.pgm		\
  dilation-lena-table-out-512-fast.pgm		\
  dilation-lena-table-out-512-fast_static.pgm	\
  dilation-lena-table-out-1024-nongen.pgm	\
  dilation-lena-table-out-1024-nongen_2ptr.pgm	\
  dilation-lena-table-out-1024-gen.pgm		\
  dilation-lena-table-out-1024-fast.pgm		\
  dilation-lena-table-out-1024-fast_static.pgm	\
  dilation-lena-table-out-2048-nongen.pgm	\
  dilation-lena-table-out-2048-nongen_2ptr.pgm	\
  dilation-lena-table-out-2048-gen.pgm		\
  dilation-lena-table-out-2048-fast.pgm		\
  dilation-lena-table-out-2048-fast_static.pgm	\
						\
  gradient-lena-out.pgm				\
  gradient-spe-lena-out-0.pgm			\
  gradient-spe-lena-out-1.pgm			\
  gradient-spe-lena-out-2.pgm			\
  gradient-spe-lena-out-3.pgm			\
  gradient-spe-lena-out-4.pgm

# Use Make to generate tests instead of `configure', as advised by
# Autoconf's manual (see section ``Installation Directory
# Variables'').
edit = sed -e 's|@top_srcdir[@]|$(top_srcdir)|g'	\
           -e 's|@srcdir[@]|$(srcdir)|g'
EXTRA_DIST += test-dilation-lena-bench.in
test-dilation-lena-bench: test-dilation-lena-bench.in Makefile
	rm -f $@ $@.tmp
	srcdir='';				\
	  test -f ./$< || srcdir=$(srcdir)/;	\
	  $(edit) $${srcdir}$< >>$@.tmp
	chmod a-w,a+x $@.tmp
	mv $@.tmp $@
CLEANFILES = test-dilation-lena-bench

TESTS = test-dilation-lena-bench


## --------------------- ##
## Watershed transform.  ##
## --------------------- ##

noinst_PROGRAMS +=				\
  watershed-lena

watershed_lena_SOURCES = watershed-lena.cc

MOSTLYCLEANFILES +=				\
  watershed-lena-out-512-nongen.pgm		\
  watershed-lena-out-512-gen.pgm		\
  watershed-lena-out-512-fast.pgm		\
  watershed-lena-out-1024-nongen.pgm		\
  watershed-lena-out-1024-gen.pgm		\
  watershed-lena-out-1024-fast.pgm		\
  watershed-lena-out-2048-nongen.pgm		\
  watershed-lena-out-2048-gen.pgm		\
  watershed-lena-out-2048-fast.pgm
