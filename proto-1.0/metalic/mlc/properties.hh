// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef METALIC_PROPERTIES_HH
# define METALIC_PROPERTIES_HH

# include <iostream>
# include <typeinfo>

# include <mlc/types.hh>
# include <mlc/bool.hh>
# include <mlc/cmp.hh>


// FIXME: temporary routines; equivalent to the ones of mlc/bool.hh and mlc/cmp.hh
// FIXME: modify mlc/* so that "types" are prefered to "values"...

namespace mlc
{
  namespace internal
  {
    template <class C, class T, class F> struct my_if;
    template <class T, class F> struct my_if <mlc::true_type,  T, F> { typedef T ret; };
    template <class T, class F> struct my_if <mlc::false_type, T, F> { typedef F ret; };

    template <class T1, class T2> struct my_eq { typedef mlc::false_type ret; };
    template <class T> struct my_eq <T, T> { typedef mlc::true_type ret; };
    
  }
}

// FIXME: temporary macros (Cf. above)

# define mlc_internal_if(C,T,F) \
typename mlc::internal::my_if<C,T,F>::ret

# define mlc_internal_eq(T1,T2) \
typename mlc::internal::my_eq<T1,T2>::ret



/// macro to equip a namespace for properties:

# define mlc_equip_namespace_for_properties()					\
										\
  template <typename T>								\
  struct set_category								\
  {										\
    typedef mlc::undefined_type ret;						\
  };										\
										\
  template <typename T>								\
  struct set_super_type								\
  {										\
    typedef mlc::no_type ret;							\
  };										\
										\
  template <typename category, typename type>					\
  struct set_props								\
  {										\
    typedef mlc::false_type user_defined_;					\
  };										\
										\
  template <typename category, typename kind = mlc::no_type>			\
  struct props_of								\
  {										\
  };										\
										\
  template <typename category, typename type>					\
  struct get_props : public props_of <category, type>				\
  {										\
  };										\
										\
  template <typename category, typename from_type, typename target>		\
  struct set_type_of								\
  {										\
    typedef mlc::undefined_type ret;						\
  };										\
										\
  template <typename category, typename from_type, typename target>		\
  struct get_type_of : public set_type_of <category, from_type, target>		\
  {										\
  };										\
										\
  namespace internal								\
  {										\
    template <typename T>							\
    struct get_category : public set_category <T>				\
    {										\
    };										\
    template <typename T>							\
    struct get_super_type : public set_super_type <T>				\
    {										\
    };										\
										\
    template <typename category, typename type>					\
    struct get_props : public set_props <category, type>			\
    {										\
      /* FIXME: test that user_defined_ is ok */				\
    };										\
										\
    template <typename category, typename type, typename target>		\
    struct get_prop								\
    {										\
      typedef mlc::undefined_type ret;						\
    };										\
										\
    template <typename category, typename type, typename target,		\
	      typename is_no_type__type, typename is_user_defined__type>	\
    struct get_prop_helper							\
    {										\
      typedef mlc::undefined_type ret;						\
    };										\
										\
  }										\
void namespace_is_equiped_for_properties()



/// macro to register a property in a props_of<..>:

# define mlc_register_prop(CATEGORY, TARGET)					\
										\
namespace internal								\
{										\
										\
  template <>									\
  struct get_prop < CATEGORY, mlc::no_type, target::TARGET >			\
  {										\
    typedef props_of<CATEGORY, mlc::default_type>:: TARGET ret;			\
  };										\
										\
  template <typename type>							\
  struct get_prop < CATEGORY, type, target::TARGET >				\
  {										\
    typedef get_prop_helper<CATEGORY, type, target::TARGET,			\
			      mlc_internal_eq(type, mlc::no_type),		\
			      typename get_props<CATEGORY, type>::user_defined_	\
			      > helper_type;					\
    typedef typename helper_type::ret ret;					\
  };										\
  template <typename type>							\
  struct get_prop_helper < CATEGORY, type, target::TARGET, mlc::false_type,	\
			     mlc::true_type >					\
  {										\
    typedef typename get_props<CATEGORY, type>:: TARGET prop_type;		\
    typedef typename get_prop<CATEGORY,						\
                              typename get_super_type<type>::ret,		\
                              target::TARGET>::ret super_prop_type;		\
										\
    typedef mlc_internal_if( mlc_internal_eq(prop_type, mlc::unknown_type),	\
			      super_prop_type,					\
			      prop_type ) ret;					\
  };										\
  template <typename type>							\
  struct get_prop_helper < CATEGORY, type, target::TARGET, mlc::false_type,	\
			     mlc::false_type >					\
  {										\
    typedef typename get_prop<CATEGORY,						\
                              typename get_super_type<type>::ret,		\
                              target::TARGET>::ret ret;				\
  };										\
}										\
										\
template <typename type>							\
struct set_type_of < CATEGORY, type, target::TARGET >				\
{										\
  typedef typename internal::get_prop<CATEGORY, type, target::TARGET>::ret ret;	\
}



/// internal macro:

# define mlc_internal_retrieve_prop(CATEGORY, TYPE, TARGET)		\
 typename internal::get_prop<CATEGORY, TYPE, target::TARGET>::ret



/// macros to declare a property:

# define mlc_decl_prop_with_default(CATEGORY, NAME, DEFAULT)			\
  typedef  mlc_internal_if(mlc_internal_eq(type, mlc::default_type),		\
		  DEFAULT,							\
		  mlc_internal_if(mlc_internal_eq(type, mlc::no_type),		\
			 mlc::unknown_type,					\
			 mlc_internal_retrieve_prop(CATEGORY, type, NAME))	\
		  )  NAME

# define mlc_decl_prop(CATEGORY, NAME)						\
  typedef  mlc_internal_if(mlc_internal_eq(type, mlc::default_type),		\
		  mlc::undefined_type,						\
		  mlc_internal_if(mlc_internal_eq(type, mlc::no_type),		\
			 mlc::unknown_type,					\
			 mlc_internal_retrieve_prop(CATEGORY, type, NAME))	\
		  )  NAME



/// macro to retrieve a type (TARGET) from a class (NAMESPACE::TYPE)

# define mlc_type_of(NAMESPACE, TYPE, TARGET)						\
typename NAMESPACE::get_type_of<typename NAMESPACE::internal::get_category<TYPE>::ret,	\
                           TYPE,							\
                           NAMESPACE::target::TARGET##_type>::ret

# define mlc_type_of_(NAMESPACE, TYPE, TARGET)				\
NAMESPACE::get_type_of<typename NAMESPACE::internal::get_category<TYPE>::ret,	\
                           TYPE,					\
                           NAMESPACE::target::TARGET##_type>::ret


/// useful printing macro to debug

# define mlc_print_props(NAMESPACE, TYPE, OSTREAM) \
NAMESPACE::props_of<typename NAMESPACE::internal::get_category<TYPE>::ret, TYPE>::echo(OSTREAM)

# define mlc_print_props_(NAMESPACE, TYPE, OSTREAM) \
NAMESPACE::props_of<NAMESPACE::internal::get_category<TYPE>::ret, TYPE>::echo(OSTREAM)


#endif // ! METALIC_PROPERTIES_HH
