## Process this file with Automake to produce Makefile.in -*-Makefile-*-

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


SUBDIRS = $(OLN_UTILITIES_SUBDIRS)

# AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) $(ZLIB_CXXFLAGS)
# AM_LDFLAGS = $(LDFLAGS) $(ZLIB_LDFLAGS)

# bin_PROGRAMS = oln-mosaic-u8 oln-mosaic-u16
# dist_man_MANS = oln-mosaic-u8.1 oln-mosaic-u16.1

# INCLUDES = -I$(srcdir)/../lib
# LDADD = ../lib/libolntools.a

# oln_mosaic_u8_SOURCES = mosaic.cc
# oln_mosaic_u8_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u8
# if REGEN_MANPAGES
# $(srcdir)/oln-mosaic-u8.1: oln-mosaic-u8$(EXEEXT)
# 	PATH=.:$$PATH $(HELP2MAN) --output=$@ $< 
# endif

# oln_mosaic_u16_SOURCES = mosaic.cc
# oln_mosaic_u16_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u16
# if REGEN_MANPAGES
# $(srcdir)/oln-mosaic-u16.1: oln-mosaic-u16$(EXEEXT)
# 	PATH=.:$$PATH $(HELP2MAN) --output=$@ $<
# endif

# if REGEN_MANPAGES
### FIXME
# This is ugly, but necessary since `make dist' can be called
# before the library was compiled.
# ../lib/libolntools.a:
# 	cd ../lib && $(MAKE) libolntools.a
# endif

# MAINTAINERCLEANFILES = $(srcdir)/oln-mosaic-u8.1 $(srcdir)/oln-mosaic-u16.1
