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

#ifndef PAGESWIDGET_HH
# define PAGESWIDGET_HH

#include <QListView>
#include <QShortcut>

#include "pagesdelegate.hh"
#include "pagesmodel.hh"

class PagesWidget : public QListView
{
  Q_OBJECT;

public:
  explicit PagesWidget(QWidget *parent = 0);

  QStringList filenames() const;
  int currentRow() const;
  void setCurrentRow(int currentRow);

  void addPicture(const QString& filename, const QPixmap& pixmap);

  bool haveNextRow() const;
  bool havePreviousRow() const;

protected:
  void focusOutEvent(QFocusEvent *event);

private:
  PagesModel model_;

public slots:
  void removeSelection();
  void selectNextRow();
  void selectPreviousRow();

private slots:
  void getPicture(const QModelIndex& index);

signals:
  void imageSelected(const QString& filename);
};


#endif // PAGESWIDGET_HH
