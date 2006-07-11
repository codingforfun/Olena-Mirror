## Process this file through Automake to produce Makefile.in -*- Makefile -*-
##
## Makefile.am for swilena/python
##

SUBDIRS = . tests

# ltihooks.py: Python import hooks understanding Libtool libraries.
EXTRA_DIST = ltihooks.py

python_PYTHON =						\
  swilena_display.py					\
  swilena_all1d.py swilena_all2d.py swilena_all3d.py

$(srcdir)/makefile.swig: $(top_srcdir)/tools/swilena/expand.sh
	top_srcdir="$(top_srcdir)" $< $(top_srcdir)/tools/swilena/
include $(srcdir)/makefile.swig
