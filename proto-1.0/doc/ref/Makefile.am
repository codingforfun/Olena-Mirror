# ###
# ### General definitions
# ###
include $(top_srcdir)/olena/oln/makefile.src
include $(top_srcdir)/metalic/mlc/makefile.src
include $(top_srcdir)/integre/ntg/makefile.src

olnsrc = $(top_srcdir)/olena/oln
OUT_DIR = "out"
CLEANFILES = 	html.tar.gz \
		oln-ref.pdf \
		out/out* out/makefile out/all.mk

clean-local:
	rm -rf img html latex


MAINTAINERCLEANFILES = $(dist_noinst_DATA) \
	./html/* ./img/*

HDRS_DEP = $(NTG_DEP)
HDRS_DEP += $(MLC_DEP)
HDRS_DEP += $(OLN_DEP)
VPATH += $(top_srcdir)/olena/oln:$(top_srcdir)/integre/ntg:$(top_srcdir)/metalic/mlc:$(top_builddir)/olena/oln

doc: html.tar.gz oln-ref.pdf

html.tar.gz: $(HDRS_DEP)
	$(DOXYGEN) doxygen.config
	$(TAR) -czf $@ html

oln-ref.pdf: html.tar.gz
	$(MAKE) -C ./latex || echo "Discarding problems..." # FIXME: Find a better way to use large graph with pdflatex.
	cp latex/refman.pdf ./$@

dist_noinst_DATA = oln-ref.pdf html.tar.gz $(srcdir)/exdoc.pl $(srcdir)/img_conv.pl

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




