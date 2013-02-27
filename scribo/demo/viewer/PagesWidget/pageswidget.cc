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

#include <QDebug>
#include "pageswidget.hh"

PagesWidget::PagesWidget(QWidget *parent):
        QListView(parent)
{
    setUniformItemSizes(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(200, 200));
    setMovement(QListView::Static);
    setResizeMode(QListView::Adjust);
    setSpacing(10);

    setModel(&model_);
    setItemDelegate(new PagesDelegate());

    QShortcut *remove = new QShortcut(Qt::Key_Delete, this);
    connect(remove, SIGNAL(activated()), this, SLOT(removeSelection()));
    connect(this, SIGNAL(doubleClicked(QModelIndex)),
	    this, SLOT(getPicture(QModelIndex)));
}

void PagesWidget::removeSelection()
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();
    if(hasFocus() && indexes.count() > 0)
    {
        // Sort the list because after the first removal, the second row index may not be the good one.
        qSort(indexes.begin(), indexes.end());

        // From the highest row to the smallest to be sure to remove the good index.
        for(int i = indexes.count()-1; i > -1; i--)
            model_.removePixmap(indexes[i]);

        // Draw new image on the scene.
        getPicture(model_.index(indexes[0].row()));
        clearSelection();

        model_.setCurrentRow(indexes[0].row());
        scrollTo(indexes[0]);
    }
}

void PagesWidget::getPicture(const QModelIndex& index)
{
    QString filename = index.data(Qt::UserRole).toString();
    model_.setCurrentRow(index.row());

    emit imageSelected(filename);
}

QStringList PagesWidget::filenames() const
{
  return model_.filenames();
}

int PagesWidget::currentRow() const
{
  return model_.currentRow();
}

void PagesWidget::setCurrentRow(int currentRow)
{
  model_.setCurrentRow(currentRow);
}

void PagesWidget::addPicture(const QString& filename,
				    const QPixmap& pixmap)
{
  model_.addPicture(filename, pixmap);
}

void PagesWidget::focusOutEvent(QFocusEvent *event)
{
  QListView::focusOutEvent(event); clearSelection();
}

bool PagesWidget::haveNextRow() const
{
  return currentRow() + 1 < model_.rowCount();
}

bool PagesWidget::havePreviousRow() const
{
  return currentRow() - 1 >= 0;
}

void PagesWidget::selectNextRow()
{
  Q_ASSERT(haveNextRow());
  QModelIndex index = model_.index(currentRow() + 1);
  getPicture(index);
  scrollTo(index);
}

void PagesWidget::selectPreviousRow()
{
  Q_ASSERT(havePreviousRow());
  QModelIndex index = model_.index(currentRow() - 1);
  getPicture(index);
  scrollTo(index);
}
