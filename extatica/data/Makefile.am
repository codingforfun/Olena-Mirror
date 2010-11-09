# Copyright (C) 2005, 2009, 2010 EPITA Research and Development
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


# Do not install me.
lib_LTLIBRARIES = libextatica_function.la
# FIXME: This is probably not portable!  
# libextatica_function_la_LDFLAGS = `xtc-config --libtool-libs`
# libextatica_function_la_CXXFLAGS = `xtc-config --cflags`
libextatica_function_la_LDFLAGS = $(top_builddir)/src/libextatica.la -export-dynamic
libextatica_function_la_CPPFLAGS = -I$(top_builddir)/src -I$(top_srcdir)/src
libextatica_function_la_SOURCES = function.cc

pkgdata_DATA = Makefile.template Makefile.repository

Makefile.template: Makefile
	cat $< | \
	sed -e 's|^srcdir = .*|srcdir = '`cd $(srcdir) && pwd`'|' \
	    -e 's|^top_srcdir = .*|top_srcdir = '`cd $(top_srcdir) && pwd`'|' \
	    -e 's|^top_builddir = .*|top_builddir = '`cd $(top_builddir) && pwd`'|' \
	    -e 's|^pkgdata_DATA = .*|pkgdata_DATA = |' \
	    -e 's|^CLEANFILES = .*|CLEANFILES = |' \
	    -e 's|^Makefile: |Makefile.old: |' \
	    -e 's|^VPATH = .*|VPATH = .|' > $@

Makefile.repository: Makefile
	cat $< | \
	sed -e 's|^srcdir = .*|srcdir = '`cd $(srcdir) && pwd`'|' \
	    -e 's|^top_srcdir = .*|top_srcdir = '`cd $(top_srcdir) && pwd`'|' \
	    -e 's|^top_builddir = .*|top_builddir = '`cd $(top_builddir) && pwd`'|' \
	    -e 's|^pkgdata_DATA = .*|pkgdata_DATA = |' \
	    -e 's|^CLEANFILES = .*|CLEANFILES = make.out|' \
	    -e 's|^lib_LTLIBRARIES = .*|lib_LTLIBRARIES = |' \
	    -e 's|^BUILT_SOURCES = .*|BUILT_SOURCES = |' \
	    -e 's/^SUBDIRS = .*/SUBDIRS = $$(shell ls | grep -v Makefile)/' \
	    -e 's|^Makefile: |Makefile.old: |' \
	    -e 's|^VPATH = .*|VPATH = .|' | grep -v '^include ' > $@

CLEANFILES = Makefile.template Makefile.repository
