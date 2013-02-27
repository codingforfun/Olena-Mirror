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

#ifndef PREPROCESS_H
# define PREPROCESS_H

#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <QObject>

using namespace scribo::toolchain::internal;

class Preprocess :
        public QObject,
        public text_in_doc_preprocess_functor<mln::image2d<mln::value::rgb8> >
{
        Q_OBJECT

    public:
        virtual void on_progress();
        virtual void on_new_progress_label(const char *label);

    signals:
        void newProgressLabel(const QString& label);
        void progress();
};

#endif // PREPROCESS_H
