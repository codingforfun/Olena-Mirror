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

#ifndef RegionITEM_HH
# define RegionITEM_HH

#include <QGraphicsPolygonItem>
#include <QTreeWidgetItem>
#include <QPen>

#include "variantpointer.hh"
#include "region.hh"

class XmlItem;

class RegionItem :
        public QGraphicsPolygonItem
{
    public:
        explicit RegionItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
        explicit RegionItem(const QPolygonF& path, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

        void setColor(const QColor& color);
        QColor color() const;

        void setXmlItem(XmlItem *xmlItem);
        XmlItem *xmlItem() const;

        void loadData(const GraphicsRegion::Data& data);
        int region() const;

        bool intersects(const QRectF& rect);
        bool contains(const QPointF& point);

        void setSelected(bool isSelected);
        bool isSelected();
        void unselect();
        void select();

    private:
        void init();

        XmlItem *xmlItem_;
        int region_;
        bool isSelected_;

        QPen selectedPen_;
        QPen unselectedPen_;
        QBrush selectedBrush_;
        QBrush unselectedBrush_;
};

#endif // RegionITEM_HH
