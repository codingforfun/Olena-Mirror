// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_CORE_ABSTRACT_HIERARCHY_HH
# define NTG_CORE_ABSTRACT_HIERARCHY_HH

namespace ntg
{

  /*
    Abstract hierarchy is useful to easily check the properties of
    each types. type_traits<T>::abstract_type gives the abstract type
    associated to T. The macro ntg_is_a(T, abstract_type) is a simple
    way to ensure a given type has good properties.

    Examples:
    
    ntg_is_a(int, integer)::ensure();
    mlc_is_a(type_traits<int_u8>::abstract_type, unsigned_integer)::ensure();
  */
  
  //! Top of the hierarchy.
  class data_type {};

  /*------.
  | reals |
  `------*/

  class real : public data_type {};

  // int, unsigned, int_u, int_s, etc.
  class integer : public real {};
  class unsigned_integer : public integer {};
  class signed_integer : public integer {};

  // float_s, float_d, etc.
  class decimal : public real {};

  /*-----------.
  | enumerated |
  `-----------*/

  class enumerated : public data_type {};

  // bin, bool, etc.
  class binary : public enumerated {};

  /*----------.
  | vectorial |
  `----------*/

  // vec, cplx, etc.
  class vectorial : public data_type {};

} // end of ntg.

#endif // !NTG_CORE_ABSTRACT_HIERARCHY_HH
