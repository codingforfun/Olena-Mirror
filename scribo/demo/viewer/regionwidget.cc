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

#include "regionwidget.hh"

RegionWidget::RegionWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setHeaderHidden(true);
    setSelectionMode(QAbstractItemView::NoSelection);

    addTopLevelItem(createRoot("Text", GraphicsRegion::Text,
			       GraphicsRegion::TextRegion,
			       GraphicsRegion::Line));
    addTopLevelItem(createRoot("Typological Lines", GraphicsRegion::Typology,
			       GraphicsRegion::Baseline,
			       GraphicsRegion::Meanline));
    addTopLevelItem(createRoot("Separators", GraphicsRegion::Separators,
			       GraphicsRegion::VerticalSeparator,
			       GraphicsRegion::WhiteSpaceSeparator));
    addTopLevelItem(createRoot("Miscellaneous", GraphicsRegion::Miscellaneous,
			       GraphicsRegion::Image, GraphicsRegion::Chart));

    expandAll();

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
	    this, SLOT(checkStateChanged(QTreeWidgetItem*)));
}

QTreeWidgetItem *RegionWidget::createRoot(const QString& text,
					  const GraphicsRegion::Id& region,
					  const GraphicsRegion::Id& begin,
					  const GraphicsRegion::Id& end)
{
    // Create root.
    QTreeWidgetItem *rootItem = createItem(text, region);

    // Fill root with corresponding childs.
    for(int region_ = begin; region_ <= end; region_++)
        fillRoot(rootItem, static_cast<GraphicsRegion::Id>(region_));

    return rootItem;
}

QTreeWidgetItem *RegionWidget::createItem(const QString& text,
					  const GraphicsRegion::Id& region,
					  const QColor& color)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, text);
    item->setCheckState(0, Qt::Checked);
    item->setBackgroundColor(0, color);
    // Store graphical id in the object to recognize it afterward.
    item->setData(0, Qt::UserRole, static_cast<int>(region));

    return item;
}

void RegionWidget::checkStateChanged(QTreeWidgetItem *item)
{
    // If it's a root item, go to childs.
    if(item->childCount() != 0)
    {
        QTreeWidgetItem *child;
        for(int i = 0; i < item->childCount(); i++)
        {
            child = item->child(i);
            child->setCheckState(0, item->checkState(0));
        }
    }
    else
    {
        if(item->checkState(0) == Qt::Checked)
        {
            filterString_.append('|' + item->text(0));
            emit checkStateChanged(static_cast<GraphicsRegion::Id>(item->data(0, Qt::UserRole).toInt()), true);
        }
        else
        {
            filterString_.remove('|' + item->text(0), Qt::CaseSensitive);
            emit checkStateChanged(static_cast<GraphicsRegion::Id>(item->data(0, Qt::UserRole).toInt()), false);
        }

        //emit checkStateChanged(filterString_);
    }
}


inline void RegionWidget::fillRoot(QTreeWidgetItem *rootItem,
				   const GraphicsRegion::Id& region)
{
  GraphicsRegion::Data data = Xml::dataFromRegion(region);
  filterString_.append('|' + data.name);
  rootItem->addChild(createItem(data.name, region, data.color));
}


inline QString RegionWidget::filterString() const
{
  return filterString_.right(filterString_.count()-1);
}
