# -------------------------------------------------
# Project created by QtCreator 2013-01-21T09:20:54
# -------------------------------------------------
QT += xml
INCLUDEPATH += /lrde/home/stage/froger_a/olena/scribo/ \
    /lrde/home/stage/froger_a/olena/milena/ \
    /lrde/home/stage/froger_a/olena/_build/scribo/demo/
QMAKE_CXXFLAGS += -DNDEBUG \
    -DMLN_WO_GLOBAL_VARS
LIBS += -I/usr/include/graphicsImage \
    -lGraphicsMagick++ \
    -ltesseract_full \
    -ltiff
TARGET = GScribo
TEMPLATE = app
SOURCES += regionwidget.cc \
    mainwindow.cc \
    main.cc \
    xml.cc \
    Rendering/scene.cc \
    Rendering/selection.cc \
    Rendering/regionitem.cc \
    Rendering/graphicsview.cc \
    PagesWidget/pageswidget.cc \
    PagesWidget/pagesmodel.cc \
    PagesWidget/pagesdelegate.cc \
    Preferences/segmentationoptions.cc \
    Preferences/preprocessingoptions.cc \
    Preferences/ocroptions.cc \
    Preferences/generaloptions.cc \
    Preferences/preferencesdialog.cc \
    Processing/progressdialog.cc \
    Processing/preprocess.cc \
    Processing/process.cc \
    Processing/runner.cc \
    XmlWidget/xmlwidget.cc \
    XmlWidget/xmlmodel.cc \
    XmlWidget/xmlitem.cc \
    XmlWidget/xmlattributes.cc \
    XmlWidget/attributesmodel.cc \
    XmlWidget/xmlview.cc
HEADERS += mainwindow.hh \
    variantpointer.hh \
    regionwidget.hh \
    aboutdialog.hh \
    configs.hh \
    region.hh \
    dir.hh \
    xml.hh \
    Rendering/scene.hh \
    Rendering/selection.hh \
    Rendering/regionitem.hh \
    Rendering/graphicsview.hh \
    PagesWidget/pageswidget.hh \
    PagesWidget/pagesmodel.hh \
    Processing/preprocess.hh \
    Processing/process.hh \
    Processing/runner.hh \
    Processing/progressdialog.hh \
    Preferences/segmentationoptions.hh \
    Preferences/preprocessingoptions.hh \
    Preferences/ocroptions.hh \
    Preferences/generaloptions.hh \
    Preferences/optionwidget.hh \
    Preferences/preferencesdialog.hh \
    XmlWidget/xmlwidget.hh \
    XmlWidget/xmlmodel.hh \
    XmlWidget/xmlitem.hh \
    XmlWidget/xmlattributes.hh \
    XmlWidget/selectionproxy.hh \
    XmlWidget/xmldelegate.hh \
    XmlWidget/attributesmodel.hh \
    PagesWidget/pagesdelegate.hh \
    Rendering/rootgraphicsitem.hh \
    XmlWidget/xmlview.hh \
    dockwidget.hh
FORMS += mainwindow.ui \
    aboutdialog.ui \
    Preferences/preferencesdialog.ui \
    Preferences/ocroptions.ui \
    Preferences/segmentationoptions.ui \
    Preferences/generaloptions.ui \
    Preferences/preprocessingoptions.ui
