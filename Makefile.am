# Copyright (C) 2006, 2007, 2008, 2009 EPITA Research and Development
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

SUBDIRS = build-aux milena

if ENABLE_SWILENA
  SUBDIRS += swilena
endif ENABLE_SWILENA

#<<lrde
if ENABLE_SCRIBO
  SUBDIRS += scribo
endif ENABLE_SCRIBO
#>>

# Target shortcuts delegating the actual action to milena/Makefile.
LOCAL_RECURSIVE_TARGETS = pretty-check tests
$(LOCAL_RECURSIVE_TARGETS):
	cd milena && $(MAKE) $(AM_MAKEFLAGS) $@
.PHONY: $(LOCAL_RECURSIVE_TARGETS)

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += milena scribo


#<<lrde
## FIXME: All of this it too much ad hoc.  Use Git branches to
## distinguish distributed files from non distributed ones after the
## conversion of the repository.

LRDE_STRIPPED_DIST_FILES =				\
  $(distdir)/Makefile.am				\
  $(distdir)/configure.ac

LRDE_NODIST_FILES =					\
  $(distdir)/scribo

# Do not distribute parts tagged `lrde' (see build-aux/strip-lrde).
# Then, regen files.
dist-hook:
	for f in $(LRDE_STRIPPED_DIST_FILES); do	\
	  $(top_srcdir)/build-aux/strip-lrde $$f;	\
	done;						\
	rm -rf $(LRDE_NODIST_FILES)			\
	&&						\
	(						\
	  cd $(distdir);				\
	  set -evx;					\
	  autoreconf -f -v -i;				\
	  find . -name autom4te.cache | xargs rm -rf	\
	)


# Set the package version to olena-x.x-snapshot-dd-mm-yy.
#
# Do not check NEWS file (this is not a release so there is no
# entries in this file for this package).
snapshot:
	current_date=`date +'%d-%m-%y'` 				\
	&& sed -e "s/^\(AC_INIT(\[Olena\],\) \[\([\.[:alnum:]]*\)\(-snapshot-.*-[[:alnum:]]*\)*\], \(.*\)/\1 [\2-snapshot-$$current_date], \4/g" 	\
	       -e 's/ check-news / /g'					\
	       $(srcdir)/configure.ac > $(srcdir)/configure.ac.tmp 	\
	&& cp -f $(srcdir)/configure.ac $(srcdir)/configure.ac.old 	\
	&& cp -f $(srcdir)/configure.ac.tmp $(srcdir)/configure.ac 	\
	&& $(MAKE) $(AM_MAKEFLAGS) dist 				\
	&& mv -f $(srcdir)/configure.ac.old $(srcdir)/configure.ac
#>>
