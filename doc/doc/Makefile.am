dist_noinst_DATA = oln-ref.ps oln-ref.pdf oln-ref.dvi html/oln-ref.html \
              oln-dev.ps oln-dev.pdf oln-dev.dvi

info_TEXINFOS = oln-dev.texi

tex_TEX = oln-ref.tex

built_oln_ref_TEX = \
  ref-morpho.tex \
  ref-level.tex
oln_ref_TEX = \
  $(built_oln_ref_TEX) \
  ref-types.tex

EXTRA_DIST = $(tex_TEX) $(oln_ref_TEX) 
# html/*.png bin/*.eps bin/*.pgm bin/*.pdf

TEXI2PDF = $(TEXI2DVI) --pdf

oln-ref.dvi: $(tex_TEX) $(oln_ref_TEX)
	$(TEXI2DVI) `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)

oln-ref.pdf: $(tex_TEX) $(oln_ref_TEX)
	$(TEXI2PDF) `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)

html/oln-ref.html: $(tex_TEX) $(oln_ref_TEX)
	$(HEVEA) -o html/oln-ref.html `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)
	$(HEVEA) -o html/oln-ref.html `test -f '$(tex_TEX)' || echo '$(srcdir)/'`$(tex_TEX)

olnsrc = $(OLN_SOURCE_DIR)

COMMONGETDEFSFLAGS =  --agarg=-DCXXCOMPILE="$(CXX) -I$(OLN_INCLUDEDIR) $(CXXFLAGS)" \
	              --agarg=--templ-dirs=$(top_srcdir)/tpl \
	              --agarg=-DIMGDIR=$(top_srcdir)/img
PROCESSINGGETDEFSFLAGS = \
  --subblock=tpl=type,name,doc \
  --subblock=arg=type,name,mode,doc \
  --listattr=exh \
  --listattr=ns \
  --template $(top_srcdir)/tpl/processing.tpl \
  --srcfile

morpho_files = $(shell fgrep -l '/*=' \
	         $(olnsrc)/morpho/*.hh $(olnsrc)/morpho/*.inc)

ref-morpho.tex: $(morpho_files) $(srcdir)/../tpl/processing.tpl
	$(GETDEFS) $(COMMONGETDEFSFLAGS) $(PROCESSINGGETDEFSFLAGS) \
	  $(GETDEFSFLAGS) --base-name=ref-morpho $(morpho_files)

level_files = $(shell fgrep -l '/*=' $(olnsrc)/level/*.hh)

ref-level.tex: $(level_files) $(top_srcdir)/tpl/processing.tpl
	$(GETDEFS) $(COMMONGETDEFSFLAGS) $(PROCESSINGGETDEFSFLAGS) \
	  $(GETDEFSFLAGS) --base-name=ref-level $(level_files)

MAINTAINERCLEANFILES = $(built_oln_ref_TEX) $(dist_noinst_DATA) html/* bin/*a.out
CLEANFILES = = *.tmp *.aux *.bbl *blg *.dvi *.lot *.log *.out *.toc  
