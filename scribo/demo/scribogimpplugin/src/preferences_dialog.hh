// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_DEMO_VIEWER_PREFERENCES_DIALOG_HH
# define SCRIBO_DEMO_VIEWER_PREFERENCES_DIALOG_HH

extern "C"
{
#include <libgimp/gimp.h>
}


# include <QtGui>
# include <preferences_dialog.ui.h>

# include <runner.hh>

# include <mln/core/image/image2d.hh>
# include <scribo/core/def/lbl_type.hh>


class preferences_dialog : public QDialog, private Ui::PreferencesDialog
{
  Q_OBJECT;

  typedef mln::image2d<scribo::def::lbl_type> L;

public:
  preferences_dialog(QWidget *parent = 0);
  ~preferences_dialog();

  void set_current_image(const mln::image2d<mln::value::rgb8>& ima, gint32 image_id);

private slots:
  void on_optionList_currentRowChanged(int row);
  virtual void accept();
  virtual void reject();
  void diplay_result();

private: // Methods
  void load_option_list();
  void select_option_widget(int row);
  void reset_progress_dialog();
  void process();

private: // Attributes
  QVector<QWidget *> widgets_;

  runner runner_;
  QProgressDialog pdialog_;

  mln::image2d<mln::value::rgb8> current_image_;
  gint32 image_id_;
};

#endif // ! SCRIBO_DEMO_VIEWER_PREFERENCES_DIALOG_HH
