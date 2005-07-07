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

#ifndef OLENA_LEVEL_INVERT_HH
# define OLENA_LEVEL_INVERT_HH

# include <oln/funobj/invert.hh>

namespace oln
{

  namespace level
  {

    /// Invert the values of an image.
    template <typename I>
    oln_type_of(I, concrete) invert (const abstract::image<I>& input)
    {
      entering("level::invert");
      registering(input, "input");

      // FIXME: funobj::invert should be turned into a f_::invert
      // functor, and we should use apply() here.
      funobj::invert f_invert;
      oln_type_of(I, concrete) output(input.size(), "output");
      oln_type_of(I, piter) p(input.size());
      for_all_p (p)
	output[p] = f_invert(input[p]);

      exiting("level::invert");
      return output;
    }

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLENA_LEVEL_INVERT_HH
