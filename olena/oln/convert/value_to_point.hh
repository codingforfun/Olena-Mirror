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

#ifndef OLENA_VALUE_TO_POINT
# define OLENA_VALUE_TO_POINT
# include <oln/core/image1d.hh>
# include <oln/core/image2d.hh>
# include <oln/core/image3d.hh>
# include <ntg/int.hh>
# include <ntg/color/color.hh>
namespace oln {

  template <typename T>
  struct value_to_point
  {
    typedef point1d result_type;
    typedef T input_type;
    typedef image1d_size size_type;

    result_type
    operator()(const input_type &input) const
    {
      result_type r(input - ntg_min_val(input_type));
      return r;
    }

    size_type
    size() const
    {
      return size_type(ntg_max_val(input_type) - ntg_min_val(input_type) + 1,
		       1);
    }
  protected:
    typedef typename ntg_is_a(input_type, ntg::non_vectorial)::ensure_type ensure_type;
  };

  //FIXME: the number of components is not a parameter (due to size_type)
  template <unsigned Qbits,template <unsigned> class S>
  struct value_to_point<typename ntg::color<3, Qbits, S> >
  {
    enum {nbcomps = 3, qbits = Qbits};

    typedef ntg::color<nbcomps, qbits, S> input_type;
    typedef typename dim_traits<nbcomps, unsigned>::img_type::size_type
    size_type;
    typedef typename input_type::comp_type comp_type;
    typedef point3d result_type;

    result_type
    operator()(const input_type &input) const
    {
      result_type r;
      for (unsigned i = 0; i < nbcomps; ++i)
	r.nth(i) = input[i];
      return r;
    }

    size_type
    size() const
    {
      return size_type(ntg_max_val(comp_type) - ntg_min_val(comp_type) + 1,
		       ntg_max_val(comp_type) - ntg_min_val(comp_type) + 1,
		       ntg_max_val(comp_type) - ntg_min_val(comp_type) + 1,
		       1);
    }
  };
}
#endif
