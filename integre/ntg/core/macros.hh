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

#ifndef NTG_CORE_MACROS_HH
# define NTG_CORE_MACROS_HH

/*------.
| casts |
`------*/

# define ntg_type(T) typename ntg::type_traits<T>::ntg_type
# define ntg_type_(T) typename ntg::type_traits<T>::ntg_type

# define ntg_cast(Val) ntg::to_ntg(Val)
# define ntg_signed_cast(Val) ntg::to_signed_ntg(Val)

/*--------------.
| type checking |
`--------------*/

# define ntg_is_a(T, U) is_a(typename ntg::type_traits<T>::abstract_type, U)

/*----------------.
| types accessors |
`----------------*/

# define ntg_storage_type(T) typename ntg::type_traits<T>::storage_type
# define ntg_storage_type_(T) ntg::type_traits<T>::storage_type

# define ntg_base_type(T) typename ntg::type_traits<T>::base_type
# define ntg_base_type_(T) ntg::type_traits<T>::base_type

# define ntg_signed_type(T) typename ntg::type_traits<T>::signed_type
# define ntg_signed_type_(T) ntg::type_traits<T>::signed_type

# define ntg_unsigned_type(T) typename ntg::type_traits<T>::unsigned_type
# define ntg_unsigned_type_(T) ntg::type_traits<T>::unsigned_type

# define ntg_integer_type(T) typename ntg::type_traits<T>::integer_type
# define ntg_integer_type_(T) ntg::type_traits<T>::integer_type

# define ntg_cumul_type(T) typename ntg::type_traits<T>::cumul_type
# define ntg_cumul_type_(T) ntg::type_traits<T>::cumul_type

# define ntg_signed_cumul_type(T) \
  typename ntg::type_traits<T>::signed_cumul_type
# define ntg_signed_cumul_type_(T) ntg::type_traits<T>::signed_cumul_type

# define ntg_unsigned_cumul_type(T) \
  typename ntg::type_traits<T>::unsigned_cumul_type
# define ntg_unsigned_cumul_type_(T) ntg::type_traits<T>::unsigned_cumul_type

# define ntg_largest_type(T) typename ntg::type_traits<T>::largest_type
# define ntg_largest_type_(T) ntg::type_traits<T>::largest_type

# define ntg_signed_largest_type(T) \
  typename ntg::type_traits<T>::signed_largest_type
# define ntg_signed_largest_type_(T) ntg::type_traits<T>::signed_largest_type

# define ntg_unsigned_largest_type(T) \
  typename ntg::type_traits<T>::unsigned_largest_type
# define ntg_unsigned_largest_type_(T) \
  ntg::type_traits<T>::unsigned_largest_type

/*-----------------.
| values accessors |
`-----------------*/

# define ntg_zero_val(T) ntg::type_traits<T>::zero()

# define ntg_unit_val(T) ntg::type_traits<T>::unit()

# define ntg_max_val(T) ntg::type_traits<T>::max()

# define ntg_min_val(T) ntg::type_traits<T>::min()

# define ntg_inf_val(T) ntg::type_traits<T>::inf()

# define ntg_sup_val(T) ntg::type_traits<T>::sup()

/*--------------------------.
| operator traits accessors |
`--------------------------*/

# define ntg_return_type(Op, T1, T2) \
  typename internal::deduce_from_traits<Op, T1, T2>::ret
# define ntg_return_type_(Op, T1, T2) \
  internal::deduce_from_traits<Op, T1, T2>::ret

/*----------.
| utilities |
`----------*/

# define ntg_name(T) ntg::type_traits<T>::name()

#endif // ndef NTG_CORE_MACROS_HH
