# ###
# ### General definitions
# ###
EXTRA_DIST = out/exdoc.config.in
olnsrc = $(top_srcdir)/olena/oln
DOXYGEN = doxygen
EXDOC = $(srcdir)/exdoc.pl
OUT_DIR = "out"
IMG_CONV = $(srcdir)/img_conv.pl
CLEANFILES = html.tar.gz \
	oln-ref.pdf


doc: html.tar.gz oln-ref.pdf

doxy:
	$(EXDOC) --config=$(OUT_DIR)/exdoc.config  --output-dir=$(OUT_DIR) --input-dir=$(top_srcdir)
	mkdir -p img
	cd $(OUT_DIR) && $(MAKE) -f makefile
	$(IMG_CONV) $(top_srcdir)/olena/img img preserve
	$(IMG_CONV) img img nopreserve
	$(DOXYGEN) doxygen.config

html.tar.gz: doxy
	$(TAR) -czf $@ html

oln-ref.pdf: doxy
	make -C ./latex || echo "Discarding problems..." # FIXME: Find a better way to use large graph with pdflatex.
	cp latex/refman.pdf ./$@

dist_noinst_DATA = oln-ref.pdf html.tar.gz $(srcdir)/exdoc.pl $(srcdir)/img_conv.pl

clean-local:
	rm -rf img out/out* html latex out/makefile out/all.mk

distclean-local:
	rm -f out/exdoc.config


MAINTAINERCLEANFILES = $(dist_noinst_DATA) \
	./html/* ./img/*

# ###
# ### What gets installed.
# ###

docdir = $(datadir)/doc/@PACKAGE_TARNAME@

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	@ for p in $(dist_noinst_DATA); do \
	  p1=`basename "$$p"`; \
	  if test -f $$p; then d=.; else d=$(srcdir); fi; \
	  if test -f $$d/$$p; then \
	    echo " $(INSTALL_DATA) $$d/$$p $(DESTDIR)$(docdir)/$$p"; \
	    $(INSTALL_DATA) $$d/$$p $(DESTDIR)$(docdir)/$$p1; \
	  else : ; fi; \
          done

uninstall-hook:
	@ for p in $(dist_noinst_DATA); do \
	  p1=`basename "$$p"`; \
	  echo " rm -f $(DESTDIR)$(docdir)/$$p1"; \
	  rm -f $(DESTDIR)$(docdir)/$$p1; \
	done

