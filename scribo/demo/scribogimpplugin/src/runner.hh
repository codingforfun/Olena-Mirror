// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_DEMO_VIEWER_RUNNER_HH
# define SCRIBO_DEMO_VIEWER_RUNNER_HH

# include <QtGui>

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/int_u16.hh>
# include <scribo/core/document.hh>
# include <scribo/core/def/lbl_type.hh>

using namespace mln;

enum RunMode
{
  Demat,
  Export
};

class runner : public QThread
{
  Q_OBJECT;

public:
  typedef scribo::document<image2d<scribo::def::lbl_type> > doc_t;

  runner(QObject *parent = 0);

  void start_demat(const image2d<value::rgb8>& filename);

  const doc_t& result() const;

public slots:
  void stop();
  void progress_value();

signals:
  void new_step(const QString& step_name);
  void new_progress_max_value(int i);
  void new_progress_label(const QString& msg);
  void progress(int step);
  void finished();

private: // members
  image2d<bool> preprocess(const image2d<value::rgb8>& ima);
  void process(const image2d<value::rgb8>& original_ima,
	       const image2d<bool>& processed_ima);

  virtual void run();

private: // attributes
  QList<image2d<value::rgb8> > args_;
  RunMode mode_;
  int step_;

  doc_t doc_;
};


#endif // ! SCRIBO_DEMO_VIEWER_RUNNER_HH
