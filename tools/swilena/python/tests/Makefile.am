## Process this file through Automake to produce Makefile.in -*- Makefile -*-
##
## Makefile.am for swilena/python/tests
##

TESTS_ENVIRONMENT = \
	PYTHONPATH="..:$(srcdir)/..:$(PYTHONPATH):" \
	IMGDIR="$(top_srcdir)/olena/img"

dist_noinst_DATA = sanity.py arith.py conversions.py morpho.py simple1.py

TESTS = $(dist_noinst_DATA)
