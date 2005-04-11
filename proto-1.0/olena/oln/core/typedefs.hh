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

#ifndef OLENA_CORE_TYPEDEFS_HH
# define OLENA_CORE_TYPEDEFS_HH

// FIXME: rename this present file (?)


# include <mlc/types.hh>
# include <mlc/typedef.hh>
# include <mlc/properties.hh>
# include <mlc/to_string.hh>




namespace oln
{

  mlc_equip_namespace_with_properties();

  // category::image

  mlc_decl_typedef(data_type);
  mlc_decl_typedef(value_type);
  mlc_decl_typedef(neighb_type);
  mlc_decl_typedef(value_storage_type);
  mlc_decl_typedef(storage_type);
  mlc_decl_typedef(point_type);
  mlc_decl_typedef(dpoint_type);
  mlc_decl_typedef(image_type);
  mlc_decl_typedef(concrete_type);

  mlc_decl_typedef(piter_type);
  mlc_decl_typedef(fwd_piter_type);
  mlc_decl_typedef(bkd_piter_type);
  mlc_decl_typedef(iter_type);

  mlc_decl_typedef(delegated_type);
  mlc_decl_typedef(size_type);
  mlc_decl_typedef(se_type);
  mlc_decl_typedef(window_type);

  mlc_decl_typedef(image_neighbness_type);
  mlc_decl_typedef(image_constness_type);
  mlc_decl_typedef(image_dimension_type);
  mlc_decl_typedef(image_typeness_type);
  mlc_decl_typedef(image_valuedness_type);

  // extension in image_operators
  mlc_decl_typedef(output_type);
  mlc_decl_typedef(input_type);
  mlc_decl_typedef(input1_type);
  mlc_decl_typedef(input2_type);

  // category::window

  mlc_decl_typedef(fwd_qiter_type);
  mlc_decl_typedef(bkd_qiter_type);



} // end of namespace oln



// FIXME: memo...

//   namespace category
//   {
//     struct data_storage;
//     struct struct_elt;
//     struct neighborhood;
//     struct point;
//     struct size;
//     struct piter;
//     struct niter;
//     // FIXME: ...
//   }


#endif // ! OLENA_CORE_TYPEDEFS_HH
