// Copyright (C) 2003, 2004  EPITA Research and Development Laboratory
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

// oln_label_image_(Name, Super)
// Define a type label image This macro create two classes,
// Name<Exact> and Name##_with_dim<N, Exact>.  Super is used to
// determine the parent class of Name##_with_dim and is suffixed with
// _with_dim.


# define oln_label_image_(Name, Super)				\
  template <class Exact>					\
  class Name							\
    : virtual public Super<Exact>				\
  {								\
  public:							\
    typedef Name<Exact> self_type;				\
    typedef Exact exact_type;					\
								\
    exact_type&							\
    operator=(self_type rhs)					\
    {								\
      return this->exact().assign(rhs.exact());			\
    }								\
								\
    static std::string				      		\
    name()							\
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
    exact_type& 						\
    operator=(self_type rhs)				       	\
    {								\
      return this->exact().assign(rhs.exact());			\
    }								\
								\
    static std::string						\
    name()							\
    {								\
      std::ostringstream s;					\
      s << #Name "_with_dim<"					\
	<< Dim << ", "						\
	<< Exact::name() << ">";				\
      return s.str();						\
    }								\
  };

namespace oln {
  
  namespace abstract {

    
    // The label images here were defined because they appeared to be
    // quite handy. They are different than using image_with_type<T>,
    // because it handle categories of types, not only one.
    //
    // With each label image, another label_with_dim<Dim, Exact> class
    // is defined.
    //
    // Proxy towards image_with_type
    
    /*! \class data_type_image
    **
    ** This class is used as a proxy towards image_with_type.
    ** \note This shouldn't be used.
    */
    
    template <class Exact>
    struct data_type_image
      : virtual public image_with_type<typename image_id<Exact>::value_type, Exact>
    {
    public:

      typedef data_type_image<Exact> self_type;
      typedef Exact exact_type;

      /*! \brief Perform a shallow copy between \a rhs and the
      ** current point, the points will not be duplicated but 
      ** shared between the two images.
      ** 
      ** \see image::clone()
      */

      
      exact_type& 
      operator=(self_type rhs)
      {
	return this->exact().assign(rhs.exact());
      }

      static std::string 
      name()
      {
	return
	  std::string("abstract::data_type_image<")
	  + Exact::name() + ">";
      }
    };
    
    /*! \class data_type_image_with_dim
    **
    **
    ** This class when used in a function declaration,
    ** force the function to only accept images with a 
    ** dimension equal to \a Dim.
    */
    
    
    template <unsigned Dim, class Exact>
    struct data_type_image_with_dim : 
      virtual public data_type_image<Exact>,
      virtual public image_with_dim<Dim, Exact>
    {
    public:

      typedef data_type_image<Exact> self_type;
      typedef Exact exact_type;

      /*! \brief Perform a shallow copy between \a rhs and the
      ** current point, the points will not be duplicated but
      ** shared between the two images.
      **
      ** \see image::clone()
      */
      
      exact_type& 
      operator=(self_type rhs)
      {
	return this->exact().assign(rhs.exact());
      }

      static std::string 
      name()
      {
	return
	  std::string("abstract::data_type_image<")
	  + Exact::name() + ">";
      }
    };

    /*! This class, when used in a function declaration,
    ** forces the function to only accept vectorial images.
    */
    
    oln_label_image_(vectorial_image, data_type_image);
    
    /*! This class, when used in a function declaration,
    ** forces the function to only accept vectorial images
    ** with a dimension equal to 'Dim'.
    */
    

    /*! \class non_vectorial_image
    ** 
    ** This class, when used in a function declaration,
    ** forces the function to only accept non vectorial images.
    */

    oln_label_image_(non_vectorial_image, data_type_image);

    /*! \class non_vectorial_image_with_dim
    **
    ** This class, when used in a function declaration,
    ** forces the function to only accept non vectorial images.
    ** with a dimension equal to 'Dim'
    */
    

    /*! \class binary_image
    ** 
    ** This class, when used in a function declaration,
    ** forces the function to only accept binary images.
    */ 
    
    oln_label_image_(binary_image, non_vectorial_image);
    
    /*! \class binary_image_with_dim
    **
    ** This class, when used in a function declaration,
    ** forces the function to only accept binary images
    ** with a dimension equal to 'Dim'.
    */
    

    /*! \class integer_image
    ** 
    ** This class, when used in a function declaration,
    ** forces the function to only accept integer images.
    */ 

    oln_label_image_(integer_image, non_vectorial_image);
    
    /*! \class integer_image_with_dim
    **
    ** This class, when used in a function declaration,
    ** forces the function to only accept integer images
    ** with a dimension of 'Dim'.
    */
    

    /*! \class decimal_image
    ** 
    ** This class, when used in a function declaration,
    ** forces the function to only accept decimal images.
    */ 

    oln_label_image_(decimal_image, non_vectorial_image);
    
    /*! \class decimal_image_with_dim
    **
    ** This class, when used in a function declaration,
    ** forces the function to only accept decimal images
    ** with a dimension of 'Dim'.
    */



    /*! \class image_with_type_with_dim_switch
    **
    ** A metaswitch that return the right type of
    ** an image regarding its dimension and 
    ** value_type.
    */
    
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
	      	        data_type_image_with_dim<Dim, Exact> >

      > > > > > >::ret ret; /*!< Translated in pseudo code :\n
			    ** switch (T)\n
			    **  case ntg::binary : ret = binary_image_with_dim<Dim, Exact>\n
			    **  case ntg::integer : ret = integer_image_with_dim<Dim, Exact>\n
			    **  case ntg::decimal : ret = decimal_image_with_dim<Dim, Exact>\n
			    **  case ntg::vectorial : ret = vectorial_image_with_dim<Dim, Exact>\n
			    **  case ntg::non_vectorial : ret = non_vectorial_image_with_dim<Dim, Exact>\n
			    **  default : ret = data_type_image_with_dim<Dim, Exact>\n
			    */
    };

  } // end of namespace abstract

} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_WITH_DIM_HH
