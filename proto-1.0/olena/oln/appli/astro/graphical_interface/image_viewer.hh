// Copyright (C) 2005  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef _IMAGE_VIEWER_HH_
# define _IMAGE_VIEWER_HH_
# include "qtincludes.hh"

class image_viewer : public QWidget
{

public:

  QImage  img_;
  int loaded;
  int h_;
  int w_;
  double coeff_;

  image_viewer()
  {
    loaded = 0;
  }

  ~image_viewer()
  {
    close();
  }

  void load(const char *fname)
  {
    loaded = 1;
    if (!img_.load(fname))
      {
      setCaption((std::string("LOAD FAILED ") + fname).c_str());
      resize(width(), 200);
      resize(height(), 200);
      loaded = 0;
      return;
      }
    w_ = img_.width();
    h_ = img_.height();
    coeff_ = 1;
    resize_window(w_, h_);
  };

  void reload(const char *fname)
  {
    if (!loaded)
      load(fname);
    else
      {
	double scoeff = coeff_;
	QWMatrix m;
	load(fname);
	coeff_ = scoeff;
	m.scale(coeff_, coeff_);
	img_ = img_.xForm(m);
    }
    resize_window(img_.width(), img_.height());
  }

  void resize_window(int w, int h)
  {
    resize(w, h);
    setMinimumSize(w, h);
    setMaximumSize(w, h);
  }

  void resize_image(int scale_type)
  {
    QWMatrix m;
    if (scale_type < 0)
      {
	m.scale(0.5, 0.5);
	coeff_ *= 0.5;
      }
    else if (scale_type > 0)
      {
	m.scale(2, 2);
	coeff_ *= 2;
      }
    else
      {
	m.scale(coeff_, coeff_);
	coeff_ = 1;
      }
    img_ = img_.xForm(m);
    resize_window(img_.width(), img_.height());
  }


protected:  
  
  void closeEvent(QCloseEvent *e)
  {
    hide();
  };
  
  void paintEvent(QPaintEvent *pe)
  {
    if (!loaded) return;
    bitBlt(this, 0, 0, &img_, 0, 0, img_.width(), img_.height(), CopyROP);
  };

  void mouseReleaseEvent(QMouseEvent * e)
  {
    switch(e->button())
      {
      case LeftButton:
	resize_image(-1);
	break;
      case RightButton:
	resize_image(1);
	break;
      case MidButton:
	resize_image(0);
	break;
      }
  }

};

#endif // !_IMAGE_VIEWER_HH_
