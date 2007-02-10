## Copyright (C) 2003  EPITA Research and Development Laboratory
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


include ../check/Makefile.runtests
include ../check/Makefile.check

EXTRA_DIST += image_print.sh

check_PROGRAMS = multiple_sources

multiple_sources_SOURCES = multiple_sources_1.cc multiple_sources_2.cc
multiple_sources_LDFLAGS = $(AM_LDFLAGS)
multiple_sources_LDADD = $(LDADD) $(FFTW_LDFLAGS)
multiple_sources_CXXFLAGS = $(AM_CXXFLAGS) $(FFTW_CFLAGS)

check-image-print:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(LDFLAGS_RUNTESTS)" \
	CPPFLAGS="$(CPPFLAGS_RUNTESTS)" \
	CXXFLAGS="$(CXXFLAGS_RUNTESTS)" \
	  $(SHELL) $(srcdir)/image_print.sh

check-local: check-runtests check-image-print
