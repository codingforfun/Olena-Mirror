# Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE).
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

# Camera stream processing.

# Find Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_builddir)/milena
# Produce fast code (by default).
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)


if HAVE_OPENCV

noinst_PROGRAMS = camprocess
camprocess_SOURCES = camprocess.cc
# Do not use optimization, as it does not seem to please OpenCV.
camprocess_CPPFLAGS = $(AM_CPPFLAGS) $(OPENCV_CPPFLAGS)
camprocess_CXXFLAGS = -O0
camprocess_LDADD = libprocess.la
camprocess_LDFLAGS = $(AM_LDFLAGS) $(OPENCV_LDFLAGS)

# Convenience library to compile process.cc with optimizations turned on.
noinst_LTLIBRARIES = libprocess.la
libprocess_la_SOURCES = process.hh process.cc
libprocess_la_CPPFLAGS = $(AM_CPPFLAGS) $(OPENCV_CPPFLAGS)
# Force static linking.
libprocess_la_LDFLAGS = -static

endif HAVE_OPENCV
