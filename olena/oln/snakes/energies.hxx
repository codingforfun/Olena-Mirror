// Copyright (C) 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_SNAKES_ENERGIES_HXX
# define OLENA_SNAKES_ENERGIES_HXX

namespace oln {

  namespace snakes {


    template <class I>
    inline
    ntg::float_s
    continuity_energy<I>::compute(const I&,
				  const node<I>& prev,
				  const node<I>& current,
				  const node<I>&)
    {
      ntg::float_s d = *average_dist - (current - prev).norm2();
      return d > 0 ? d : -d;
    }

    template <class I>
    inline
    ntg::float_s
    curvature_energy<I>::compute(const I&,
				 const node<I>& prev,
				 const node<I>& current,
				 const node<I>& next)
    {
      typename I::point_type twice_current;

      twice_current.row() = 2 * current.row();
      twice_current.col() = 2 * current.col();
      return (next + (prev - twice_current) -
	      typename I::point_type(0,0)).norm2();
    }

    template <class I>
    inline
    ntg::float_s
    image_energy<I>::compute(const I& gradient,
			     const node<I>&,
			     const node<I>& current,
			     const node<I>&)
    {
      // FIXME: Add magic trick: if there is very little gradient difference,
      // don't pay too much attention to it.
      // If max_gradient < min_gradient + 5: max_gradient = min_gradient + 5
      return ntg_sup_val(oln_value_type(I)) - gradient[current];
    }

  } // end snakes

} // end oln

#endif // !OLENA_SNAKES_ENERGIES_HXX
