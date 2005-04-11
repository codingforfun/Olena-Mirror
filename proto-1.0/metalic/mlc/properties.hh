// Copyright (C) 2005 EPITA Research and Development Laboratory
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

# include <mlc/types.hh>
# include <mlc/typedef.hh>
# include <mlc/bool.hh>
# include <mlc/implies.hh>
# include <mlc/cmp.hh>
# include <mlc/if.hh>
# include <mlc/is_a.hh>


// this is an internal macro
# define mlc_internal_get_typedef_(FromType, TypedefType) \
  typename internal::get_typedef <FromType, TypedefType> ::ret



# define mlc_equip_namespace_with_properties()					\
										\
  mlc_equip_namespace_with_typedef();						\
  mlc_decl_typedef(ret);							\
										\
  template <template <typename> class abstraction>				\
  struct is_a									\
  {										\
    template <typename E>							\
    struct instantiated_with							\
    {										\
      typedef abstraction<E> ret;						\
    };										\
  };										\
										\
  template <typename type, unsigned i = 0>					\
  struct set_super_type								\
  {										\
    typedef mlc::no_type ret;							\
  };										\
										\
  template <typename type, unsigned i = 0>					\
  struct set_category								\
  {										\
    typedef mlc::no_type ret;							\
  };										\
										\
  template <typename category>							\
  struct set_default_props							\
  {										\
  };										\
										\
  template <typename category, typename type>					\
  struct set_props								\
  {										\
  };										\
										\
  template <typename category, typename type>					\
  struct get_props								\
  {										\
  };										\
										\
  template <typename category, typename from_type, typename typedef_type>	\
  struct set_type								\
  {										\
  };										\
										\
  namespace internal								\
  {										\
										\
    template <typename type, unsigned i = 0>					\
    struct get_super_type : public set_super_type <type, i>			\
    {										\
    };										\
										\
    template <typename type, unsigned i = 0>					\
    struct get_category : public set_category <type, i>				\
    {										\
    };										\
										\
    template <typename category>						\
    struct get_default_props : public set_default_props <category>		\
    {										\
    };										\
										\
    template <typename category, typename type>					\
    struct get_props : public set_props <category, type>			\
    {										\
    };										\
										\
    template <typename category, typename from_type, typename typedef_type>	\
    struct get_type : public set_type <category, from_type, typedef_type>	\
    {										\
      ~get_type()								\
      {										\
	typedef set_type <category, from_type, typedef_type> super_type;	\
	typedef mlc_internal_get_typedef_(get_default_props<category>,		\
					  typedef_type) prop_type;		\
										\
	mlc::implies< mlc::neq< mlc_typedef_of(super_type, ret),		\
	                   mlc::internal::not_found >,				\
	         mlc::eq< prop_type,						\
	                  mlc::internal::not_found > >::ensure();		\
      }										\
    };										\
										\
										\
    template <typename category, typename from_type, typename typedef_type>	\
    struct f_rec_get_prop							\
    {										\
      typedef get_props<category, from_type> props;				\
      typedef mlc_internal_get_typedef_(props, typedef_type) prop;		\
										\
      typedef typename								\
      mlc::if_< mlc::neq< prop, mlc::internal::not_found >,			\
	        prop,								\
	   typename f_rec_get_prop< category,					\
				typename get_super_type<from_type, 0>::ret,	\
				typedef_type >::ret				\
      >::ret ret;								\
    };										\
										\
    template <typename category, typename typedef_type>				\
    struct f_rec_get_prop <category, mlc::no_type, typedef_type>		\
    {										\
      typedef mlc_internal_get_typedef_(get_default_props<category>,		\
					typedef_type) ret;			\
      ~f_rec_get_prop()								\
      {										\
	mlc::and_< mlc::neq< ret, mlc::internal::not_found >,			\
	           mlc::neq< ret, mlc::undefined_type > >::ensure();		\
      }										\
    };										\
										\
										\
    template <typename category, typename from_type, typename typedef_type>	\
    struct f_rec_get_type							\
    {										\
      typedef get_type<category, from_type, typedef_type> client_type;		\
      typedef mlc_typedef_of(client_type, ret) type;				\
										\
      typedef typename								\
      mlc::if_< mlc::neq< type, mlc::internal::not_found >,			\
	   type,								\
	   typename f_rec_get_type< category,					\
				typename get_super_type<from_type, 0>::ret,	\
				typedef_type >::ret				\
      >::ret ret;								\
    };										\
										\
    template <typename category, typename typedef_type>				\
    struct f_rec_get_type <category, mlc::no_type, typedef_type>		\
    {										\
      typedef mlc::internal::not_found ret;					\
    };										\
										\
    template <typename category, typename from_type, typename typedef_type>	\
    struct f_get_type								\
    {										\
      typedef									\
      mlc_internal_get_typedef_(get_default_props<category>, typedef_type)	\
      default_prop;								\
										\
      typedef									\
      typename f_rec_get_prop<category, from_type, typedef_type>::ret		\
      prop;									\
										\
      typedef									\
      typename f_rec_get_type<category, from_type, typedef_type>::ret		\
      type;									\
 										\
      ~f_get_type()								\
      {										\
	mlc::implies< mlc::is_found<default_prop>,				\
	              mlc::is_not_found<type> >::ensure();			\
	mlc::xor_< mlc::is_found<prop>,						\
	           mlc::is_found<type> >::ensure();				\
      }										\
										\
      typedef typename mlc::if_< mlc::is_found<prop>, prop, type>::ret ret;	\
    };										\
										\
  }										\
										\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



# define mlc_type_of_(Namespace, Category, FromType, Alias)				\
  Namespace::internal::f_get_type<Category,						\
                                     FromType,						\
                                     Namespace::internal::typedef_::Alias##_type>::ret

# define mlc_type_of(Namespace, Category, FromType, Alias) \
typename mlc_type_of_(Namespace, Category, FromType, Alias)



// FIXME: TODO-list
//
// f_get_type lance d'une part f_rec_get_prop et d'autre part f_rec_get_type
// fusion des résultats: si 2 not_found err, si 1 ok + 1 not_found -> ok
//
// f_rec_get_prop et f_rec_get_type examine plusieurs super (i=0,1,2)


#endif // ! METALIC_PROPERTIES_HH
