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

#include "attributesmodel.hh"

AttributesModel::AttributesModel(const XmlAttributes& attributes, QObject *parent) :
        QAbstractItemModel(parent)
{
    attributes_ = attributes;
}

AttributesModel::AttributesModel(const XmlAttributes& attributes, QAbstractItemModelPrivate& dd, QObject *parent) :
        QAbstractItemModel(dd, parent)
{
    attributes_ = attributes;
}

QVariant AttributesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return tr("Name");

            case 1:
                return tr("Value");

            default:
                return QVariant();
        }
    }

    return QVariant();
}

QVariant AttributesModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch(role)
    {
        case Qt::UserRole:
            return attributes_.names()[index.row()];

        case Qt::UserRole+1:
            return attributes_.values()[index.row()];

        default:
            return QVariant();
    }
}

void AttributesModel::load(const XmlAttributes& attributes)
{
    beginResetModel();

    attributes_ = attributes;

    endResetModel();
}

void AttributesModel::reset()
{
    beginResetModel();

    attributes_ = XmlAttributes();

    endResetModel();
}


int AttributesModel::rowCount(const QModelIndex&) const
{
  return attributes_.names().count();
}

int AttributesModel::columnCount(const QModelIndex&) const
{
  return 2;
}

QModelIndex AttributesModel::index(int row, int column, const QModelIndex& parent) const
{
  if(!hasIndex(row, column, parent))
    return QModelIndex();
  return createIndex(row, column, 0);
}

QModelIndex AttributesModel::parent(const QModelIndex&) const
{
  return QModelIndex();
}
