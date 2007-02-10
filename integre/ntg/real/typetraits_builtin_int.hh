// Copyright (C) 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef NTG_REAL_TYPETRAITS_BUILTIN_INT_HH
# define NTG_REAL_TYPETRAITS_BUILTIN_INT_HH

# include <ntg/real/behavior.hh>
# include <ntg/core/predecls.hh>
# include <ntg/core/type_traits.hh>

/*-------.
| macros |
`-------*/

# define TYPETRAITS_BUILTIN_INT(Name, Value)					\
  template<>									\
  struct typetraits<Name> : public typetraits<Value<Name > >			\
  {										\
    typedef Name self;								\
    typedef builtin::to_ntg<Name >::ret ntg_type;				\
    typedef builtin::abstract_trait<Name >::ret abstract_type;			\
    template <class E>								\
    struct build_value_type							\
    {										\
      typedef typename builtin::value_type<Name, E >::ret ret;			\
    };										\
										\
    typedef optraits<self> optraits;						\
    typedef unsafe::get<self> behavior_type;					\
    typedef unsafe abstract_behavior_type;					\
										\
    typedef self				       base_type;		\
    typedef self				       storage_type;		\
    typedef builtin::signed_trait<Name >::ret	       signed_type;		\
    typedef builtin::unsigned_trait<Name >::ret	       unsigned_type;		\
    typedef builtin::cumul_trait<Name >::ret	       cumul_type;		\
    typedef builtin::largest_trait<Name >::ret	       largest_type;		\
    typedef builtin::signed_largest_trait<Name >::ret   signed_largest_type;	\
    typedef builtin::signed_cumul_trait<Name >::ret     signed_cumul_type;	\
    typedef builtin::unsigned_largest_trait<Name >::ret unsigned_largest_type;	\
    typedef builtin::unsigned_cumul_trait<Name >::ret   unsigned_cumul_type;	\
    typedef self				       integer_type;		\
  };

namespace ntg {

  namespace builtin  {

    template <class T>
    struct abstract_trait { typedef unsigned_integer ret; };
    template <> struct abstract_trait<signed long>  { typedef signed_integer ret; };
    template <> struct abstract_trait<signed int>   { typedef signed_integer ret; };
    template <> struct abstract_trait<signed short> { typedef signed_integer ret; };
    template <> struct abstract_trait<signed char>  { typedef signed_integer ret; };
    template <> struct abstract_trait<       char>  { typedef signed_integer ret; };

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
    template <> struct unsigned_trait<       char>  { typedef unsigned char  ret; };

    template <class T>
    struct cumul_trait { typedef T ret; };
    template <> struct cumul_trait<unsigned short> { typedef unsigned int   ret; };
    template <> struct cumul_trait<  signed short> { typedef signed int     ret; };
    template <> struct cumul_trait<unsigned char>  { typedef unsigned short ret; };
    template <> struct cumul_trait<  signed char>  { typedef signed short   ret; };
    template <> struct cumul_trait<         char>  { typedef signed short   ret; };

    template <class T>
    struct largest_trait { typedef T ret; };
    template <> struct largest_trait<unsigned short> { typedef unsigned int   ret; };
    template <> struct largest_trait<  signed short> { typedef signed int     ret; };
    template <> struct largest_trait<unsigned char>  { typedef unsigned int   ret; };
    template <> struct largest_trait<  signed char>  { typedef signed int     ret; };
    template <> struct largest_trait<         char>  { typedef signed int     ret; };

    template <class T>
    struct signed_largest_trait { typedef signed int ret; };
    template <> struct signed_largest_trait<unsigned long>  { typedef signed long ret; };

    template <class T>
    struct signed_cumul_trait { typedef signed int ret; };
    template <> struct signed_cumul_trait<unsigned long>  { typedef signed long ret; };
    template <> struct signed_cumul_trait<unsigned char>  { typedef signed short ret; };
    template <> struct signed_cumul_trait<  signed char>  { typedef signed short ret; };
    template <> struct signed_cumul_trait<         char>  { typedef signed short ret; };

    template <class T>
    struct unsigned_largest_trait { typedef unsigned int ret; };
    template <> struct unsigned_largest_trait<  signed long>  { typedef unsigned long ret; };

    template <class T>
    struct unsigned_cumul_trait { typedef unsigned int ret; };
    template <> struct unsigned_cumul_trait<signed long>    { typedef unsigned long ret; };
    template <> struct unsigned_cumul_trait<unsigned char>  { typedef unsigned short ret; };
    template <> struct unsigned_cumul_trait<  signed char>  { typedef unsigned short ret; };
    template <> struct unsigned_cumul_trait<         char>  { typedef unsigned short ret; };

    template<class T> struct to_ntg { typedef T ret; };
    template<> struct to_ntg<unsigned char>  { typedef int_u8u  ret; };
    template<> struct to_ntg<  signed char>  { typedef int_s8u  ret; };
    template<> struct to_ntg<         char>  { typedef int_s8u  ret; };
    template<> struct to_ntg<unsigned short> { typedef int_u16u ret; };
    template<> struct to_ntg<  signed short> { typedef int_s16u ret; };
    template<> struct to_ntg<unsigned int>   { typedef int_u32u ret; };
    template<> struct to_ntg<  signed int>   { typedef int_s32u ret; };
    template<> struct to_ntg<unsigned long>  { typedef int_u32u ret; };
    template<> struct to_ntg<  signed long>  { typedef int_s32u ret; };

    template <class T, class E> struct value_type { typedef uint_value<E> ret; };
    template <class E> struct value_type<signed  long, E> { typedef sint_value<E> ret; };
    template <class E> struct value_type<signed   int, E> { typedef sint_value<E> ret; };
    template <class E> struct value_type<signed short, E> { typedef sint_value<E> ret; };
    template <class E> struct value_type<signed  char, E> { typedef sint_value<E> ret; };
    template <class E> struct value_type<        char, E> { typedef sint_value<E> ret; };

  } // end of builtin.

  namespace internal {

    TYPETRAITS_BUILTIN_INT(unsigned long, uint_value)
    TYPETRAITS_BUILTIN_INT(signed long, sint_value)

    TYPETRAITS_BUILTIN_INT(unsigned int, uint_value)
    TYPETRAITS_BUILTIN_INT(signed int, sint_value)

    TYPETRAITS_BUILTIN_INT(unsigned short, uint_value)
    TYPETRAITS_BUILTIN_INT(signed short, sint_value)

    TYPETRAITS_BUILTIN_INT(unsigned char, uint_value)
    TYPETRAITS_BUILTIN_INT(signed char, sint_value)
    TYPETRAITS_BUILTIN_INT(char, sint_value)

  } // end of internal.

} // end of ntg.

#endif // !NTG_REAL_TYPETRAITS_BUILTIN_INT_HH
