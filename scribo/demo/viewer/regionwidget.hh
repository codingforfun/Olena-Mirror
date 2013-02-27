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

#ifndef REGIONWIDGET_HH
# define REGIONWIDGET_HH

#include <QTreeWidget>
#include <QLabel>

#include "xml.hh"

class RegionWidget :
  public QTreeWidget
{
  Q_OBJECT;

public:
  explicit RegionWidget(QWidget *parent = 0);

  QString filterString() const;

private:
  QTreeWidgetItem *createRoot(const QString& text,
			      const GraphicsRegion::Id& region,
			      const GraphicsRegion::Id& begin,
			      const GraphicsRegion::Id& end);
  QTreeWidgetItem *createItem(const QString& text,
			      const GraphicsRegion::Id& region,
			      const QColor& color = QColor::fromRgb(255, 255, 255));
  void fillRoot(QTreeWidgetItem *rootItem,
		const GraphicsRegion::Id& region);

  QString filterString_;

private slots:
  void checkStateChanged(QTreeWidgetItem *item);

signals:
  void checkStateChanged(const GraphicsRegion::Id& region,
			 bool checked);
  void checkStateChanged(const QString& filterString);
};


#endif // REGIONWIDGET_HH
