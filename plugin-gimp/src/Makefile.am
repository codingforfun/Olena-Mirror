## Process this file with automake to produce Makefile.in

bin_PROGRAMS = milena

bindir = $(GIMP_LIBDIR)/plug-ins

INCS = 	build-image.hh	\
	build-image.hxx	\
	gimp-image.hh	\
	plugin-intl.hh	\
	interface.hxx	\
	interface.hh

milena_SOURCES = main.cc \
		 $(INCS)

milena_CPPFLAGS = \
	-DLOCALEDIR=\""$(LOCALEDIR)"\"		\
	-DDATADIR=\""$(DATADIR)"\"		\
	-I$(top_srcdir)				\
	-I$(OLN)/milena				\
	@GIMP_CFLAGS@				\
	-I$(includedir)				\

CXXFLAGS = -g -O0

LDADD = $(GIMP_LIBS)

