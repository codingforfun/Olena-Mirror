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

#ifndef OLENA_FUNOBJ_INVERT_HH
# define OLENA_FUNOBJ_INVERT_HH

# include <mlc/contract.hh>

# include <ntg/all.hh>

namespace oln {

  namespace funobj {

    struct invert
    {
      template <typename V>
      V operator()(const ntg::vect_value<V>& v) const
      {
	V ret;

	for (unsigned i = 0; i < ntg_nb_comp(V); i++)
	  ret[i] = ntg_max_val(ntg_comp_type(V)) - v[i];
	return ret;
      }

      template <typename V>
      V operator()(const ntg::real_value<V>& v) const
      {
	V ret;

	ret = ntg_max_val(V) - v;
	return ret;
      }

      template <typename V>
      V operator()(const ntg::enum_value<V>& v) const
      {
	V ret;
	ntg_integer_type(V) max_val = ntg_max_val(V);
	ntg_integer_type(V) value = v.exact();

	ret = max_val - value;
	return ret;
      }

      invert() {}

    };

  } // end of namespace funobj

} // end of namespace oln


#endif // ! OLENA_FUNOBJ_INVERT_HH


