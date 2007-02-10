// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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


#ifndef OLENA_GEOM_TRANSLATE_HH
# define OLENA_GEOM_TRANSLATE_HH

# include <cmath>
# include <iostream>

# include <oln/core/image2d.hh>
# include <oln/core/fpoint2d.hh>
# include <oln/core/fdpoint2d.hh>

namespace oln {

  namespace geom {

    template <class T, class F, class Exact>
    struct translate
    {
      typedef oln::image2d<T> im_type;


      image2d<T>
      operator()(const oln::image2d<T>& ima,
		 const fdpoint2d<F> dp,
		 fpoint2d_access<T, Exact> interp)
      {
	im_type res(ima.size());
	oln_iter_type(im_type) it(ima);

	for_all(it)
	{
	  res[it] = interp(ima, fpoint2d<F>(it.row(), it.col()) - dp);
	}
	return res;
      }
    };
  }
}





#endif // !OLENA_GEOM_TRANSLATE_HH

