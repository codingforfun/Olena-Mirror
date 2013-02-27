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

#ifndef PREPROCESSINGOPTIONS_HH
# define PREPROCESSINGOPTIONS_HH

#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>

#include "optionwidget.hh"
#include "region.hh"
#include "configs.hh"

namespace Ui
{
    class PreprocessingOptions;
}

class PreprocessingOptions :
        public OptionWidget
{
        Q_OBJECT

    public:
        explicit PreprocessingOptions(QWidget *parent = 0);
        ~PreprocessingOptions();

        void loadConfig();
        void saveConfig();

    private:
        Ui::PreprocessingOptions *ui;

};

#endif // PREPROCESSINGOPTIONS_HH
