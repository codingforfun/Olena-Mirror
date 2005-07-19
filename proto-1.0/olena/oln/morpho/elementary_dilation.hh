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

#ifndef OLENA_MORPHO_ELEMENTARY_DILATION_HH
# define OLENA_MORPHO_ELEMENTARY_DILATION_HH

# include <mlc/is_a.hh>

# include <oln/basics.hh>
# include <oln/utils/record.hh>
# include <oln/morpho/local.hh>


namespace oln {

  namespace morpho {


    // Fwd decl of elementary dilation facade.

    template <typename I>
    oln_type_of(I, concrete) elementary_dilation(const abstract::image<I>& input);



    namespace impl {


      // On sets.
      // --------

      // Generic.

      template <typename I>
      oln_type_of(I, concrete)
      elementary_dilation_(const abstract::binary_image<I>& input)
      {
	entering("->generic_on_set");
	registering(input, "input");

	oln_type_of(I, concrete) output(input.size(), "output");

        oln_type_of(I, piter) p(input.size());
        for_all_p (p)
	  output[p] = local_or_value(input, p);

	exiting("->generic_on_set");
	return output;
      }


      // On functions.
      // -------------

      // Generic.

      template <typename I>
      oln_type_of(I, concrete)
      elementary_dilation_(const abstract::not_binary_image<I>& input)
      {
	entering("->generic_on_function");
	registering(input, "input");

	oln_type_of(I, concrete) output(input.size(), "output");

        oln_type_of(I, piter) p(input.size());
        for_all_p (p)
	  output[p] = local_sup_value(input, p);

	exiting("->generic_on_function");
	return output;
      }


    } // end of namespace oln::morpho::impl


    // Elementary dilation (facade).

    template <typename I>
    oln_type_of(I, concrete) elementary_dilation(const abstract::image<I>& input)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();

      entering("morpho::elementary_dilation");
      registering(input, "input");

      oln_type_of(I, concrete) output("output");
      output = impl::elementary_dilation_(input.exact());

      exiting("morpho::elementary_dilation");
      return output;
    }


  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_ELEMENTARY_DILATION_HH
