include $(top_srcdir)/check/Makefile.check

check_PROGRAMS = slow_sum_2d_1 slow_sum_2d_2 sobel_gradient

slow_sum_2d_1_SOURCES = slow_sum_2d_1.cc
slow_sum_2d_2_SOURCES = slow_sum_2d_2.cc
sobel_gradient_SOURCES = sobel_gradient.cc

XFAIL_TESTS = sobel_gradient

CLEANFILES = lena-sobel.pgm
EXTRA_DIST = sum_on_random.pgm
