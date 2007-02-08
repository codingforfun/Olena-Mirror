## Process this file with Automake to produce Makefile.in -*- Makefile -*-

###
### Olena headers
###
EXTRA_DIST = makefile.src

include $(srcdir)/makefile.src

olndir = $(includedir)/oln

nobase_oln_HEADERS = $(OLN_DEP)

SUBDIRS = .
#<<lrde
## FIXME: Improve integration.
##
## The directory `olena/oln/lrde' is a place where we work on algorithms
## not mature enough to integrate the distribution.  Hence it is only
## available from the Subversion repository, not from the distribution.
SUBDIRS += lrde
#>>
