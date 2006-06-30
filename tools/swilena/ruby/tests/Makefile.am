## Process this file through Automake to produce Makefile.in -*- Makefile -*-
##
## Makefile.am for swilena/ruby/tests
##

TESTS_ENVIRONMENT = \
	SWILENA_PATH=".." \
	IMGDIR="$(top_srcdir)/olena/img"

dist_noinst_DATA = sanity.rb simple1.rb

TESTS = $(dist_noinst_DATA)
