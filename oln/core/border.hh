// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_BORDER_HH
# define OLENA_CORE_BORDER_HH

# include <oln/core/contract.hh>
# include <oln/core/image.hh>
# include <oln/core/type.hh>
# include <iostream>
# include <string.h>

namespace oln {

  namespace border {

    namespace internal {}

    template<class _T>
    void set_width(const image<_T>& _input, coord new_border,
		   bool copy_border = false)
    {
      Exact_cref(T, input);

      precondition(new_border >= 0);
      precondition(input.data() != 0);
      if (input.border() == new_border)
	return;			// Nothing to do.

      internal::_reallocate_and_copy(input, new_border, copy_border);
    }


    /* Extend the border if needed, don't shrink it.  */
    template<class _T>
    void adapt_width(const image<_T>& _input, coord min_border,
		     bool copy_border = false)
    {
      Exact_cref(T, input);

      precondition(min_border >= 0);
      if (input.border() >= min_border)
	return;			// Don't shrink.

      set_width(input, min_border, copy_border);
    }

    template<class _T>
    void adapt_copy(const image<_T>& _input, coord min_border)
    {
      Exact_cref(T, input);
      adapt_width(input, min_border);
      internal::_replicate_borders(input);
    }


    template<class _T>
    void adapt_mirror(const image<_T>& _input, coord min_border)
    {
      Exact_cref(T, input);
      adapt_width(input, min_border);
      internal::_mirror_borders(input);
    }


    template<class _T>
    void adapt_assign(const image<_T>& _input, coord min_border, Value(_T) val)
    {
      Exact_cref(T, input);
      adapt_width(input, min_border);
      internal::_assign_borders(input, val);
    }

  } // end of border

} // end of oln


#endif // ! OLENA_CORE_BORDER_HH
