## Process this file with Automake to produce Makefile.in -*- Makefile -*-

###
### Olena headers
###
EXTRA_DIST = makefile.src

include $(srcdir)/makefile.src

olndir = $(includedir)/oln

nobase_oln_HEADERS = $(OLN_DEP)

## -- FIXME: Improve integration.  ----------------------------------------
##
## The contents of `olena/lrde' should be moved to subdirectory of
## `olena/oln', and the contents of `olena/lrde/ufmt/bin to `olena/tests'
## or to `tools'.
SUBDIRS = . lrde
## ---------------------------------------- FIXME: Improve integration.  --
