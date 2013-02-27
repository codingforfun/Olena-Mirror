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

#include "xmlwidget.hh"

XmlWidget::XmlWidget(QWidget *parent) :
        QSplitter(Qt::Horizontal, parent)
{
    proxy_.setFilterRegExp(QRegExp("(?:Page|.Region|Line)", Qt::CaseSensitive));
    proxy_.setDynamicSortFilter(true);
    proxy_.setFilterRole(Qt::UserRole);
    proxy_.setSourceModel(&model_);

    view_.setModel(&proxy_);

    attributes_.setModel(&attributesModel_);
    attributes_.setSelectionMode(QAbstractItemView::NoSelection);
    attributes_.setItemDelegateForColumn(0, new XmlDelegate(Qt::UserRole));
    attributes_.setItemDelegateForColumn(1, new XmlDelegate(Qt::UserRole+1));

    addWidget(&view_);
    addWidget(&attributes_);

    connect(&view_, SIGNAL(resetProperty()), &attributesModel_, SLOT(reset()));
    connect(&view_, SIGNAL(clicked(QModelIndex)), this, SLOT(loadAttributes(QModelIndex)));
    connect(&view_, SIGNAL(loadAttributes(XmlAttributes)), &attributesModel_, SLOT(load(XmlAttributes)));
}

void XmlWidget::changeView(XmlItem *rootItem)
{
    attributesModel_.reset();

    model_.load(rootItem);

    view_.displayAll();
    view_.expandAll();
}


XmlView * XmlWidget::view()
{
  return &view_;
}

QTableView *XmlWidget::attributes()
{
 return &attributes_;
}

void XmlWidget::loadAttributes(const QModelIndex& index)
{
  attributesModel_.load(index.data(Qt::UserRole+2).value<XmlItem *>()->attributes());
}
