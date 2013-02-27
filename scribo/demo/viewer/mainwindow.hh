// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MAINWINDOW_HH
# define MAINWINDOW_HH

#include <QPrintPreviewDialog>
#include <QGraphicsView>
#include <QPrintDialog>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QFileDialog>
#include <QGroupBox>
#include <QPrinter>

#include "Preferences/preferencesdialog.hh"
#include "Processing/progressdialog.hh"
#include "PagesWidget/pageswidget.hh"
#include "Rendering/graphicsview.hh"
#include "XmlWidget/xmlwidget.hh"
#include "Processing/runner.hh"
#include "Rendering/scene.hh"
#include "regionwidget.hh"
#include "aboutdialog.hh"
#include "dockwidget.hh"
#include "xml.hh"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT;

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void changeEvent(QEvent *e);

private:
  void initGraphicsRegion();
  void initDialogsWidget();
  void initPageWidget();
  void initRegionWidget();
  void initXmlWidget();
  void initMenuBar();
  void connectWidgets();

  void setActionsEnabled(bool isSegmented);
  void disableActions();

  void updatePageBrowsing();

  void printItems(QPainter *painter, const QList<QGraphicsItem *>& items, QStyleOptionGraphicsItem *options);

  Xml processTmpXml(const QString& filename) const;
  QList<RegionItem *> toRegionItems(QList<XmlItem *> regionItems) const;
  QList<XmlItem *> toXmlItems(QList<RegionItem *> xmlItems) const;

  Ui::MainWindow *ui;

  DockWidget dockRegion_;
  DockWidget dockPages_;
  DockWidget dockXml_;

  GraphicsView graphicsView_;
  Scene scene_;

  PagesWidget pagesWidget_;
  RegionWidget regionWidget_;

  AboutDialog *aboutDialog_;
  ProgressDialog *progressDialog_;
  PreferencesDialog *preferencesDialog_;

  XmlWidget xmlWidget_;
  Xml xml_;
  Runner runner_;

  QAction *segment_;
  QAction *previewPrinting_;
  QAction *print_;
  QAction *export_;

  // Browse among documents.
  QAction *next_;
  QAction *prev_;

public slots:
  void printScene(QPrinter *printer);

private slots:
  void onOpen();
  void onSegment();
  void onPreviewPrint();
  void onPrint();
  void onExportation();
  void onPreferences();
  void onAbout();

  void onXmlSaved(const QString& filename);
  void onFileChanged(const QString& filename);

  void onRegionSelection(QList<RegionItem *> regionItems);

  void onXmlChangeSelection(QList<XmlItem *> xmlItems, bool select);
  void onXmlSelect(QList<XmlItem *> xmlItems);
  void onXmlUnselect(QList<XmlItem *> xmlItems);
};


#endif // MAINWINDOW_HH
