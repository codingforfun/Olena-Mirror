## Process this file with Automake to produce Makefile.in -*- Makefile -*-

info_TEXINFOS = oln-dev.texi

dist_noinst_DATA = oln-dev.pdf oln-dev.dvi

oln_dev_TEXINFOS = thanks.texi abstract.texi biblio.texi \
	build.texi install.texi design.texi faq.texi \
	srctree.texi conf.texi upgrading.texi

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
