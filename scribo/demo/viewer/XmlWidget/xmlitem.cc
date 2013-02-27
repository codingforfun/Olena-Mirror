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

#include "xmlitem.hh"

XmlItem::XmlItem(XmlItem *parent, XmlItem *precItem)
{
    init(parent, precItem);
}

XmlItem::XmlItem(const QDomElement& node, XmlItem *parent, XmlItem *precItem)
{
    init(parent, precItem);
    load(node);
}

XmlItem::~XmlItem()
{
    // Delete all childs.
    XmlItem *child;
    foreach(child, childs_)
        delete child;
}

void XmlItem::init(XmlItem *parent, XmlItem *precItem)
{
    isVisible_ = false;
    regionItem_ = 0;
    parent_ = parent;

    if(parent)
        parent->addChild(this);

    if(precItem)
        row_ = precItem->row()+1;
    else
        row_ = 0;
}

void XmlItem::load(const QDomElement& node)
{
    text_ = node.tagName();

    // Load attributes.
    attributes_.load(node.attributes());
}
