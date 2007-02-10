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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef NTG_ENUM_BUILTIN_BOOL_HH
# define NTG_ENUM_BUILTIN_BOOL_HH

# include <ntg/core/abstract_hierarchy.hh>
# include <ntg/real/behavior.hh>
# include <ntg/core/predecls.hh>
# include <ntg/core/type_traits.hh>

namespace ntg {

  namespace internal {

    /*-----------------.
    | typetraits<bool> |
    `-----------------*/

    template <>
    struct typetraits<bool>
    {
      typedef bool		self;
      typedef binary		abstract_type;	
      typedef bin		ntg_type;
      typedef optraits<self>	optraits_type;
      typedef unsafe::get<self>	behavior_type;

      typedef self		base_type;
      typedef self		storage_type;
      typedef self		cumul_type;
      typedef self		largest_type;
      typedef self		signed_largest_type;
      typedef self		signed_cumul_type;
      typedef self		unsigned_largest_type;
      typedef self		unsigned_cumul_type;
      typedef unsigned int	integer_type;
    };

    /*---------------.
    | optraits<bool> |
    `---------------*/

    template<>
    struct optraits<bool>
    {
      static bool min()  { return 0; }
      static bool max()  { return 1; }
      static bool zero() { return 0; }
      static bool unit() { return 1; }
      static bool inf()  { return min(); }
      static bool sup()  { return max(); }

      static unsigned max_print_width () { return 1U; }

      static std::string name() { return "bool"; }
    };
    
  } // end of internal.

} // end of ntg.

#endif // !NTG_ENUM_BUILTIN_BOOL_HH
