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

#ifndef GENERALOPTIONS_HH
# define GENERALOPTIONS_HH

#include <QFileDialog>

#include "optionwidget.hh"
#include "configs.hh"

namespace Ui
{
    class GeneralOptions;
}

class GeneralOptions :
        public OptionWidget
{
        Q_OBJECT

    public:
        explicit GeneralOptions(QWidget *parent = 0);
        ~GeneralOptions();

        virtual void loadConfig();
        virtual void saveConfig();

    private:
        Ui::GeneralOptions *ui;

    private slots:
        void onCustomDirBrowseBtnClicked();

};

#endif // GENERAL_OPTIONS_HH
