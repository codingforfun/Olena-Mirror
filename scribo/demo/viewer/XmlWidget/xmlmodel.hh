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

#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QAbstractItemModel>

#include "variantpointer.hh"
#include "xmlitem.hh"

class XmlModel :
        public QAbstractItemModel
{
    public:
        explicit XmlModel(XmlItem *rootItem = 0, QObject *parent = 0);
        explicit XmlModel(XmlItem *rootItem, QAbstractItemModelPrivate& dd, QObject *parent = 0);
        ~XmlModel();

        void load(XmlItem *rootItem);

        inline int rowCount(const QModelIndex& parent) const;
        inline int columnCount(const QModelIndex& parent) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(int row, int column, const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& child) const;

    private:
        inline XmlItem *toXmlItem(const QModelIndex& index) const;

        XmlItem *rootItem_;
};

#endif // XMLMODEL_H
