// Copyright (C) 2001, 2002, 2003, 2005  EPITA Research and Development Laboratory
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

#ifndef NTG_REAL_REAL_VALUE_HH
# define NTG_REAL_REAL_VALUE_HH

# include <ntg/core/type_traits.hh>
# include <ntg/core/value.hh>

# define declare_real_value_type__(Type, Parent)			\
  template <class E> 							\
  class Type : public Parent<E>						\
  {};									\
									\
  namespace internal {							\
									\
    template <class E>							\
    struct typetraits<Type<E> > : public typetraits<Parent<E> > 	\
    {};									\
									\
  }

namespace ntg
{

  declare_real_value_type__(real_value, value)
  declare_real_value_type__(float_value, real_value)
  declare_real_value_type__(int_value, real_value)
  declare_real_value_type__(uint_value, int_value)
  declare_real_value_type__(sint_value, int_value)

} // end of ntg.

#endif // !NTG_REAL_REAL_VALUE_HH
