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

#ifndef XMLWIDGET_H
# define XMLWIDGET_H

#include <QSortFilterProxyModel>
#include <QTableView>
#include <QTreeWidget>
#include <QSplitter>
#include <QDomElement>
#include <QWidget>

#include "Rendering/scene.hh"
#include "selectionproxy.hh"
#include "attributesmodel.hh"
#include "variantpointer.hh"
#include "xmldelegate.hh"
#include "xmlmodel.hh"
#include "xmlview.hh"

class XmlWidget :
        public QSplitter
{
        Q_OBJECT

    public:
        explicit XmlWidget(QWidget *parent = 0);

        XmlView *view();
        QTableView *attributes();

        void changeView(XmlItem *rootItem);

    private:
        QSortFilterProxyModel proxy_;

        XmlModel model_;
        AttributesModel attributesModel_;

        XmlView view_;
        QTableView attributes_;

    private slots:
        void loadAttributes(const QModelIndex& index);
};

#endif // XMLWIDGET_H
