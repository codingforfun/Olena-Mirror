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

#include "Rendering/rootgraphicsitem.hh"

RootGraphicsItem::RootGraphicsItem(int numberRegion)
{
  childsFromRegion_.fill(QList<QGraphicsItem *>(), numberRegion);
}

RootGraphicsItem::~RootGraphicsItem()
{
  foreach(QList<QGraphicsItem *> list, childsFromRegion_)
  {
    foreach(QGraphicsItem *child, list)
    {
      delete child;
    }
  }
}

QList<QGraphicsItem *>
RootGraphicsItem::childsFrom(const GraphicsRegion::Id& region) const
{
  return childsFromRegion_[region];
}

void
RootGraphicsItem::addItemFrom(QGraphicsItem *graphicalItem,
			      const GraphicsRegion::Id& region)
{
  childsFromRegion_[region] << graphicalItem;
}
