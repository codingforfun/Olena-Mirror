## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(srcdir)/../check/Makefile.check

check_PROGRAMS = test_array1d test_array2d test_array3d gen_test

test_array1d_SOURCES = test_array1d.cc

test_array2d_SOURCES = test_array2d.cc

test_array3d_SOURCES = test_array3d.cc

gen_test_SOURCES = gen_test.cc
gen_test_LDADD = $(LDADD) gen_test_utils/liberrors.a

check_LIBRARIES = gen_test_utils/liberrors.a
gen_test_utils_liberrors_a_SOURCES = gen_test_utils/errors.cc gen_test_utils/errors.hh

EXTRA_DIST = gen_test_utils/prelude gen_test_utils/postlude
CLEANFILES = fail.log success.log a.out gentest_array.cc
