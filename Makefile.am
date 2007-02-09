## Process this file through Automake to produce Makefile.in -*- Makefile -*-

## Copyright (C) 2002, 2003, 2006, 2007 EPITA Research and Development
## Laboratory
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


# OLN_TOP_SUBDIRS is set when "configure" is run.
SUBDIRS = metalic integre olena $(OLN_TOP_SUBDIRS)

# "configure.ac" in this directory uses macros in m4 files.
ACLOCAL_AMFLAGS = -I config
CONFIGURE_DEPENDENCIES = config/oln.m4

###
### Autoconf macro repository.
###

# oln.m4 is for users, install it with other aclocal data.
m4datadir = $(datadir)/aclocal
dist_m4data_DATA = config/oln.m4

###
### Development files.
###

EXTRA_DIST = bootstrap.sh config/oln-local.m4 contrib/runtests \
  config/strip-lrde

###
### Basic doc files generation.
###

if HAVE_DOC_DEV

dist_noinst_DATA = README BUILD INSTALL THANKS FAQ UPGRADING
MAINTAINERCLEANFILES = README BUILD INSTALL THANKS FAQ UPGRADING

INFOTOTEXT = $(MAKEINFO) --enable-encoding --no-headers --no-validate
devdoc = $(srcdir)/doc/dev

README: $(devdoc)/abstract.texi $(devdoc)/biblio.texi
	$(INFOTOTEXT) $(devdoc)/abstract.texi >$@
	$(INFOTOTEXT) $(devdoc)/biblio.texi >>$@

BUILD: $(devdoc)/build.texi
	$(INFOTOTEXT) $(devdoc)/build.texi >$@

INSTALL: $(devdoc)/install.texi
	$(INFOTOTEXT) $(devdoc)/install.texi >$@

THANKS: $(devdoc)/thanks.texi
	$(INFOTOTEXT) $(devdoc)/thanks.texi >$@

FAQ: $(devdoc)/faq.texi
	$(INFOTOTEXT) $(devdoc)/faq.texi >$@

UPGRADING: $(devdoc)/upgrading.texi
	$(INFOTOTEXT) $(devdoc)/upgrading.texi >$@

endif

DISTCHECK_CONFIGURE_FLAGS = --disable-manpages-regen

# This is totally experimental. Comments are welcome.
maintainer-check:
	if cppi --version >/dev/null 2>&1; then \
	  find $(srcdir) -name '*.{cc,hh,hxx}' | \
	    xargs cppi --check; \
	else \
	  echo "$@: GNU cppi is required" >&2; \
	fi

#<<lrde
## FIXME: Improve integration.
# Remove any trace of olena/oln/lrde from the distribution and reset
# the GNU Build system for the distdir layout.
dist-hook:
	$(top_srcdir)/config/strip-lrde $(distdir)/configure.ac &&	    \
	$(top_srcdir)/config/strip-lrde $(distdir)/Makefile.am &&	    \
	$(top_srcdir)/config/strip-lrde $(distdir)/olena/oln/Makefile.am && \
	rm -rf $(distdir)/olena/oln/lrde &&				    \
	(								    \
	  cd $(distdir);						    \
	  set -evx;							    \
	  ./bootstrap.sh;						    \
	  find . -name autom4te.cache | xargs rm -rf			    \
	)
#>>
