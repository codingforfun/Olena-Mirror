// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLN_UTILS_CLONE_HH
# define OLN_UTILS_CLONE_HH

# include <oln/core/abstract/image.hh>
# include <oln/utils/record.hh>


namespace oln
{

  // Fwd decl of clone's facade.

  template <typename I>
  oln_type_of(I, concrete) clone(const abstract::image<I>& input);


  namespace impl {

    // generic

    template <typename I>
    oln_type_of(I, concrete) clone_(const abstract::image<I>& input)
    {
      entering("->generic");
      registering(input, "input");

      oln_type_of(I, concrete) output(input.size(), "output");
      oln_type_of(I, fwd_piter) p(input.size());
      for_all_p (p)
	output[p] = input[p];

      exiting("->generic");
      return output;
    }

    // add some other impls here...

  } // end of namespace oln::morpho::impl


  /// Generic clone (facade).

  template <typename I>
  oln_type_of(I, concrete) clone(const abstract::image<I>& input)
  {
    entering("clone");
    registering(input, "input");

    oln_type_of(I, concrete) output("output");
    output = impl::clone_(input.exact());

    exiting("clone");
    return output;
  }


} // end of namespace oln


#endif // ! OLN_UTILS_CLONE_HH
