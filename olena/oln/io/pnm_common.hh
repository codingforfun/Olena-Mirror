// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_IO_PNM_COMMON_HH
# define OLENA_IO_PNM_COMMON_HH

# include <mlc/bool.hh>

# include <ntg/basics.hh>

# include <oln/core/coord.hh>
# include <oln/core/image.hh>

namespace oln {

  namespace io {

    namespace internal {

      enum pnm_type { PnmInvalid   = 0,
		      PnmBinary    = 1,
		      PnmInteger   = 2,
		      PnmVectorial = 3 };
      
      struct pnm2d_info
      {
	coord cols;
	coord rows;
	int max_val;
      };

      /*------------------------.
      | get_pnm_type from image |
      `------------------------*/

      template <pnm_type V>
      struct get_it
      {
	static const pnm_type ret = V;
      };

      // Return the pnm type associated to an image type
      // FIXME: this could be done by using labels images
      // eg: read(binary_image_with_dim<2>& ima) { // ... }
      
      /*! \class get_pnm_type
      **
      ** A metaswitch that return the pnm type associated to an image type.
      **
      ** \todo FIXME: this could be done by using labels images
      ** eg: read(binary_image_with_dim<2>& ima) { // ... }
      */
      template <class I>
      struct get_pnm_type
      {
	typedef typename mlc::bool_switch_<
	  mlc::bool_case_<ntg_is_a(oln_value_type(I), ntg::unsigned_integer)::ret,
			  get_it<PnmInteger>,

	  mlc::bool_case_<ntg_is_a(oln_value_type(I), ntg::binary)::ret, 
			  get_it<PnmBinary>,

	  mlc::bool_case_<ntg_is_a(oln_value_type(I), ntg::vectorial)::ret,
			  get_it<PnmVectorial>,

	  mlc::bool_case_<true,
			  get_it<PnmInvalid>
	> > > > >::ret tmp_type;

	static const pnm_type ret = tmp_type::ret;
      };

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_PNM_COMMON_HH
