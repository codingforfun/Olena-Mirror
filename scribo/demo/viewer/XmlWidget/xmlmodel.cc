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

#include "xmlmodel.hh"

XmlModel::XmlModel(XmlItem *rootItem, QObject *parent) :
        QAbstractItemModel(parent)
{
    rootItem_ = rootItem;
}

XmlModel::XmlModel(XmlItem *rootItem,
		   QAbstractItemModelPrivate& dd,
		   QObject *parent)
  : QAbstractItemModel(dd, parent)
{
    rootItem_ = rootItem;
}

XmlModel::~XmlModel()
{
    if(rootItem_)
        delete rootItem_;
}

QVariant XmlModel::headerData(int, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tr("Tree");

    return QVariant();
}

QVariant XmlModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch(role)
    {
        case Qt::UserRole:
            return static_cast<XmlItem *>(index.internalPointer())->text();

        case Qt::UserRole+1:
            return QVariant::fromValue(static_cast<XmlItem *>(index.internalPointer())->attributes());

        case Qt::UserRole+2:
            return QVariant::fromValue(static_cast<XmlItem *>(index.internalPointer()));

        default:
            return QVariant();
    }
}

QModelIndex XmlModel::index(int row, int column, const QModelIndex& parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    XmlItem *childItem = toXmlItem(parent)->child(row);

    // Create the model index from the child item.
    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex XmlModel::parent(const QModelIndex& child) const
{
    if(!child.isValid())
        return QModelIndex();

    XmlItem *parentItem = static_cast<XmlItem *>(child.internalPointer())->parent();

    if(!parentItem || parentItem == rootItem_)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

void XmlModel::load(XmlItem *rootItem)
{
    beginResetModel();

    if(rootItem_)
        delete rootItem_;

    rootItem_ = rootItem;

    endResetModel();
}


inline XmlItem *XmlModel::toXmlItem(const QModelIndex& parent) const
{
  return !parent.isValid() ? rootItem_ : static_cast<XmlItem *>(parent.internalPointer());
}

inline int XmlModel::rowCount(const QModelIndex& parent) const
{
  if(rootItem_)
    return toXmlItem(parent)->childs().count();
  return 0;
}

inline int XmlModel::columnCount(const QModelIndex&) const
{
  return 1;
}
