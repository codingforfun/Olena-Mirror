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

#ifndef NTG_TYPETRAITS_BUILTINS_INT_HH
# define NTG_TYPETRAITS_BUILTINS_INT_HH

# include <ntg/typetraits.hh>
# include <ntg/optraits.hh>
# include <ntg/behaviour.hh>

# define TYPETRAITS_BUILTIN_INT(Name)						\
  template<>									\
  struct typetraits<Name>							\
  {										\
    typedef Name self;								\
    typedef optraits<self> optraits;						\
    typedef unsafe::get<self> behaviour_type;					\
										\
    typedef self					base_type;		\
    typedef self					storage_type;		\
    typedef builtin::signed_trait<Name>::ret		signed_type;		\
    typedef builtin::unsigned_trait<Name>::ret		unsigned_type;		\
    typedef builtin::cumul_trait<Name>::ret		cumul_type;		\
    typedef builtin::largest_trait<Name>::ret		largest_type;		\
    typedef builtin::signed_largest_trait<Name>::ret	signed_largest_type;	\
    typedef builtin::signed_cumul_trait<Name>::ret	signed_cumul_type;	\
    typedef builtin::unsigned_largest_trait<Name>::ret	unsigned_largest_type;	\
    typedef builtin::unsigned_cumul_trait<Name>::ret	unsigned_cumul_type;	\
    typedef self					integer_type;		\
										\
    typedef self op_traits;							\
										\
  };

namespace ntg
{

  namespace builtin
  {

    template <class T>
    struct signed_trait { typedef T ret; };
    template <> struct signed_trait<unsigned long>  { typedef signed long  ret; };
    template <> struct signed_trait<unsigned int>   { typedef signed int   ret; };
    template <> struct signed_trait<unsigned short> { typedef signed int   ret; };
    template <> struct signed_trait<unsigned char>  { typedef signed short ret; };

    template <class T>
    struct unsigned_trait { typedef T ret; };
    template <> struct unsigned_trait<signed long>  { typedef unsigned long  ret; };
    template <> struct unsigned_trait<signed int>   { typedef unsigned int   ret; };
    template <> struct unsigned_trait<signed short> { typedef unsigned short ret; };
    template <> struct unsigned_trait<signed char>  { typedef unsigned char  ret; };

    template <class T>
    struct cumul_trait { typedef T ret; };
    template <> struct cumul_trait<unsigned short> { typedef unsigned int   ret; };
    template <> struct cumul_trait<  signed short> { typedef signed int     ret; };
    template <> struct cumul_trait<unsigned char>  { typedef unsigned short ret; };
    template <> struct cumul_trait<  signed char>  { typedef signed short   ret; };

    template <class T>
    struct largest_trait { typedef T ret; };
    template <> struct largest_trait<unsigned short> { typedef unsigned int   ret; };
    template <> struct largest_trait<  signed short> { typedef signed int     ret; };
    template <> struct largest_trait<unsigned char>  { typedef unsigned int   ret; };
    template <> struct largest_trait<  signed char>  { typedef signed int     ret; };

    template <class T>
    struct signed_largest_trait { typedef T ret; };
    template <> struct signed_largest_trait<unsigned long>  { typedef signed long ret; };
    template <> struct signed_largest_trait<unsigned int>   { typedef signed int ret; };
    template <> struct signed_largest_trait<unsigned short> { typedef signed int ret; };
    template <> struct signed_largest_trait<  signed short> { typedef signed int ret; };
    template <> struct signed_largest_trait<unsigned char>  { typedef signed int ret; };
    template <> struct signed_largest_trait<  signed char>  { typedef signed int ret; };
    
    template <class T>
    struct signed_cumul_trait { typedef T ret; };
    template <> struct signed_cumul_trait<unsigned long>  { typedef signed long ret; };
    template <> struct signed_cumul_trait<unsigned int>   { typedef signed int ret; };
    template <> struct signed_cumul_trait<unsigned short> { typedef signed int ret; };
    template <> struct signed_cumul_trait<  signed short> { typedef signed int ret; };
    template <> struct signed_cumul_trait<unsigned char>  { typedef signed short ret; };
    template <> struct signed_cumul_trait<  signed char>  { typedef signed short ret; };

    template <class T>
    struct unsigned_largest_trait { typedef T ret; };
    template <> struct unsigned_largest_trait<  signed long>  { typedef unsigned long ret; };
    template <> struct unsigned_largest_trait<  signed int>   { typedef unsigned int ret; };
    template <> struct unsigned_largest_trait<unsigned short> { typedef unsigned int ret; };
    template <> struct unsigned_largest_trait<  signed short> { typedef unsigned int ret; };
    template <> struct unsigned_largest_trait<unsigned char>  { typedef unsigned int ret; };
    template <> struct unsigned_largest_trait<  signed char>  { typedef unsigned int ret; };
        
    template <class T>
    struct unsigned_cumul_trait { typedef T ret; };
    template <> struct unsigned_cumul_trait<signed long>    { typedef unsigned long ret; };
    template <> struct unsigned_cumul_trait<signed int>     { typedef unsigned int ret; };
    template <> struct unsigned_cumul_trait<unsigned short> { typedef unsigned int ret; };
    template <> struct unsigned_cumul_trait<  signed short> { typedef unsigned int ret; };
    template <> struct unsigned_cumul_trait<unsigned char>  { typedef unsigned short ret; };
    template <> struct unsigned_cumul_trait<  signed char>  { typedef unsigned short ret; };

  } // end of builtin

  TYPETRAITS_BUILTIN_INT(unsigned long);
  TYPETRAITS_BUILTIN_INT(  signed long);

  TYPETRAITS_BUILTIN_INT(unsigned int);
  TYPETRAITS_BUILTIN_INT(  signed int);

  TYPETRAITS_BUILTIN_INT(unsigned short);
  TYPETRAITS_BUILTIN_INT(  signed short);

  TYPETRAITS_BUILTIN_INT(unsigned char);
  TYPETRAITS_BUILTIN_INT(  signed char);


} // end of ntg

#endif // ndef NTG_TYPETRAITS_BUILTINS_INT_HH
