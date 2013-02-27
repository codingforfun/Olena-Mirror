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

#include "Rendering/regionitem.hh"

RegionItem::RegionItem(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
    init();
}

RegionItem::RegionItem(const QPolygonF& polygon, QGraphicsItem *parent, QGraphicsScene *scene):
        QGraphicsPolygonItem(polygon, parent, scene)
{
    init();
}

void RegionItem::init()
{
    xmlItem_ = 0;

    selectedPen_.setStyle(Qt::SolidLine);
    unselectedPen_.setStyle(Qt::SolidLine);

    selectedPen_.setWidthF(2);
    unselectedPen_.setWidth(0);

    selectedPen_.setCapStyle(Qt::SquareCap);
    unselectedPen_.setCapStyle(Qt::SquareCap);

    selectedBrush_.setStyle(Qt::SolidPattern);
    unselectedBrush_.setStyle(Qt::SolidPattern);
}

void RegionItem::setColor(const QColor& color)
{
    selectedPen_.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 255));
    unselectedPen_.setColor(color);

    selectedBrush_.setColor(color);
    unselectedBrush_.setColor(QColor::fromRgb(color.red(), color.green(), color.blue(), 40));

    setPen(unselectedPen_);
    setBrush(unselectedBrush_);
}

void RegionItem::setSelected(bool isSelected)
{
    if(isSelected)
    {
        isSelected_ = true;
        setPen(selectedPen_);
        setBrush(selectedBrush_);
    }

    else
    {
        isSelected_ = false;
        setPen(unselectedPen_);
        setBrush(unselectedBrush_);
    }
}


void RegionItem::setXmlItem(XmlItem *xmlItem)
{
  xmlItem_ = xmlItem;
}

XmlItem *RegionItem::xmlItem() const
{
  return xmlItem_;
}

QColor RegionItem::color() const
{
  return selectedBrush_.color();
}

int RegionItem::region() const
{
  return region_;
}

bool RegionItem::isSelected()
{
  return isSelected_;
}

void RegionItem::select()
{
  setSelected(true);
}

void RegionItem::unselect()
{
  setSelected(false);
}

void RegionItem::loadData(const GraphicsRegion::Data& data)
{
  region_ = data.region; setColor(data.color); setZValue(data.zValue);
}

bool RegionItem::contains(const QPointF& point)
{
  return (boundingRect().width() * boundingRect().height() == 0
	  || boundingRect().contains(point))
    && shape().contains(point);
}

bool RegionItem::intersects(const QRectF& rect)
{
  return (boundingRect().width() * boundingRect().height() == 0
	  || boundingRect().intersects(rect))
    && shape().intersects(rect);
}
