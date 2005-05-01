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

# include <mlc/cmp.hh>
# include <mlc/is_a.hh>
# include <mlc/implies.hh>

# include <oln/utils/record.hh>
# include <oln/core/gen/image_with_nbh.hh>
# include <oln/morpho/tags.hh>


namespace oln {

  namespace morpho {



    // Fwd decl of elementary dilation's facade.

    template<typename K, typename I>
    oln_type_of(I, concrete) elementary_dilation(const tag::kind<K>& kind,
						 const abstract::image_with_nbh<I>& input);

    // Facade for classical elementary dilation.

    template<typename I>
    oln_type_of(I, concrete) elementary_dilation(const abstract::image_with_nbh<I>& input)
    {
      return elementary_dilation(tag::classical, input);
    }



    namespace impl {


      // generic

      template<typename K, typename I>
      oln_type_of(I, concrete) elementary_dilation_(const tag::kind<K>& kind,
						    const abstract::image_with_nbh<I>& input)
      {
	entering("->generic");
	registering(input, "input");

	oln_type_of(I, concrete) output(input.size(), "output");

        oln_type_of(I, piter) p(input.size());
        for_all_p (p)
	  output[p] = kind.max_nbh(input, p);

	exiting("->generic");
	return output;
      }


      // add some other impls here...

      
    } // end of namespace oln::morpho::impl




    /// Generic elementary_dilation (facade).

    template<typename K, typename I>
    oln_type_of(I, concrete) elementary_dilation(const tag::kind<K>& kind,
						 const abstract::image_with_nbh<I>& input)
    {
      mlc::implies< mlc_is_a(I, abstract::binary_image),
	            mlc_eq(K, tag::classical_type) >::ensure();
      entering("morpho::elementary_dilation");
      registering(input, "input");

      oln_type_of(I, concrete) output("output");
      output = impl::elementary_dilation_(kind, input.exact());

      exiting("morpho::elementary_dilation");
      return output;
    }




  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_ELEMENTARY_DILATION_HH
