noinst_PROGRAMS = libswilena.so

INCLUDES = -I/usr/include/python2.2 -I$(top_srcdir)/src -I$(OLN)
libswilena_so_SOURCES = swilena_wrap.cxx
libswilena_so_LDFLAGS = -shared


swilena_wrap.cxx: $(top_srcdir)/src/swilena.i
	swig -c++ -java -I$(top_srcdir)/src -o $@ $<

CLEANFILES = *.java

