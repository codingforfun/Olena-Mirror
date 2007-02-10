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

#ifndef NTG_UTILS_DEBUG_HH
# define NTG_UTILS_DEBUG_HH

# include <ntg/core/macros.hh>
# include <ntg/core/type_traits.hh>

# include <string>

namespace ntg
{

  /*---------------------.
  | typename_of_var(obj) |
  `---------------------*/

  template<class T> inline
  std::string
  typename_of_var(const T&)
  { return ntg_name(T); }

  /*-----------------.
  | typename_of<T>() |
  `-----------------*/

  /* 
     This function is handy then there are commas in the type
     expression. Else ntg_name(T) is shorter.
   */

  template<class T> inline
  std::string
  typename_of()
  { return ntg_name(T); }

  /*
    Debug context. The idea is to keep a context string to simplify
    debugging when an assert fails somewhere in a hidden operator.  As
    this can be slow, a dynamic boolean specifies wheter the context
    has to be maintained or not.
  */

#ifdef NTG_DEBUG
  extern bool debug_active;
  extern std::string debug_context;
#endif

}

# ifdef NTG_DEBUG
#  define ntg_debug_define_vars(State)		\
  bool ntg::debug_active = State;		\
  std::string ntg::debug_context;
#  define ntg_debug_activate() ntg::debug_active = true
#  define ntg_debug_desactivate() ntg::debug_active = false
#  define ntg_is_debug_active (ntg::debug_active == true)
#  define ntg_debug_context_set(Context) ntg::debug_context = Context
#  define ntg_debug_context_clear(Context) ntg::debug_context.clear()
# else
#  define ntg_debug_define_vars(State)
#  define ntg_debug_activate()
#  define ntg_debug_desactivate()
#  define ntg_is_debug_active false
#  define ntg_debug_context_set(Context)
#  define ntg_debug_context_clear(Context)
# endif

#endif // !NTG_UTILS_DEBUG_HH
