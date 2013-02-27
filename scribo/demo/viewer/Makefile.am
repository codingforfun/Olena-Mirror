# Copyright (C) 2009, 2010, 2011 EPITA Research and Development
# Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

include $(top_srcdir)/build-aux/autotroll.mk


bin_PROGRAMS = scribo-viewer
scribo_viewer_SOURCES = mainwindow.cc		\
			aboutdialog.cc		\
			configs.cc		\
			main.cc			\
			regionwidget.cc		\
			xml.cc			\
			dockwidget.cc		\
			PagesWidget/pagesdelegate.cc		\
			PagesWidget/pagesmodel.cc		\
			PagesWidget/pageswidget.cc		\
			Preferences/generaloptions.cc		\
			Preferences/ocroptions.cc		\
			Preferences/preferencesdialog.cc	\
			Preferences/preprocessingoptions.cc	\
			Preferences/segmentationoptions.cc	\
			Processing/preprocess.cc		\
			Processing/process.cc			\
			Processing/progressdialog.cc		\
			Processing/runner.cc			\
			Rendering/graphicsview.cc		\
			Rendering/regionitem.cc			\
			Rendering/scene.cc			\
			Rendering/selection.cc			\
			Rendering/rootgraphicsitem.cc		\
			XmlWidget/attributesmodel.cc	\
			XmlWidget/xmlattributes.cc	\
			XmlWidget/xmlitem.cc		\
			XmlWidget/xmlmodel.cc		\
			XmlWidget/xmlview.cc		\
			XmlWidget/xmlwidget.cc



nodist_scribo_viewer_SOURCES = $(BUILT_SOURCES)



scribo_viewer_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS) -I$(srcdir) 	\
			 -I$(builddir)					\
			 -I$(top_srcdir)/milena				\
			 -I$(top_srcdir)/scribo 			\
			 $(MAGICKXX_CPPFLAGS) 				\
			 $(TESSERACT_CPPFLAGS) 				\
		         $(TIFF_CPPFLAGS) 				\
			 -DNDEBUG -DMLN_WO_GLOBAL_VARS



scribo_viewer_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) 			\
			 $(STRICT_ALIASING_CXXFLAGS) -O3



scribo_viewer_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS) 			\
			 $(MAGICKXX_LDFLAGS)				\
			 $(TESSERACT_LDFLAGS)				\
		         $(TIFF_LDFLAGS)



scribo_viewer_LDADD    = $(QT_LIBS) $(LDADD)



BUILT_SOURCES = demodir.hh			\
		dockwidget.moc.cc		\
		Rendering/graphicsview.moc.cc	\
		mainwindow.moc.cc		\
		Preferences/ocroptions.moc.cc	\
		PagesWidget/pagesmodel.moc.cc	\
		PagesWidget/pageswidget.moc.cc	\
		Preferences/preferencesdialog.moc.cc	\
		Processing/preprocess.moc.cc		\
		Preferences/preprocessingoptions.moc.cc	\
		Processing/process.moc.cc		\
		Processing/progressdialog.moc.cc	\
		regionwidget.moc.cc		\
		Processing/runner.moc.cc		\
		Rendering/scene.moc.cc			\
		Preferences/segmentationoptions.moc.cc	\
		XmlWidget/xmlview.moc.cc		\
		XmlWidget/xmlwidget.moc.cc		\
		aboutdialog.ui.h		\
		Preferences/generaloptions.ui.h	\
		mainwindow.ui.h			\
		Preferences/generaloptions.moc.cc	\
		Preferences/ocroptions.ui.h	\
	 	Preferences/preferencesdialog.ui.h	\
		Preferences/preprocessingoptions.ui.h	\
		Preferences/segmentationoptions.ui.h	\
		XmlWidget/attributesmodel.moc.cc


noinst_HEADERS = mainwindow.hh			\
		 aboutdialog.hh			\
		 regionwidget.hh		\
		 xml.hh				\
		 PagesWidget/pagesdelegate.hh	\
		 PagesWidget/pagesmodel.hh	\
		 PagesWidget/pageswidget.hh	\
		 Preferences/generaloptions.hh	\
		 Preferences/ocroptions.hh	\
		 Preferences/optionwidget.hh		\
		 Preferences/preferencesdialog.hh	\
		 Preferences/preprocessingoptions.hh	\
		 Preferences/segmentationoptions.hh	\
		 Processing/preprocess.hh		\
		 Processing/process.hh			\
		 Processing/progressdialog.hh		\
		 Processing/runner.hh			\
		 Rendering/graphicsview.hh		\
		 Rendering/regionitem.hh		\
		 Rendering/rootgraphicsitem.hh		\
		 Rendering/scene.hh			\
		 Rendering/selection.hh			\
		 XmlWidget/attributesmodel.hh		\
		 XmlWidget/selectionproxy.hh		\
		 XmlWidget/xmlattributes.hh		\
		 XmlWidget/xmldelegate.hh		\
		 XmlWidget/xmlitem.hh			\
		 XmlWidget/xmlmodel.hh			\
		 XmlWidget/xmlview.hh			\
		 XmlWidget/xmlwidget.hh




# Generated files
do_subst = sed  					\
	   -e 's|@libexecdir[@]|$(libexecdir)|g'		\
	   -e 's|@abs_demodir[@]|$(abs_top_builddir)/scribo/demo|g'



demodir.hh: $(srcdir)/demodir.hh.in Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(do_subst) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@



CLEANFILES = $(BUILT_SOURCES)



EXTRA_DIST = BUGS			\
	     README			\
	     aboutdialog.ui		\
	     mainwindow.ui		\
	     Preferences/generaloptions.ui	\
	     Preferences/ocroptions.ui		\
	     Preferences/preferencesdialog.ui	\
	     Preferences/preprocessingoptions.ui	\
	     Preferences/segmentationoptions.ui	\
	     demodir.hh.in

