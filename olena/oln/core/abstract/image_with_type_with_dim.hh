// Copyright (C) 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_WITH_DIM_HH
# define OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_WITH_DIM_HH

# include <mlc/bool.hh>
# include <ntg/basics.hh>

# include <oln/core/abstract/image_with_type.hh>
# include <oln/core/abstract/image_with_dim.hh>

// Define a type label image This macro create two classes,
// Name<Exact> and Name##_with_dim<N, Exact>.  Super is used to
// determine the parent class of Name##_with_dim and is suffixed with
// _with_dim.

# define LABEL_IMAGE_(Name, Super)				\
  template <class Exact>					\
  class Name							\
    : virtual public Super<Exact>				\
  {								\
  public:							\
    typedef Name<Exact> self_type;				\
    typedef Exact exact_type;					\
								\
    exact_type& operator=(self_type rhs)			\
    {								\
      return this->exact().assign(rhs.exact());			\
    }								\
								\
    static std::string name()					\
    {								\
      return							\
	std::string(#Name "<")					\
	+ Exact::name() + ">";					\
    }								\
  };								\
								\
  template <unsigned Dim, class Exact>				\
  class Name##_with_dim						\
    : virtual public Name<Exact>,				\
      virtual public Super##_with_dim<Dim, Exact>		\
  {								\
  public:							\
    typedef Name##_with_dim<Dim, Exact> self_type;		\
    typedef Exact exact_type;					\
								\
    exact_type& operator=(self_type rhs)			\
    {								\
      return this->exact().assign(rhs.exact());			\
    }								\
								\
    static std::string name()					\
    {								\
      std::ostringstream s;					\
      s << #Name "_with_dim<"					\
	<< Dim << ", "						\
	<< Exact::name() << ">";				\
      return s.str();						\
    }								\
  }

namespace oln {
  
  namespace abstract {

    // The label images here were defined because they appeared to be
    // quite handy. They are different than using image_with_type<T>,
    // because it handle categories of types, not only one.

    // With each label image, another label_with_dim<Dim, Exact> class
    // is defined.

    // Proxy towards image_with_type

    template <class Exact>
    struct data_type_image
      : virtual public image_with_type<typename image_id<Exact>::value_type, Exact>
    {
    public:

      typedef data_type_image<Exact> self_type;
      typedef Exact exact_type;

      exact_type& operator=(self_type rhs)
      {
	return this->exact().assign(rhs.exact());
      }

      static std::string name()
      {
	return
	  std::string("abstract::data_type_image<")
	  + Exact::name() + ">";
      }
    };

    template <unsigned Dim, class Exact>
    struct data_type_image_with_dim : 
      virtual public data_type_image<Exact>,
      virtual public image_with_dim<Dim, Exact>
    {
    public:

      typedef data_type_image<Exact> self_type;
      typedef Exact exact_type;

      exact_type& operator=(self_type rhs)
      {
	return this->exact().assign(rhs.exact());
      }

      static std::string name()
      {
	return
	  std::string("abstract::data_type_image<")
	  + Exact::name() + ">";
      }
    };

    LABEL_IMAGE_(vectorial_image, data_type_image);
    LABEL_IMAGE_(non_vectorial_image, data_type_image);
    LABEL_IMAGE_(binary_image, non_vectorial_image);
    LABEL_IMAGE_(integer_image, non_vectorial_image);
    LABEL_IMAGE_(decimal_image, non_vectorial_image);

    template<class Exact>
    struct image_with_type_with_dim_switch
    {
      enum { Dim = image_id<Exact>::dim };
      typedef typename image_id<Exact>::value_type T;

      typedef typename mlc::bool_switch_<

        mlc::bool_case_<ntg_is_a(T, ntg::binary)::ret,
			binary_image_with_dim<Dim, Exact>,

        mlc::bool_case_<ntg_is_a(T, ntg::integer)::ret,
			integer_image_with_dim<Dim, Exact>,

        mlc::bool_case_<ntg_is_a(T, ntg::decimal)::ret,
			decimal_image_with_dim<Dim, Exact>,

        mlc::bool_case_<ntg_is_a(T, ntg::vectorial)::ret,
			vectorial_image_with_dim<Dim, Exact>,

        mlc::bool_case_<ntg_is_a(T, ntg::non_vectorial)::ret,
			non_vectorial_image_with_dim<Dim, Exact>,
			
        mlc::bool_case_<true,
	      	        data_type_image<Exact> >

      > > > > > >::ret_t ret;
    };

  } // end of namespace abstract

} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_WITH_DIM_HH
