info_TEXINFOS = oln-ref.texi
oln_ref_TEXINFOS = ref-morpho.texi

olnsrc = $(top_srcdir)/../src/

GETDEFS = getdefs
COMMONGETDEFSFLAGS = --agarg=--templ-dirs=../tpl
PROCESSINGGETDEFSFLAGS = \
  --subblock=tpl=type,name,doc \
  --subblock=arg=type,name,mode,doc \
  --template ../tpl/processing.tpl \
  --srcfile

morpho_files = $(shell fgrep -l '/*=' \
	         $(olnsrc)/morpho/*.hh $(olnsrc)/morpho/*.inc)
ref-morpho.texi: $(morpho_files) $(srcdir)/../tpl/processing.tpl
	$(GETDEFS) $(COMMONGETDEFSFLAGS) $(PROCESSINGGETDEFSFLAGS) \
	  $(GETDEFSFLAGS) --base-name=ref-morpho $(morpho_files)

## The rest of this file is adapted from gettext-0.1.40/doc/Makefile.am

docdir = $(prefix)/doc/@PACKAGE@
dvidir = $(docdir)
psdir = $(docdir)
pdfdir = $(docdir)
htmldir = $(docdir)

SED = sed
RM = rm -f

MAKEINFO = env LANG= LANGUAGE= @MAKEINFO@

all-local: html ps pdf
install-data-local: install-html install-ps install-pdf
installdirs-local: installdirs-html installdirs-ps installdirs-pdf
uninstall-local: uninstall-html uninstall-ps uninstall-pdf

html: html-split
install-html: install-html-split
uninstall-html: uninstall-html-split

EXTRA_DIST_html = $(shell echo oln-ref*.html)
CLEANFILES = oln-ref.pdf oln-ref*.html
# oln-ref.dvi and oln-ref.ps are already known to automake.
MAINTAINERCLEANFILES = oln-ref_*.html

# Documentation in DVI format.

install-dvi: oln-ref.dvi
	$(mkinstalldirs) $(DESTDIR)$(dvidir)
	$(INSTALL_DATA) `if test -f oln-ref.dvi; then echo .; else echo $(srcdir); fi`/oln-ref.dvi $(DESTDIR)$(dvidir)/oln-ref.dvi

installdirs-dvi:
	$(mkinstalldirs) $(DESTDIR)$(dvidir)

uninstall-dvi:
	$(RM) $(DESTDIR)$(dvidir)/oln-ref.dvi


# Documentation in Postscript format.

DVIPS = @DVIPS@ -D600

ps: oln-ref.ps

oln-ref.ps: oln-ref.dvi
	$(DVIPS) -o $@ `if test -f oln-ref.dvi; then echo oln-ref.dvi; else echo $(srcdir)/oln-ref.dvi; fi`

install-ps: oln-ref.ps
	$(mkinstalldirs) $(DESTDIR)$(psdir)
	$(INSTALL_DATA) `if test -f oln-ref.ps; then echo .; else echo $(srcdir); fi`/oln-ref.ps $(DESTDIR)$(psdir)/oln-ref.ps

installdirs-ps:
	$(mkinstalldirs) $(DESTDIR)$(psdir)

uninstall-ps:
	$(RM) $(DESTDIR)$(psdir)/oln-ref.ps


# Documentation in Portable Document Format.

TEXI2PDF = @TEXI2PDF@
SUFFIXES = .pdf

pdf: oln-ref.pdf

.texi.pdf: version.texi
	TEXINPUTS="$(top_srcdir)/../tools$(PATH_SEPARATOR)$$TEXINPUTS" \
	  MAKEINFO='$(MAKEINFO) -I $(srcdir)' $(TEXI2PDF) $<

install-pdf: oln-ref.pdf
	$(mkinstalldirs) $(DESTDIR)$(pdfdir)
	$(INSTALL_DATA) `if test -f oln-ref.pdf; then echo .; else echo $(srcdir); fi`/oln-ref.pdf $(DESTDIR)$(pdfdir)/oln-ref.pdf

installdirs-pdf:
	$(mkinstalldirs) $(DESTDIR)$(pdfdir)

uninstall-pdf:
	$(RM) $(DESTDIR)$(pdfdir)/oln-ref.pdf


# Documentation in HTML format.

TEXI2HTML = @TEXI2HTML@

html-monolithic: oln-ref.html
html-split: oln-ref_toc.html

oln-ref.html: oln-ref.texi version.texi $(oln-ref_TEXINFOS)
	$(TEXI2HTML) -expandinfo -number -monolithic `if test -f oln-ref.texi; then echo oln-ref.texi; else echo $(srcdir)/oln-ref.texi; fi`

oln-ref_toc.html: oln-ref.texi version.texi $(oln-ref_TEXINFOS)
	case "$(TEXI2HTML)" in \
	  *"/missing texi2html") \
	     $(TEXI2HTML) -expandinfo -number -split_chapter `if test -f oln-ref.texi; then echo oln-ref.texi; else echo $(srcdir)/oln-ref.texi; fi` || exit 0 ;; \
	  *) $(RM) oln-ref_*.html ; \
	     $(TEXI2HTML) -expandinfo -number -split_chapter `if test -f oln-ref.texi; then echo oln-ref.texi; else echo $(srcdir)/oln-ref.texi; fi` ;; \
	esac

install-html-monolithic: oln-ref.html
	$(mkinstalldirs) $(DESTDIR)$(htmldir)
	$(INSTALL_DATA) `if test -f oln-ref.html; then echo .; else echo $(srcdir); fi`/oln-ref.html $(DESTDIR)$(htmldir)/oln-ref.html

install-html-split: oln-ref_toc.html
	$(mkinstalldirs) $(DESTDIR)$(htmldir)
	for file in `if test -f oln-ref_toc.html; then echo .; else echo $(srcdir); fi`/oln-ref_*.html; do \
	  $(INSTALL_DATA) $$file $(DESTDIR)$(htmldir)/`basename $$file`; \
	done

installdirs-html:
	$(mkinstalldirs) $(DESTDIR)$(htmldir)

uninstall-html-monolithic:
	$(RM) $(DESTDIR)$(htmldir)/oln-ref.html

uninstall-html-split:
	$(RM) $(DESTDIR)$(htmldir)/oln-ref_*.html

installdirs: installdirs-local
