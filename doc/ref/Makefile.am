# ###
# ### General definitions
# ###

# TEXI2PDF = $(TEXI2DVI) --pdf
# TEXI2PDF_FLAGS = --batch -I $(srcdir)/graphics

# TEXI2DVI_FLAGS = --batch -I $(srcdir)/graphics

olnsrc = $(top_srcdir)/olena/oln

# include $(srcdir)/filelists.make

# $(srcdir)/filelists.make: $(srcdir)/gen_filelists.sh
# 	$(SHELL) $@ $(olnsrc)

# $(srcdir)/Makefile.in: $(srcdir)/filelists.make

# ###
# ### Auto-generation of TeX files.
# ###

# AM_LDFLAGS = $(LDFLAGS) $(ZLIB_LDFLAGS)
# AM_CXXFLAGS = $(CXXFLAGS) $(ZLIB_CXXFLAGS)

# INCLUDES = -I$(top_srcdir)/olena -I$(top_builddir)/olena
# COMMONGETDEFSFLAGS =  --agarg=-DCXXCOMPILE="$(CXX) -I$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(AM_CXXFLAGS) $(AM_LDFLAGS)" \
# 		      --agarg=-DEPSTOPDF="$(EPSTOPDF)" \
# 		      --agarg=-DCONVERT="$(IMAGEMAGICK_CONVERT)" \
# 	              --agarg=--templ-dirs=$(srcdir) \
# 	              --agarg=-DIMGDIR=$(top_srcdir)/olena/img
# PROCESSINGGETDEFSFLAGS = \
#   --subblock=tpl=type,name,doc \
#   --subblock=arg=type,name,mode,doc \
#   --listattr=exh \
#   --listattr=ns \
#   --template $(srcdir)/processing.tpl \
#   --srcfile

# ref-morpho.tex: $(morpho_files) $(srcdir)/processing.tpl
# 	$(GETDEFS) $(COMMONGETDEFSFLAGS) $(PROCESSINGGETDEFSFLAGS) \
# 	  $(GETDEFSFLAGS) --base-name=ref-morpho $(morpho_files)

# ref-level.tex: $(level_files) $(srcdir)/processing.tpl
# 	$(GETDEFS) $(COMMONGETDEFSFLAGS) $(PROCESSINGGETDEFSFLAGS) \
# 	  $(GETDEFSFLAGS) --base-name=ref-level $(level_files)

# ###
# ### Rules to build the manuals.
# ###

# tex_TEX = oln-ref.tex

# built_oln_ref_TEX = ref-morpho.tex ref-level.tex
# oln_ref_TEX = $(built_oln_ref_TEX) integre-ref.tex

# oln-ref.dvi: $(tex_TEX) $(oln_ref_TEX) $(graphics_DIA)
# 	$(TEXI2DVI) $(TEXI2DVI_FLAGS) \
# 	  `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)

DOXYGEN = doxygen
EXDOC = $(srcdir)/exdoc.pl
OUT_DIR = "out"
IMG_CONV = $(srcdir)/img_conv.pl
code:
	$(EXDOC) --config=exdoc.config  --output-dir=$(OUT_DIR) --input-dir=$(top_srcdir)

execute: code
	mkdir -p img
	cd $(OUT_DIR) && make -f makefile

images: execute
	$(IMG_CONV) $(top_srcdir)/olena/img img
	$(IMG_CONV) img img

doxygen: images
	$(DOXYGEN) doxygen.config

html: doxygen


doc: html pdf


oln-ref.pdf: doxygen
	cd latex && make && cp refman.pdf ../$@


# oln-ref.pdf: $(tex_TEX) $(oln_ref_TEX) $(graphics_DIA)
# 	$(TEXI2PDF) $(TEXI2PDF_FLAGS) \
# 	  `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)

# # This rule is not activated by default since the generated files cannot
# # be easily distributed (automake barks loudly when the filenames are
# # not known at the time Makefile.in is generated)

# # FIXME: uncomment when ready
# # cp $(srcdir)/graphics/*-html.png html/

# html.tar.gz: $(tex_TEX) $(oln_ref_TEX) $(graphics_DIA)
# 	$(HEVEA) -fix -o html/oln-ref.html -I $(srcdir) -I $(srcdir)/graphics `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)
# 	cd html && $(HACHA) -tocbis -o index.html oln-ref.html; cd ..
# 	$(TAR) -czf $@ html

html.tar.gz: html
	$(TAR) -czf $@ html

# ###
# ### What gets distributed.
# ###

dist_noinst_DATA = oln-ref.pdf html.tar.gz

# EXTRA_DIST = $(tex_TEX) $(oln_ref_TEX) \
# 	processing.tpl filelists.make gen_filelists.sh

# bin.tar.gz: oln-ref.pdf oln-ref.dvi
# 	$(TAR) -czf $@ `test -f bin/lena256.pdf || echo '$(srcdir)/'`bin

# ###
# ### What gets cleaned.
# ###
# dist-local:
# 	rm -f bin/*a.out$(EXEEXT)
dist-local:
	rm -f img/*

# MAINTAINERCLEANFILES = $(built_oln_ref_TEX) $(dist_noinst_DATA) \
# 	$(srcdir)/html/* $(srcdir)/bin/* \
# 	$(srcdir)/filelists.make

MAINTAINERCLEANFILES = $(dist_noinst_DATA) \
	./html/* ./img/* $(OUT_DIR)/*

# CLEANFILES = = *.tmp *.aux *.bbl *blg *.lot *.log *.out *.toc

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

