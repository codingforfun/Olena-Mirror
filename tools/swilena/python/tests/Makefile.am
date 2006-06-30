## Process this file through Automake to produce Makefile.in -*- Makefile -*-
##
## Makefile.am for swilena/python/tests
##

dist_noinst_DATA = sanity.py arith.py conversions.py simple1.py morpho.py

TESTS_ENVIRONMENT = \
	PYTHONPATH="$(PYTHONPATH):..:$(srcdir)/.." \
	IMGDIR="$(top_srcdir)/olena/img"

TESTS = $(dist_noinst_DATA)
