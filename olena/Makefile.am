## Process this file with Automake to produce Makefile.in -*- Makefile -*-

## Copyright (C) 2002, 2003, 2004 EPITA Research and Development
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


# OLN_SRC_SUBDIRS is set when "configure" is run.
SUBDIRS = img oln $(OLN_SRC_SUBDIRS)

###
### Development files
###

EXTRA_DIST = oln/config/pconf-hh.in

###
### Stuff pertaining to compiler-specific auto-configuration
###

CLEANFILES = conf/config.log* conf/configure.log* conf/*.log conf/config.status conf/oln-config*.sh conf/*.1

MAINTAINERCLEANFILES = conf/aclocal.m4 conf/autom4te.cache

EXTRA_DIST += conf/configure.ac \
	conf/compilers.def \
	conf/gen-scripts.sh 

dist_pkgdata_DATA = conf/configure conf/oln-config.shin

###
### Experimental stuff: read olena/conf/README
###

#if OLN_CONFIG_SH

# FIXME: when the two following rules are enabled, they are
# always run, resulting in errors if aclocal and/or autoconf
# are not available.

# $(srcdir)/conf/aclocal.m4: $(srcdir)/conf/configure.ac $(srcdir)/config/oln.m4 
#	cd $(srcdir)/conf && $(ACLOCAL) -I .. -I ../..

# $(srcdir)/conf/configure: $(srcdir)/conf/configure.ac $(srcdir)/conf/aclocal.m4
#	cd $(srcdir)/conf && $(AUTOCONF)

#
# FIXME: this section is ugly and exists because no good substitute
# has been found yet. Please improve.
#

#install-data-hook: $(srcdir)/conf/configure $(srcdir)/conf/gen-scripts.sh $(srcdir)/conf/compilers.def
#	mkdir -p conf && $(SHELL) $(srcdir)/conf/gen-scripts.sh $(DESTDIR)$(includedir) $(srcdir)/conf ./conf
#	$(mkinstalldirs) $(DESTDIR)$(bindir)
#	@ for p in conf/oln-config-*.sh; do \
#	  if test -f $$p; then \
#	    f=`echo "$$p" | sed 's,^.*/,,;$(transform)'`; \
#	    echo " $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f"; \
#	    $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f || exit 1; \
#	  else :; fi; \
#	done

#uninstall-hook:
#	@ for p in conf/oln-config-*.sh; do \
#	  f=`echo "$$p" | sed 's,^.*/,,;$(transform)'`; \
#	  echo " rm -f $(DESTDIR)$(bindir)/$$f"; \
#	  rm -f $(DESTDIR)$(bindir)/$$f; \
#	done

#endif
