###
### General definitions
###

TEXI2PDF = $(TEXI2DVI) --pdf

olnsrc = $(top_srcdir)/oln/oln

include $(srcdir)/filelists.make

$(srcdir)/filelists.make: $(srcdir)/gen_filelists.sh
	$(SHELL) $@ $(olnsrc)

$(srcdir)/Makefile.in: $(srcdir)/filelists.make

###
### Auto-generation of TeX files.
###

INCLUDES = -I$(top_srcdir)/oln -I$(top_builddir)/oln
COMMONGETDEFSFLAGS =  --agarg=-DCXXCOMPILE="$(CXX) -I$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(OLN_CPPFLAGS) $(CXXFLAGS) $(OLN_CXXFLAGS)" \
		      --agarg=-DEPSTOPDF="$(EPSTOPDF)" \
		      --agarg=-DCONVERT="$(IMAGEMAGICK_CONVERT)" \
	              --agarg=--templ-dirs=$(srcdir) \
	              --agarg=-DIMGDIR=$(top_srcdir)/oln/img
PROCESSINGGETDEFSFLAGS = \
  --subblock=tpl=type,name,doc \
  --subblock=arg=type,name,mode,doc \
  --listattr=exh \
  --listattr=ns \
  --template $(srcdir)/processing.tpl \
  --srcfile

ref-morpho.tex: $(morpho_files) $(srcdir)/processing.tpl
	$(GETDEFS) $(COMMONGETDEFSFLAGS) $(PROCESSINGGETDEFSFLAGS) \
	  $(GETDEFSFLAGS) --base-name=ref-morpho $(morpho_files)

ref-level.tex: $(level_files) $(srcdir)/processing.tpl
	$(GETDEFS) $(COMMONGETDEFSFLAGS) $(PROCESSINGGETDEFSFLAGS) \
	  $(GETDEFSFLAGS) --base-name=ref-level $(level_files)

###
### Rules to build the manuals.
###

tex_TEX = oln-ref.tex

built_oln_ref_TEX = ref-morpho.tex ref-level.tex
oln_ref_TEX = $(built_oln_ref_TEX) ref-types.tex

oln-ref.dvi: $(tex_TEX) $(oln_ref_TEX)
	$(TEXI2DVI) `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)

oln-ref.pdf: $(tex_TEX) $(oln_ref_TEX)
	$(TEXI2PDF) `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)

# This rule is not activated by default since the generated files cannot
# be easily distributed (automake barks loudly when the filenames are
# not known at the time Makefile.in is generated)

html.tar.gz: $(tex_TEX) $(oln_ref_TEX)
	$(HEVEA) -o html/oln-ref.html `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)
	$(HEVEA) -o html/oln-ref.html `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)
	$(TAR) -czf $@ html

###
### What gets distributed.
###

dist_noinst_DATA = oln-ref.pdf oln-ref.dvi bin.tar.gz html.tar.gz

EXTRA_DIST = $(tex_TEX) $(oln_ref_TEX) \
	processing.tpl filelists.make gen_filelists.sh

bin.tar.gz: oln-ref.pdf oln-ref.dvi
	$(TAR) -czf $@ `test -f bin/lena256.pdf || echo '$(srcdir)/'`bin

###
### What gets cleaned.
###
dist-local:
	rm -f bin/*a.out$(EXEEXT)

MAINTAINERCLEANFILES = $(built_oln_ref_TEX) $(dist_noinst_DATA) \
	$(srcdir)/html/* $(srcdir)/bin/* \
	$(srcdir)/filelists.make

CLEANFILES = = *.tmp *.aux *.bbl *blg *.dvi *.lot *.log *.out *.toc  

###
### What gets installed.
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
