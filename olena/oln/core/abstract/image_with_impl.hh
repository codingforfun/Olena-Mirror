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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_WITH_IMPL_HH
# define OLENA_CORE_ABSTRACT_IMAGE_WITH_IMPL_HH

# include <oln/core/abstract/image_with_type_with_dim.hh>
# include <oln/core/impl/image_impl.hh>


namespace oln {

  namespace abstract {

    template<class Impl, class Exact>
    class image_with_impl; // fwd_decl

  } // end of namespace abstract

  /*! \class image_traits<abstract::image_with_impl<Impl, Exact> >
  ** 
  ** The specialized version for image_with_impl,
  ** give the impl_type of an image.
  **
  */
  
  template<class Impl, class Exact>
  struct image_traits<abstract::image_with_impl<Impl, Exact> > : 
    public image_traits<typename abstract::image_with_dim<image_id<Exact>::dim, Exact> >,
				public image_traits<typename abstract::image_with_type<typename image_id<Exact>::value_type, Exact> >
  {
    typedef Impl impl_type;
  };
  
  
  
  namespace abstract {
    
    /*! \class image_with_impl
    **
    ** This class contains all the implementation relative methods.
    */
    
    template<class Impl, class Exact>
    class image_with_impl: 
      public image_with_type_with_dim_switch<Exact>::ret
    {
    public:
      
      typedef typename image_traits<Exact>::point_type point_type; 
      /*!< Prefer the macro oln_point_type(I) to retrieve the point_type
      ** of an image.
      **
      ** \see point
      */
      typedef typename image_traits<Exact>::iter_type iter_type; 
      /*!< Prefer the macro oln_iter_type(I) to retrieve the iter_type 
      ** of an image.
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
      
      typedef image_with_impl<Impl, Exact> self_type;
      typedef Exact exact_type;
      typedef typename image_with_type_with_dim_switch<Exact>::ret super_type; 
      friend class image<exact_type>;
      friend class image_with_dim<image_id<Exact>::dim, exact_type>;
      friend class image_with_type<typename image_id<Exact>::value_type, Exact>;

      // shallow copy

      /*! /brief Construct a new image by performing 
      ** a shallow copy, the points will not be 
      ** duplicated but shared between \a rhs and the
      ** current image.
      ** 
      ** \see image::clone()
      */

      image_with_impl(self_type& rhs) 
	: super_type(rhs)
      {
	assertion(rhs.has_impl());
	impl_ = rhs.impl();
	impl_->ref();
      }

      /*! \brief Perform a shallow copy from rhs to
      ** the current image, the points will not be 
      ** duplicated but shared between the two images.
      **
      ** \see image::clone()
      */
      
      exact_type& 
      operator=(self_type rhs)
      {
	return this->exact().assign(rhs.exact());
      }

      /// Return the core data of the image.
      
      const impl_type* 
      impl() const
      {
	return impl_;
      }

      /// Return the core data of the image.

      impl_type* 
      impl()
      {
	return impl_;
      }

      static std::string 
      name()
      {
	return
	  std::string("abstract::image_with_impl<")
	  + Impl::name() + ", "
	  + Exact::name() + ">";
      }

      /*! \brief Create a copy of the current image data, 
      ** at the \a output_data address.
      */
      
      void 
      clone_to(impl_type* output_data) const
      {
	assertion(impl_ != 0);
	impl()->clone_to(output_data);
      }

      /// Free the image data.

      void 
      clear()
      {
	if (impl_ != 0)
	  {
	    impl_->unref();
	    impl_ = 0; // security
	  }
      }

    protected:
      
      /*! \brief Return a reference to the value stored 
      ** at \a p in the current image.
      **
      ** \warning This method must not be overloaded,
      ** unlike operator[].
      ** 
      ** \see image::operator[]()
      */
      
      const value_type& 
      at(const point_type& p) const
      {
	return impl_->at(p);
      }
      
        
      /*! \brief Return the value stored at \a p in the
      ** current image.
      **
      ** \warning This method must not be overloaded,
      ** unlike operator[].
      ** 
      ** \see image::operator[]()
      */
      
      value_type& 
      at(const point_type& p)
      {
	return impl_->at(p);
      }

      /*! \brief Perform a shallow copy from \a rhs to
      ** the current image, the points will not be 
      ** duplicated but shared between the two images.
      **
      ** \see image::clone()
      */
      
      exact_type& 
      assign(exact_type rhs) // shallow assignment
      {
	assertion(rhs.impl() != 0);
	if ( &rhs == this )
	  return this->exact();
	if (this->impl() != 0)
	  this->impl()->unref();
	this->impl_ = rhs.impl();
	this->impl()->ref();
	return this->exact();
      }
      
      ~image_with_impl()
      {
	clear();
      }

      /*! \brief Empty constructor for image_with_impl,
      ** set \a impl_ to 0
      */

      image_with_impl() : impl_(0)
      {}
      
      /// Assign \a impl to \a this->impl_.
      
      image_with_impl(impl_type* impl) :
	super_type(),
	impl_(impl) 
      {
	impl_->ref(); 
      }
      
    private:
      
      /*! \brief Implementation type of the image. 
      ** It can be an array, a std::vector, ...
      */
      
      impl_type* impl_;
      
      image_with_impl(const self_type& rhs); // w/o impl

    };

  } // end of namespace abstract

} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_WITH_IMPL_HH
