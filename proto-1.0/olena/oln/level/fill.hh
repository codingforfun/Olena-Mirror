// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_LEVEL_FILL_HH
# define OLENA_LEVEL_FILL_HH

# include <oln/basics.hh>

namespace oln {

  namespace level {

    // FIXME: AWFUL in place!!!

    /*! \brief Fill the image with a value.
     */
    template <class I>
    I
    fill(abstract::image<I>& input, const oln_type_of(I, value)& val)
    {
      oln_type_of(I, fwd_piter) p(input.size());
      for_all_p (p)
	input[p] = val;
      return input.exact();
    }

  } // end of namespace level

} // end of namespace oln

#endif // ! OLENA_LEVEL_FILL_HH
