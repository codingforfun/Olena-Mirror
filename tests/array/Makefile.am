include $(top_srcdir)/tests/check/Makefile.check

SUBDIRS = gen_test_utils

check_PROGRAMS = test_array1d test_array2d test_array3d gen_test

test_array1d_SOURCES = test_array1d.cc

test_array2d_SOURCES = test_array2d.cc

test_array3d_SOURCES = test_array3d.cc

gen_test_SOURCES = gen_test.cc

gen_test_LDADD = gen_test_utils/liberrors.a
