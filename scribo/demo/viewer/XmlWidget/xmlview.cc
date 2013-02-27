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

#include "xmlview.hh"

XmlView::XmlView(QWidget *parent) :
        QTreeView(parent)
{
    proxy_.setFilterRole(Qt::UserRole);
    proxy_.setDynamicSortFilter(true);

    setUniformRowHeights(true);
    setItemDelegate(new XmlDelegate(Qt::UserRole));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void XmlView::hideAll()
{
    proxy_.beginResetModel();

    XmlItem *child;
    foreach(child, displayedItems_)
        child->hide();

    displayedItems_.clear();
    emit resetProperty();

    proxy_.endResetModel();
}

void XmlView::display(QList<XmlItem *> displayedItems, bool addToDisplayedList)
{
    proxy_.beginResetModel();

    if(!addToDisplayedList)
        hideAll();

    XmlItem *child;
    foreach(child, displayedItems)
    {
        child->show();
        displayedItems_ << child;
    }

    proxy_.endResetModel();

    expandAll();
}

QList<XmlItem *> XmlView::transform(const QModelIndexList& modelIndexList) const
{
    QList<XmlItem *> selectedItems;
    QModelIndex index;

    foreach(index, modelIndexList)
        selectedItems << index.data(Qt::UserRole+2).value<XmlItem *>();

    return selectedItems;
}

void XmlView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    QTreeView::selectionChanged(selected, deselected);

    if(selected.indexes() == selectedIndexes())
        emit resetSelection();

    if(!selectedIndexes().isEmpty())
    {
        QList<XmlItem *> xmlItems;
        if(!selected.isEmpty())
        {
            xmlItems = transform(selected.indexes());
            emit select(xmlItems);
        }
        else
        {
            xmlItems = transform(deselected.indexes());
            emit unselect(xmlItems);
        }

        emit loadAttributes(xmlItems.last()->attributes());
    }
    else
    {
        emit resetProperty();
        emit emptySelection();
    }

    emit newSelection();
}
