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

#ifndef OLENA_CORE_PROPERTIES_HH
# define OLENA_CORE_PROPERTIES_HH

# include <mlc/properties.hh>


/*! \namespace oln
** \brief oln namespace.
*/
namespace oln
{

  namespace category
  {
    struct data_storage;
    struct image;
    struct struct_elt;
    struct neighborhood;
    struct point;
    struct size;
    struct piter;
    struct witer;
    struct niter;
    // FIXME: ...
  }

  namespace target
  {
    struct data_type;
    struct value_type;
    struct neighb_type;
    struct value_storage_type;
    struct storage_type;
    struct point_type;
    struct image_type;
    struct concrete_type;
    struct dpoint_type;
    struct piter_type;
    struct fwd_piter_type;
    struct bkd_piter_type;
    struct fwd_witer_type;
    struct iter_type;
    struct delegated_type;
    struct size_type;
    struct se_type;
    struct window_type;

    struct image_neighbness_type;
    struct image_constness_type;
    struct image_dimension_type;
    struct image_typeness_type;
    struct image_valuedness_type;
    // FIXME: ...

    struct dummy_type; // FIXME: remove (test purpose!)
  }

  mlc_equip_namespace_for_properties();


  // FIXME: change name?
  template <template <typename> class abstraction>
  struct is_a // abstraction
  {
    template <typename E>
    struct instantiated_with // E
    {
      typedef abstraction<E> ret;
    };
  };


} // end of namespace oln



/// macro to retrieve a type (TARGET) from a class (oln::TYPE)

# define oln_type_of(TYPE, TARGET) mlc_type_of(oln, TYPE, TARGET)
# define oln_type_of_(TYPE, TARGET) mlc_type_of_(oln, TYPE, TARGET)



#endif // ! OLENA_CORE_PROPERTIES_HH
