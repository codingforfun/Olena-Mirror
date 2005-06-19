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

#include <oln/basics2d.hh>
#include <ntg/real/int_u.hh>
#include <oln/fancy/iota.hh>
#include <oln/fancy/print.hh>
#include <oln/appli/astro/clean.hh>
#include <oln/io/write_image.hh>
#include <oln/io/read_image.hh>
#include <qapplication.h>
#include "filterinterface.hh"

int main(int argc, char * argv[])
{
  typedef oln::image_with_nbh<oln::image2d<ntg::int_u8>, oln::neighborhood2d> ima_type;

  QApplication app(argc, argv);

  if (argc < 2)
    {
      std::cout << "Usage: " << argv[0] << " <image_file>" << std::endl;
      return 1;
    }

  oln::image2d<ntg::int_u8> ima_raw;
  ima_raw = oln::io::read(argv[1]);
  ima_type ima(ima_raw, oln::neighb_c4());
  oln::appli::astro::clean<ima_type> mt(ima);

  FilterVisualizationWindow win(argv[1], mt);
  app.setMainWidget(&win);
  win.show();
  return app.exec();
}
