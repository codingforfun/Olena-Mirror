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

#ifndef PROCESS_H
# define PROCESS_H

#include <scribo/toolchain/internal/content_in_doc_functor.hh>
#include <mln/core/image/image2d.hh>
#include <QObject>

using namespace scribo::toolchain::internal;

struct Process :
        public QObject,
        public content_in_doc_functor<mln::image2d<bool> >
{
        Q_OBJECT
        typedef content_in_doc_functor<mln::image2d<bool> > superT;

    public :
        explicit Process(const char *docFilename);
        virtual void on_progress();
        virtual void on_new_progress_label(const char *label);
        virtual void on_xml_saved();

    signals:
        void newProgressLabel(const QString& label);
        void progress();
        void xmlSaved(const QString& filename);
};

#endif // PROCESS_H
