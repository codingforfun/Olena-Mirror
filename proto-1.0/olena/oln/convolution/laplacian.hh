// Copyright (C) 2001-2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_CONVOLUTION_LAPLACIAN_HH
# define OLENA_CONVOLUTION_LAPLACIAN_HH

# include <mlc/is_a.hh>
# include <oln/basics.hh>


namespace oln {

  namespace convolution {

    // Fwd decl of the laplacian image computation

    template <typename I>
    oln_type_of(I, concrete)
      laplacian(const abstract::image<I>& input);

    namespace impl {


      // FIXME : type I must be a ntg::int_s8 u8...

      template <typename I>
      oln_type_of(I, concrete)
	laplacian_(const abstract::image2d<I>& input)
      {
	oln_type_of(I, concrete) lpl(input.size(), "laplacian second derivative");
	oln_type_of(I, fwd_piter) p(input.size());
	oln_type_of(I, point) pt;
	oln_type_of(I, value) v;

	for_all_p(p)
	  {
	    pt = p;
	    lpl[p] = (input[oln_type_of(I, point)((coord_t)(pt.row() - 1), pt.col())].value() +
		      input[oln_type_of(I, point)((coord_t)(pt.row() + 1), pt.col())].value() +
		      input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() - 1))].value() +
		      input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() + 1))].value() -
		      4 * input[p].value()) / 4;
	  }
	return lpl;
      }


    } // end of namespace oln::convolution::impl

    template <typename I>
    oln_type_of(I, concrete)
      laplacian_(const abstract::image<I>& input)
    {
      return impl::laplacian_(input.exact());
    }


  } // end of namespace oln::convolution

} // end of namespace oln


#endif // ! OLENA_CONVOLUTION_LAPLACIAN_HH
