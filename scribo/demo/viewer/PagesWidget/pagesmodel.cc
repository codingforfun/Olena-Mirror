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

#include "pagesmodel.hh"

PagesModel::PagesModel(QObject *parent):
        QAbstractListModel(parent)
{
    currentRow_ = 0;
}

QVariant PagesModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DecorationRole)
        return pixmaps_[index.row()];

    if(role == Qt::UserRole)
        return filenames_[index.row()];

    if(role == Qt::UserRole+1)
        return currentRow_;

   return QVariant();
}

void PagesModel::addPicture(const QString& filename, const QPixmap& pixmap)
{
    beginInsertRows(QModelIndex(), 0, 0);

    pixmaps_.prepend(pixmap.scaled(QSize(200, 200),
				   Qt::KeepAspectRatio,
				   Qt::SmoothTransformation));
    filenames_.prepend(filename);

    endInsertRows();
}

void PagesModel::removePixmap(const QModelIndex &parent)
{
    int row = parent.row();

    beginRemoveRows(parent, row, row);

    pixmaps_.removeAt(row);
    filenames_.removeAt(row);

    endRemoveRows();
}

void PagesModel::setCurrentRow(int currentRow)
{
    beginResetModel();

    currentRow_ = currentRow;

    endResetModel();
}


QStringList PagesModel::filenames() const
{
  return filenames_;
}


int PagesModel::rowCount(const QModelIndex&) const
{
  return filenames_.count();
}


int PagesModel::currentRow() const
{
  return currentRow_;
}
