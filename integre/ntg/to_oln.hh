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

#ifndef NTG_TO_OLN_HH
# define NTG_TO_OLN_HH

# include <ntg/predecls.hh>

// FIXME: rename, oln has nothing to do here !

# define TO_OLN_CAST(T, Val) \
  (static_cast<typename ntg::internal::to_oln<T>::ret>(Val))

namespace ntg
{

  namespace internal
  {

    template<class T> struct to_oln { typedef T ret; };

    // C built-in type  ->  oln type

    template<> struct to_oln<bool>           { typedef bin ret; };

    template<> struct to_oln<unsigned char>  { typedef int_u8u  ret; };
    template<> struct to_oln<  signed char>  { typedef int_s8u  ret; };
    template<> struct to_oln<unsigned short> { typedef int_u16u ret; };
    template<> struct to_oln<  signed short> { typedef int_s16u ret; };
    template<> struct to_oln<unsigned long>  { typedef int_u32u ret; };
    template<> struct to_oln<  signed long>  { typedef int_s32u ret; };

    template<> struct to_oln<unsigned int>   { typedef int_u32u  ret; };
    template<> struct to_oln<  signed int>   { typedef int_s32u  ret; };

  } // end of internal

} // end of ntg

#endif // ! NTG_TO_OLN_HH
