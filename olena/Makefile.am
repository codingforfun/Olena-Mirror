## Process this file with Automake to produce Makefile.in -*- Makefile -*-

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
