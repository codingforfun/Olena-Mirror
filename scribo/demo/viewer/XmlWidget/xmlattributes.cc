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

#include "xmlattributes.hh"

XmlAttributes::XmlAttributes()
{
}

XmlAttributes::XmlAttributes(const QDomNamedNodeMap& node)
{
    load(node);
}

void XmlAttributes::load(const QDomNamedNodeMap& node)
{
    QDomAttr attr;

    for(int i = 0; i < node.count(); i++)
    {
        attr = node.item(i).toAttr();

        names_ << attr.name();
        values_ << attr.value();
    }
}

QStringList XmlAttributes::names() const
{
  return names_;
}

QStringList XmlAttributes::values() const
{
  return values_;
}
