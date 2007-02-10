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

#ifndef NTG_CORE_CONTRACT_HH
# define NTG_CORE_CONTRACT_HH

# include <mlc/contract.hh>

/*
  Custom version of assert, which can print a context string if
  NTG_DEBUG is defined. The context string should be filled by
  operators.
*/

# ifdef NDEBUG
#  define ntg_assert(expr) (static_cast<void>(0))
# else
// FIXME: repair NTG_DEBUG
#  if 0 // ifdef NTG_DEBUG
#   define ntg_assert(expr)					\
  if (!(expr))							\
  {								\
    if (!ntg::debug_context.empty())				\
      std::cerr << "In context: " << ntg::debug_context 	\
                << std::endl;					\
    assertion(expr);						\
  }
#  else
    # define ntg_assert(expr) assertion(expr)
#  endif
# endif

#endif // !NTG_CORE_CONTRACT_HH
