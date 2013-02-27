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

#ifndef PREFERENCESDIALOG_HH
# define PREFERENCESDIALOG_HH

#include <QDialog>

#include "preprocessingoptions.hh"
#include "segmentationoptions.hh"
#include "generaloptions.hh"
#include "ocroptions.hh"

namespace Ui
{
    class PreferencesDialog;
}

class PreferencesDialog :
        public QDialog
{
        Q_OBJECT

    public:
        explicit PreferencesDialog(QWidget *parent = 0);
        ~PreferencesDialog();

    private:
        void loadOptionList();
        void selectOptionWidget(int row);

        Ui::PreferencesDialog *ui;
        QVector<QWidget *> widgets;

    private slots:
        void onOptionListCurrentRowChanged(int row);
        virtual void accept();
        virtual void reject();

};

#endif // PREFERENCESDIALOG_HH
