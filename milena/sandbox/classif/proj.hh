// Copyright (C) 2008 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef MLN_PROJ_HH
# define MLN_PROJ_HH

#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/level/paste.hh>
#include <mln/accu/mean.hh>

namespace mln
{

  template <typename T, typename A>
  image2d<mln_result(A)>
  proj(const image3d<T>& input, A a)
  {
    image2d<A> acc(geom::nslis(input), geom::nrows(input));
    mln_piter(image3d<T>) p(input.domain());
    for_all(p) // 3d
      acc.at(p.sli(), p.row()).take(input(p));

    image2d<mln_result(A)> output(acc.domain());
    level::paste(acc, output);
    return output;
  }

  template <typename T>
  void
  save_class(const image3d<T>& histo, const char * fn)
  {
    accu::mean<unsigned, unsigned long, value::int_u8> mean;
    io::pgm::save(proj(histo, mean), fn);
  }

} // end of namespace mln

#endif /* MLN_PROJ_HH */

