// Copyright (C) 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_TYPETRAITS_BUILTINS_FLOAT_HH
# define NTG_TYPETRAITS_BUILTINS_FLOAT_HH

# include <ntg/core/internal/traits.hh>

namespace ntg {

  namespace internal {

    template <>
    struct typetraits<float>
    {
      typedef float self;
      typedef decimal abstract_type;
      typedef self ntg_type;
      ntg_build_value_type(float_value<E>);
      
      typedef optraits<self> optraits;
      typedef unsafe::get<self> behavior_type;     

      typedef self base_type;
      typedef self storage_type;
      typedef self signed_type;
      typedef self unsigned_type;
      typedef self cumul_type;
      typedef self largest_type;
      typedef self signed_largest_type;
      typedef self signed_cumul_type;
      typedef self unsigned_largest_type;
      typedef self unsigned_cumul_type;
      typedef self integer_type;
    };

    template <>
    struct typetraits<double>
    {
      typedef double self;
      typedef decimal abstract_type;
      typedef self ntg_type;
      ntg_build_value_type(float_value<E>);

      typedef optraits<self> optraits;
      typedef unsafe::get<self> behavior_type;

      typedef self base_type;
      typedef self storage_type;
      typedef self signed_type;
      typedef self unsigned_type;
      typedef self cumul_type;
      typedef self largest_type;
      typedef self signed_largest_type;
      typedef self signed_cumul_type;
      typedef self unsigned_largest_type;
      typedef self unsigned_cumul_type;
      typedef self integer_type;
    };

  } // end of internal.

} // end of ntg.

#endif // ndef NTG_TYPETRAITS_BUILTINS_FLOAT_HH
