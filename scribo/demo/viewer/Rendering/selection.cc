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

#include "selection.hh"

Selection::Selection(QGraphicsScene *scene):
    QGraphicsRectItem(0, scene)
{
    init();
}

Selection::Selection(const QRectF &rect, QGraphicsScene *scene):
    QGraphicsRectItem(rect, 0, scene)
{
    init();
}

Selection::Selection(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene):
    QGraphicsRectItem(x, y, width, height, 0, scene)
{
    init();
}

void Selection::init()
{
    // Always drawn on top of the scene.
    setZValue(INT_MAX);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setCosmetic(true);
    setPen(pen);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor::fromRgb(80, 200, 10, 60));
    setBrush(brush);
}
