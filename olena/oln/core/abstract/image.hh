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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_HH

# include <mlc/type.hh>
# include <mlc/contract.hh>
# include <oln/core/coord.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/iter.hh>


namespace oln {


  namespace abstract {

    // fwd_decl
    template <class Exact>
    class image;

  } // end of namespace abstract

  template<class Exact>
  struct image_id;

  template<class Ima>
  struct image_traits;
  
  /*! \class image_traits
  ** 
  ** A helping structure to find the
  ** exact_type of a given class.
  */
  
  template <class Exact>
  struct image_traits<abstract::image<Exact> >
  {
    typedef Exact exact_type;
  };

  

  namespace abstract {

    /*! \class image
    **
    ** The image class whom derives all
    ** other image classes.
    */
    
    template <class Exact>
    class image : public mlc_hierarchy::any_with_diamond<Exact>
    {
    public:
      typedef typename image_traits<Exact>::point_type point_type; 
      /*!< Prefer the macro oln_point_type(I) to retrieve the point_type
      ** of an image.
      **
      ** \see point
      */
      typedef typename image_traits<Exact>::dpoint_type dpoint_type; 
      /*!< Prefer the macro oln_dpoint_type(I) to retrieve the dpoint_type
      ** of an image.
      **
      ** \see dpoint
      */
      typedef typename image_traits<Exact>::iter_type iter_type; 
      /*!< Prefer the macro oln_iter_type(I) to retrieve the iter_type
      ** of an image
      **
      ** \see iter
      */
      typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type; 
      /*!< Forward iterator type. */
      typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type; 
      /*!< Backward iterator type. */
      typedef typename image_traits<Exact>::value_type value_type; 
      /*!< Prefer the macro oln_value_type(I) to retrieve the value_type
      ** of an image.
      */
      typedef typename image_traits<Exact>::size_type size_type; 
      /*!< Indicate how the image size is handled.
      **
      ** \see image_size
      */
      typedef typename image_traits<Exact>::impl_type impl_type; 
      /*!< Underlying implementation. */
      typedef image<Exact> self_type; 
      typedef Exact exact_type; 

      enum { dim = image_traits<Exact>::dim };

      
      /*! \brief Return a reference to the value stored
      ** at \a p in the current image.
      */
      
      const value_type&
      operator[](const abstract::point<point_type>& p) const
      {
  	return this->exact().at(p.exact());
      }

      /*! \brief Return the value stored stored at \a p
      ** in the current image.
      */
      
      value_type&
      operator[](const abstract::point<point_type>& p)
      {
	return this->exact().at(p.exact());
      }

      /*! \brief Indicate if the image can be processed.
      ** 
      ** \return True if the image can be processed, false otherwise.
      */

      bool
      has_impl() const
      {
	return this->exact().impl() != 0;
      }

      /*! \brief Clone the image, all the points are 
      ** duplicated.
      ** 
      ** \return A real copy of the current image.
      */
      
      exact_type
      clone() const
      {
	return this->exact().clone_();
      }
      
      /*! \brief Test if the point \a p belong to the image.
      **
      ** \return True if p belong to the image, false otherwise.
      */

      bool
      hold(const abstract::point<point_type>& p) const
      {
	assertion(has_impl());
	return this->exact().impl()->hold(p.exact());
      }
      
      /// Return a reference to the image size.

      const size_type&
      size() const
      {
	assertion(has_impl());
	return this->exact().impl()->size();
      }
      
      /// Return the value of the border width.
      
      coord
      border() const
      {
	return size().border();
      }
      
      /*! \brief Return the total number of points
      ** in the current image.
      */
      

      size_t
      npoints() const
      {
	return this->exact().npoints_();
      }

      /*! \brief Perform a shallow copy from \rhs to
      ** the current image, the points are not duplicated
      ** but shared between the two images.
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
	  std::string("abstract::image<")
	  + Exact::name() + ">";
      }

      // borders

      
      /*! \brief Set the width of the border to perform
      ** operations on the image sides.
      **
      ** \arg new_border The new value of the border width.
      **
      ** \arg copy_border Its default value is false.
      **
      ** \see image_size::border_
      */

      void
      border_set_width(coord new_border, bool copy_border = false) const
      {
	precondition(new_border >= 0);
	precondition(has_impl());
	if (border() == new_border)
	  return; // Nothing to do.

	const_cast<impl_type *>(this->exact().impl())->border_reallocate_and_copy(new_border, copy_border);
      }

      /*! \brief Adapt the border if min_border is 
      ** less or equal to the current border
      ** value.
      ** 
      ** \arg min_border The new value of the border width.
      **
      ** \arg copy_border Its default value is false.
      **
      ** \see image_size::border_
      */
      
      void
      border_adapt_width(coord min_border, bool copy_border =
			 false) const
      {
	precondition(min_border >= 0);
	if (border() >= min_border)
	  return;			// Don't shrink.

	this->exact().border_set_width(min_border, copy_border);
      }

      /*! \brief The border points will have the same
      ** value as the nearer real point of the image.
      **
      ** \arg min_border The new value of the border width.
      **
      ** \see image_size::border_
      */
      
      void
      border_adapt_copy(coord min_border) const
      {
	border_adapt_width(min_border);
	const_cast<impl_type *>(this->exact().impl())->border_replicate();
      }

      /*! \brief The border points value are set according
      ** to the value of the points on the image sides.
      **
      ** \arg min_border The new value of the border width.
      **
      ** \see image_size::border_
      */

      void
      border_adapt_mirror(coord min_border) const
      {
	border_adapt_width(min_border);
	const_cast<impl_type *>(this->exact().impl())->border_mirror();
      }

      /*! \brief The border points value will be equal to 
      ** the \a val parameters.
      **
      ** \arg min_border The new value of the border width.
      **
      ** \arg val The new value of the border points.
      **
      ** \see image_size::border_
      */

      void
      border_adapt_assign(coord min_border, value_type val) const
      {
	border_adapt_width(min_border);
	const_cast<impl_type *>(this->exact().impl())->border_assign(val);
      }

    protected:
      
      image()
      {}

      image(self_type& rhs)
      {};
    };

  } // end of namespace abstract

# define oln_value_type(ImgType)		\
mlc_exact_type(ImgType)::value_type

# define oln_concrete_type(ImgType)		\
typename mute<ImgType>::ret

# define oln_iter_type(Iterable)		\
mlc_exact_type(Iterable)::iter_type

# define oln_point_type(Pointable)		\
mlc_exact_type(Pointable)::point_type

# define oln_dpoint_type(DPointable)		\
mlc_exact_type(DPointable)::dpoint_type

} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_HH
