# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE).
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


## ------------------ ##
## Generic material.  ##
## ------------------ ##

## FIXME: Factor as much as possible.  See how we handled this in TC.

## FIXME: Adjust.

AM_CPPFLAGS = -I$(PYTHONINC) -I$(top_builddir)/src -I$(top_srcdir)/src
## FIXME: Define an equivalent to Olena's TOOLS_CXXFLAGS?
##TOOLS_CXXFLAGS = @TOOLS_CXXFLAGS@
##AM_CXXFLAGS = $(TOOLS_CXXFLAGS)
## FIXME: Warning, `AM_SWIGFLAGS' is not a standard Automake variable.
AM_SWIGFLAGS = -Wall -c++ -python -I$(top_builddir)/src -I$(top_srcdir)/src
# We build modules, not plain libs.
AM_LDFLAGS = -avoid-version -module -shared
# Libraries with their dependencies.
libextatica_la = $(top_builddir)/src/libextatica.la
libextatica_milena_la = $(top_builddir)/src/wrappers/libextatica-milena.la $(libextatica_la)

# Run Swig to create the C++ wrapper files, the Python interface
# files, and the dependency Makefile snippets.
# FIXME: `%'-style patterns rules are not portable.
%-wrap.cc %.py: $(top_srcdir)/swig/%.i
@AMDEP_TRUE@	if $(SWIG) $(AM_SWIGFLAGS) $(SWIGFLAGS) -MD -MF "$(DEPDIR)/$*-wrap.Tcc" -o $@ $<; then \
@AMDEP_TRUE@	  mv -f "$(DEPDIR)/$*-wrap.Tcc" "$(DEPDIR)/$*-wrap.Pcc";\
@AMDEP_TRUE@	else \
@AMDEP_TRUE@	  rm -f "$(DEPDIR)/$*-wrap.Tcc"; exit 1;\
@AMDEP_TRUE@	fi
@AMDEP_FALSE@	$(SWIG) $(AM_SWIGFLAGS) $(SWIGFLAGS) -o $@ $<


# ltihooks.py: Python import hooks that understand Libtool libraries.
python_PYTHON = ltihooks.py

# Use Make to generate `mln_config_local.py' instead of `configure',
# as advised by Autoconf's manual (see section ``Installation
# Directory Variables'').
edit = sed							\
         -e 's|@top_srcdir[@]|$(top_srcdir)|g'			\
         -e 's|@abs_srcdir[@]|$(abs_srcdir)|g'			\
         -e 's|@abs_builddir[@]|$(abs_builddir)|g'		\
         -e 's|@abs_top_builddir[@]|$(abs_top_builddir)|g'	\
         -e 's|@MILENA_DIR[@]|$(MILENA_DIR)|g'

BUILT_SCRIPTS_xtc =
$(BUILT_SCRIPTS_xtc): Makefile
	rm -f $@ $@.tmp
	srcdir='';				\
	  test -f ./$@.in || srcdir=$(srcdir)/;	\
	  $(edit) $${srcdir}$@.in >>$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@


## ----------------- ##
## Milena wrappers.  ##
## ----------------- ##

# FIXME: This section should be optional.

# mln/config.py: Milena configuration.
nobase_nodist_python_PYTHON = mln/config.py
# This target is not listed in BUILT_SCRIPTS_xtc, since its action
# potentially needs to create a directory.
mln/config.py: mln/config.py.in Makefile
	rm -f $@ $@.tmp
	test -d mln || mkdir mln &&		\
	  srcdir='';				\
	  test -f ./$@.in || srcdir=$(srcdir)/;	\
	  $(edit) $${srcdir}$@.in >>$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@
EXTRA_DIST = mln/config.py.in
CLEANFILES = mln/config.py mln/config.py[co]
# Use the all-local since ``[...] byte-compilation occurs at install
# time, any files listed in `noinst_PYTHON' will not be compiled''
# (and therefore generated, in our case) at build time, according to
# Automake's manual.
all-local: mln/config.py

