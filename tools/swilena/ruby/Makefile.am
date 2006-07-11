## Process this file through Automake to produce Makefile.in -*- Makefile -*-
##
## Makefile.am for swilena/python
##

SUBDIRS = . tests

rubydir = $(libdir)/ruby

# ltrequire.rb: Extension of the `require' understanding Libtool libraries.
EXTRA_DIST = ltrequire.rb

$(srcdir)/makefile.swig: $(top_srcdir)/tools/swilena/expand.sh
	top_srcdir="$(top_srcdir)" $< $(top_srcdir)/tools/swilena/
include $(srcdir)/makefile.swig
