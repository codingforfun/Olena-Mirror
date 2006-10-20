## Process this file through Automake to produce Makefile.in -*- Makefile -*-
##
## Makefile.am for swilena/python
##

SUBDIRS = . tests

rubydir = $(libdir)/ruby

# ltrequire.rb: Extension of the `require' understanding Libtool libraries.
dist_ruby_SCRIPT = ltrequire.rb

# A utility script to display images (not installed).
# FIXME: Is it still useful?
dist_noinst_ruby_SCRIPT = swilena_display.rb

$(srcdir)/makefile.swig: $(top_srcdir)/tools/swilena/expand.sh
	top_srcdir="$(top_srcdir)" $< $(top_srcdir)/tools/swilena/
include $(srcdir)/makefile.swig