# mln_config_local.py: Milena configuration (local version used in tests).
BUILT_SCRIPTS_xtc += mln_config_local.py
EXTRA_DIST += mln_config_local.py.in
CLEANFILES += mln_config_local.py mln_config_local.py[co]
# Same remark as above.
all-local: mln_config_local.py

# Milena wrappers.
nobase_python_PYTHON =				\
  mln/__init__.py				\
  mln/common.py					\
  mln/data.py					\
  mln/debug.py					\
  mln/io/__init__.py				\
  mln/io/pgm.py					\
  mln/morpho/__init__.py			\
  mln/morpho/closing.py				\
  mln/morpho/watershed.py			\
  mln/value.py

# data.py: Access to the data of the distribution (images, meshes,
# etc.).
BUILT_SCRIPTS_xtc += data.py
EXTRA_DIST += data.py.in
CLEANFILES += data.py data.py[co]
# Same remark as above.
all-local: data.py


## ----------------- ##
## Wrapped modules.  ##
## ----------------- ##

## FIXME: All of this should be generated.
## Don't forget to add a `deps-reset' target as in TC.

## FIXME: Dependencies do not work as expected, e.g., touching
## mln/core/point.hh will not void point-wrap.cc, and Swig will not
## regen it (it will merely recompile _point2d.la).  See how other
## projects people handle this.

nodist_python_PYTHON =
pyexec_LTLIBRARIES =

## xtc.
pyexec_LTLIBRARIES += _xtc.la
nodist__xtc_la_SOURCES = xtc-wrap.cc
_xtc_la_LIBADD = $(libextatica_la)
CLEANFILES += $(nodist__xtc_la_SOURCES) xtc.py xtc.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_xtc-wrap.Pcc@am__quote@
nodist_python_PYTHON += xtc.py

# FIXME: Should be optional.
## xtc_mln.
pyexec_LTLIBRARIES += _xtc_mln.la
nodist__xtc_mln_la_SOURCES = xtc_mln-wrap.cc
_xtc_mln_la_LIBADD = $(libextatica_milena_la)
CLEANFILES += $(nodist__xtc_mln_la_SOURCES) xtc_mln.py xtc_mln.py[co]
## Include the dependency files.  Copied from Automake's generated
## case for C++.
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/_xtc_mln-wrap.Pcc@am__quote@
nodist_python_PYTHON += xtc_mln.py


## ------- ##
## Tests.  ##
## ------- ##

include $(top_srcdir)/swig/run.mk

TESTS_ENVIRONMENT =				\
  abs_top_srcdir="$(abs_top_srcdir)"		\
  abs_top_builddir="$(abs_top_builddir)"	\
  $(RUN)
# Ensure `run' is rebuilt before the tests are run.
$(TESTS): $(srcdir)/run.stamp
# The dependency is on `run.in' and not `run', since `run' is
# regenerated at distribution time, and voids the time stamps (which
# we don't want!).
EXTRA_DIST += $(srcdir)/run.stamp
$(srcdir)/run.stamp: $(RUN_IN)
	@rm -f $@
	@rm -f $@.tmp
	@touch $@.tmp
	$(MAKE) $(AM_MAKEFLAGS) $(RUN)
	@mv -f $@.tmp $@

# FIXME: We should not use the `.py' extension: it is not needed, and
# it prevents us from using the same name for both a module (wrapper)
# and a test.  Alas, the script `run' expects a file name with an
# extension as argument.  We could improve this by adding options such
# as `--python' to `run'.
TESTS = milena-libextatica.py milena-libextatica-milena.py

# FIXME: Is this really needed?
EXTRA_DIST += $(TESTS)

# Log produced by libextatica.
CLEANFILES += xtc.log

clean-local: clean-repository
.PHONY: clean-repository
clean-repository:
	-rm -rf repository
