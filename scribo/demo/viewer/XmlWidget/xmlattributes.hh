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

#ifndef XMLATTRIBUTES_H
# define XMLATTRIBUTES_H

#include <QDomNamedNodeMap>
#include <QStringList>
#include <QVariant>

class XmlAttributes
{
    public:
        explicit XmlAttributes();
        explicit XmlAttributes(const QDomNamedNodeMap& node);

        void load(const QDomNamedNodeMap& node);

        QStringList names() const;
        QStringList values() const;

    private:
        QStringList names_;
        QStringList values_;
};


Q_DECLARE_METATYPE(XmlAttributes)
Q_DECLARE_METATYPE(XmlAttributes *)

#endif // XMLATTRIBUTES_H
