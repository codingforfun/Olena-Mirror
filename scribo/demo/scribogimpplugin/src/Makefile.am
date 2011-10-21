## Process this file with automake to produce Makefile.in

include $(top_srcdir)/../../../build-aux/autotroll.mk

bin_PROGRAMS = scribo-plugin

bindir = $(GIMP_LIBDIR)/plug-ins

scribo_plugin_SOURCES = 		\
	main.cc 			\
	preprocessing_options.cc 	\
	segmentation_options.cc 	\
	ocr_options.cc 			\
	general_options.cc 		\
	preferences_dialog.cc 		\
	config.cc			\
	option_widget.cc		\
	process.cc			\
	preprocess.cc			\
	runner.cc

BUILT_SOURCES = preprocessing_options.moc.cc	\
		preprocessing_options.ui.h	\
		segmentation_options.moc.cc	\
		segmentation_options.ui.h	\
		ocr_options.moc.cc		\
		ocr_options.ui.h		\
		preferences_dialog.moc.cc	\
		preferences_dialog.ui.h		\
		general_options.moc.cc		\
		general_options.ui.h		\
		process.moc.cc			\
		preprocess.moc.cc		\
		runner.moc.cc

nodist_scribo_plugin_SOURCES = $(BUILT_SOURCES)

CLEANFILES = $(BUILT_SOURCES)

noinst_HEADERS = preprocessing_options.hh	\
		 segmentation_options.hh	\
		 ocr_options.hh			\
		 general_options.hh 		\
		 preferences_dialog.hh		\
		 config.hh			\
		 option_widget.hh		\
		 runner.hh

scribo_plugin_CPPFLAGS =			\
	$(QT_CPPFLAGS) $(AM_CPPFLAGS)		\
	$(TESSERACT_CPPFLAGS) 			\
	-DLOCALEDIR=\""$(LOCALEDIR)"\"		\
	-DDATADIR=\""$(DATADIR)"\"		\
	-I$(top_srcdir)				\
	-I$(srcdir)				\
	-I$(top_builddir)			\
	-I$(OLN)/milena				\
	-I$(OLN)/scribo				\
	-I$(OLN)/scribo/demo/viewer		\
	@GIMP_CFLAGS@				\
	-DNDEBUG -DMLN_WO_GLOBAL_VARS

scribo_plugin_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -O3
scribo_plugin_LDFLAGS  = $(QT_LDFLAGS) $(TESSERACT_LDFLAGS) $(LDFLAGS)
scribo_plugin_LDADD    = $(QT_LIBS) $(LDADD) $(GIMP_LIBS)

EXTRA_DIST =			\
	general_options.ui	\
	preferences_dialog.ui 	\
	preprocessing_options.ui\
	segmentation_options.ui	\
	ocr_options.ui



