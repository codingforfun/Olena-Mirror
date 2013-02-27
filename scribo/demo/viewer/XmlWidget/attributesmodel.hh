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

#ifndef ATTRIBUTESMODEL_HH
# define ATTRIBUTESMODEL_HH

#include <QAbstractItemModel>

#include "xmlattributes.hh"

class AttributesModel :
  public QAbstractItemModel
{
  Q_OBJECT

  public:
  explicit AttributesModel(const XmlAttributes& attributes = XmlAttributes(),
			   QObject *parent = 0);
  explicit AttributesModel(const XmlAttributes& attributes,
			   QAbstractItemModelPrivate& dd,
			   QObject *parent = 0);

  int rowCount(const QModelIndex& parent) const;
  int columnCount(const QModelIndex& parent) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QVariant data(const QModelIndex& index, int role) const;

  QModelIndex index(int row, int column, const QModelIndex &parent) const;
  QModelIndex parent(const QModelIndex &child) const;

public slots:
  void reset();

private:
  XmlAttributes attributes_;

public slots:
  void load(const XmlAttributes& attributes);
};

#endif // ATTRIBUTESMODEL_HH
