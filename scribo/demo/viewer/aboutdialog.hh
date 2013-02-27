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

#ifndef ABOUTDIALOG_HH
# define ABOUTDIALOG_HH

#include <QDialog>

#include "aboutdialog.ui.h"

namespace Ui
{
    class AboutDialog;
}

class AboutDialog :
        public QDialog
{
public:
  explicit AboutDialog(QWidget *parent = 0);
  ~AboutDialog();

private:
  Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_HH
