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

#ifndef SEGMENTATIONOPTIONS_HH
# define SEGMENTATIONOPTIONS_HH


#include "optionwidget.hh"
#include "configs.hh"
#include "region.hh"

namespace Ui
{
    class SegmentationOptions;
}

class SegmentationOptions :
        public OptionWidget
{
        Q_OBJECT

    public:
        explicit SegmentationOptions(QWidget *parent = 0);
        ~SegmentationOptions();

        void loadConfig();
        void saveConfig();

    private:
        Ui::SegmentationOptions *ui;
};

#endif // SEGMENTATIONOPTIONS_HH
