## Process this file through Automake to create Makefile.in -*- Makefile -*-

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


include Makefile.flags

AM_CPPFLAGS += -DCXX=\""$(CXX)"\" \
   -DCPPFLAGS=\""$(CPPFLAGS)"\" \
   -DCXXFLAGS=\""$(CXXFLAGS)"\" \
   -DOLN_EXTRA_CPPFLAGS=\""$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS)"\" \
   -DCXXFLAGS_OPTIMIZE=\""$(CXXFLAGS_OPTIMIZE)"\" \
   -DCXXFLAGS_STRICT=\""$(CXXFLAGS_STRICT)"\" \
   -DCXXFLAGS_STRICT_ERRORS=\""$(CXXFLAGS_STRICT_ERRORS)"\"

check_LIBRARIES = libcheck.a

libcheck_a_SOURCES =				\
  check.hh					\
  main.cc					\
  srcdir.cc					\
  srcdir.hh					\
  verbose.cc					\
  verbose.hh					\
  compile.cc					\
  compile.hh

EXTRA_DIST = Makefile.flags Makefile.check
