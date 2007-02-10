// Copyright (C) 2001, 2003, 2004  EPITA Research and Development Laboratory
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


#ifndef OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_HH

# include <oln/core/abstract/image.hh>

# include <string>

namespace oln {

  namespace abstract {
    
    template<class T, class Exact>
    class image_with_type; //fwd declaration
    
  } // end of namespace abstract
  
  
  /*! \class image_traits<abstract::image_with_type<T, Exact> >
  **
  ** The specialized version for image_with_type
  ** 
  ** \warning This class may change, prefer the macro oln_value_type(I)
  ** to retrieve the value_type of an image.
  */
  
  template <class T, class Exact>
  struct image_traits<abstract::image_with_type<T, Exact> >
    : public image_traits<abstract::image<Exact> >
  {
    typedef T value_type;
  };

  
  namespace abstract {

    
    /*! \class image_with_type
    **
    ** The template parameter T gives the value_type of the image.
    */
    
    template<class T, class Exact>
    class image_with_type: virtual public image<Exact>
    {
    public:
      
      typedef image<Exact> super_type; /*!< The base class whom derives image_with_type. */
      typedef image_with_type<T, Exact> self_type;
      typedef Exact exact_type;

      /*! \brief Shallow copy from \a rhs to the current image,
      ** the points will not be duplicated but shared between
      ** the two images.
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
	  std::string("abstract::image_with_type<")
	  + ntg_name(T) + ", "
	  + Exact::name() + ">";
      }

      //    protected:
      //      image_with_type() : super_type() {}

    };

  } // end of namespace abstract

} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_HH
