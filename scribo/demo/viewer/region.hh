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

#ifndef REGION_HH
# define REGION_HH

#include <QColor>

namespace GraphicsRegion
{
    // WARNING : The order is important.
    enum Id
    {
        TextRegion,
        Line,

        VerticalSeparator,
        HorizontalSeparator,
        WhiteSpaceSeparator,

        Image,
        Noise,
        Table,
        Maths,
        Graphic,
        Chart,

        Baseline,
        Meanline,

        Text,
        Separators,
        Miscellaneous,
        Typology,

        None
    };

    struct Data
    {
        QColor color;
        QString name;
        GraphicsRegion::Id region;
        GraphicsRegion::Id parent;
        int zValue;
    };
}

namespace Separator
{
    enum FindSeparator
    {
        Lines,
        Whitespaces,
        Both
    };
}

#endif // REGION_HH
