noinst_DATA = oln-ref.ps oln-ref.pdf html/oln-ref.html

tex_TEX = oln-ref.tex
built_oln_ref_TEX = \
  ref-morpho.tex \
  ref-level.tex
oln_ref_TEX = \
  $(built_oln_ref_TEX) \
  ref-types.tex

TEXI2PDF = $(TEXI2DVI) --pdf

oln-ref.dvi: $(tex_TEX) $(oln_ref_TEX)
	$(TEXI2DVI) $(tex_TEX)

oln-ref.pdf: $(tex_TEX) $(oln_ref_TEX)
	$(TEXI2PDF) $(tex_TEX)

.dvi.ps:
	$(DVIPS) $< -o $@

html/oln-ref.html: $(tex_TEX) $(oln_ref_TEX)
	$(HEVEA) $(tex_TEX) -o html/oln-ref.html
	$(HEVEA) $(tex_TEX) -o html/oln-ref.html

olnsrc = $(top_srcdir)/../oln/

COMMONGETDEFSFLAGS =  --agarg=-DCXX=@CXX@ --agarg=--templ-dirs=../tpl
PROCESSINGGETDEFSFLAGS = \
  --subblock=tpl=type,name,doc \
  --subblock=arg=type,name,mode,doc \
  --listattr=exh \
  --listattr=ns \
  --template ../tpl/processing.tpl \
  --srcfile

morpho_files = $(shell fgrep -l '/*=' \
	         $(olnsrc)/morpho/*.hh $(olnsrc)/morpho/*.inc)

ref-morpho.tex: $(morpho_files) $(srcdir)/../tpl/processing.tpl
	$(GETDEFS) $(COMMONGETDEFSFLAGS) $(PROCESSINGGETDEFSFLAGS) \
	  $(GETDEFSFLAGS) --base-name=ref-morpho $(morpho_files)

level_files = $(shell fgrep -l '/*=' $(olnsrc)/level/*.hh)

ref-level.tex: $(level_files) $(srcdir)/../tpl/processing.tpl
	$(GETDEFS) $(COMMONGETDEFSFLAGS) $(PROCESSINGGETDEFSFLAGS) \
	  $(GETDEFSFLAGS) --base-name=ref-level $(level_files)

CLEANFILES = $(built_oln_ref_TEX) $(noinst_DATA) *.pdf *.eps *.ps
CLEANFILES += *.aux *.bbl *blg *.dvi *.lot *.log *.out *.toc html/* bin/*
