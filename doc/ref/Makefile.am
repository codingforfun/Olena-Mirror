# ###
# ### General definitions
# ###
include $(top_srcdir)/olena/oln/makefile.src
include $(top_srcdir)/metalic/mlc/makefile.src
include $(top_srcdir)/integre/ntg/makefile.src

EXTRA_DIST = out/exdoc.config.in
olnsrc = $(top_srcdir)/olena/oln
EXDOC = $(srcdir)/exdoc.pl
OUT_DIR = "out"
IMG_CONV = $(srcdir)/img_conv.pl
CLEANFILES = html.tar.gz \
	oln-ref.pdf
HDRS_DEP = $(NTG_DEP)
HDRS_DEP += $(MLC_DEP)
HDRS_DEP += $(OLN_DEP)
VPATH += $(top_srcdir)/olena/oln:$(top_srcdir)/integre/ntg:$(top_srcdir)/metalic/mlc:$(top_builddir)/olena/oln

doc: html.tar.gz oln-ref.pdf

html.tar.gz: $(HDRS_DEP)
	mkdir -p img
	$(EXDOC) --config=$(OUT_DIR)/exdoc.config  --output-dir=$(OUT_DIR) --input-dir=$(top_srcdir)/olena
	cd $(OUT_DIR) && $(MAKE) -f makefile
	$(EXDOC) --config=$(OUT_DIR)/exdoc.config  --output-dir=$(OUT_DIR) --input-dir=$(top_srcdir)/integre
	cd $(OUT_DIR) && $(MAKE) -f makefile
	$(EXDOC) --config=$(OUT_DIR)/exdoc.config  --output-dir=$(OUT_DIR) --input-dir=$(top_srcdir)/metalic
	cd $(OUT_DIR) && $(MAKE) -f makefile
	$(IMG_CONV) $(top_srcdir)/olena/img img preserve
	$(IMG_CONV) img img nopreserve
	$(DOXYGEN) doxygen.config
	$(TAR) -czf $@ html

oln-ref.pdf: html.tar.gz
	$(MAKE) -C ./latex || echo "Discarding problems..." # FIXME: Find a better way to use large graph with pdflatex.
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




