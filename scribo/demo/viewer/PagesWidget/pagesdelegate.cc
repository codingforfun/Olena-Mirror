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

#include "pagesdelegate.hh"

PagesDelegate::PagesDelegate(QObject *parent) :
        QStyledItemDelegate(parent)
{
    pen_.setColor(QColor::fromRgb(255, 69, 0, 255));
    pen_.setWidth(2);
}

void PagesDelegate::paint(QPainter *painter,
			  const QStyleOptionViewItem& option,
			  const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if(index.row() == index.data(Qt::UserRole+1).toInt())
    {
        painter->setPen(pen_);
        painter->drawRect(option.rect);
        painter->fillRect(option.rect, QColor::fromRgb(255, 69, 0, 60));
    }
}
