# ###
# ### General definitions
# ###
EXTRA_DIST = out/exdoc.config.in
olnsrc = $(top_srcdir)/olena/oln
DOXYGEN = doxygen
EXDOC = $(srcdir)/exdoc.pl
OUT_DIR = "out"
IMG_CONV = $(srcdir)/img_conv.pl



doc: html.tar.gz oln-ref.pdf

doxy:
	$(EXDOC) --config=$(OUT_DIR)/exdoc.config  --output-dir=$(OUT_DIR) --input-dir=$(top_srcdir)
	mkdir -p img
	cd $(OUT_DIR) && make -f makefile
	$(IMG_CONV) $(top_srcdir)/olena/img img
	$(IMG_CONV) img img
	$(DOXYGEN) doxygen.config

html.tar.gz: doxy
	$(TAR) -czf $@ html

oln-ref.pdf: doxy
	cd latex && make && cp refman.pdf ../$@

dist_noinst_DATA = oln-ref.pdf html.tar.gz $(srcdir)/exdoc.pl $(srcdir)/img_conv.pl

dist-local:
	rm -f img/*

MAINTAINERCLEANFILES = $(dist_noinst_DATA) \
	./html/* ./img/* $(OUT_DIR)/*

# ###
# ### What gets installed.
# ###

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

