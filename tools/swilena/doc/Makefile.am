## Copyright (C) 2002, 2003  EPITA Research and Development Laboratory
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


# FIXME: install the examples somewhere
EXTRA_DIST = examples
dist_noinst_DATA = swilena.dvi swilena.pdf
info_TEXINFOS = swilena.texi

###
### What gets installed
###

docdir = $(datadir)/doc/@PACKAGE_TARNAME@

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	@ for p in $(dist_noinst_DATA); do \
	  if test -f $$p; then d=.; else d=$(srcdir); fi; \
	  if test -f $$d/$$p; then \
	    echo " $(INSTALL_DATA) $$d/$$p $(DESTDIR)$(docdir)/$$p"; \
	    $(INSTALL_DATA) $$d/$$p $(DESTDIR)$(docdir)/$$p; \
	  else : ; fi; \
	done

uninstall-hook:
	@ for p in $(dist_noinst_DATA); do \
	  p1=`basename "$$p"`; \
	  echo " rm -f $(DESTDIR)$(docdir)/$$p1"; \
	  rm -f $(DESTDIR)$(docdir)/$$p1; \
	done
