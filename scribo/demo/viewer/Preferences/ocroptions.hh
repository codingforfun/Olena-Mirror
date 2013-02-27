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

#ifndef OCR_OPTIONS_HH
# define OCR_OPTIONS_HH

//#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>

#include "optionwidget.hh"
#include "configs.hh"
#include "region.hh"

namespace Ui
{
    class OcrOptions;
}

class OcrOptions :
        public OptionWidget
{
        Q_OBJECT

    public:
        explicit OcrOptions(QWidget *parent = 0);
        ~OcrOptions();

        void loadConfig();
        void saveConfig();

    private:
        int findIndex(const QString& lang);

        Ui::OcrOptions *ui;
};

#endif // OCR_OPTIONS_HH
