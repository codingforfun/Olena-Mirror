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

#ifndef OLENA_CORE_IMPL_IMAGE_IMPL_HH
# define OLENA_CORE_IMPL_IMAGE_IMPL_HH

# include <oln/core/internal/fake.hh>
# include <oln/core/coord.hh>
# include <oln/core/abstract/image.hh>
# include <mlc/contract.hh>
# include <mlc/type.hh>

# include <iostream>

namespace oln {

  namespace impl {

    template<class Exact>
    class image_impl;

  } // end of impl

  template<class Impl>
  struct impl_traits;

  /*! \class impl_traits<impl::image_impl<Exact> >
  **
  ** Specialized version for impl::image_impl<Exact>. Retrieve
  ** associated types.
  */

  template<class Exact>
  struct impl_traits<impl::image_impl<Exact> >
  {

  };

  namespace impl {

    /*! \class image_impl
    **
    ** Data array implementation for image
    */

    template<class Exact>
    class image_impl : public mlc_hierarchy::any<Exact>
    {

    public:

      typedef typename impl_traits<Exact>::point_type point_type;
      typedef typename impl_traits<Exact>::value_type value_type;
      typedef typename impl_traits<Exact>::size_type size_type;

      typedef Exact exact_type;

      image_impl(const size_type s): refcount_(0), size_(s) {}


      /// Notice that there is a new reference to the object.

      void
      ref() const
      {
	++refcount_;
      }

      /*! \brief Notice that there a reference to the object has disappeared.
      ** When there is no reference left, the object is deleted.
      */

      void
      unref() const
      {
	invariant(refcount_ > 0);
	--refcount_;
	if (refcount_ == 0)
	  delete mlc::to_exact(this);
      }

      /// Return a constant reference to the value stored at \a p.

      const value_type&
      at(const point_type& p) const
      {
	return this->exact().at_(p);
      }

      /// Return a reference to the value stored at \a p.

      value_type&
      at(const point_type& p)
      {
	return this->exact().at_(p);
      }

      /// Return true if \a p belongs to the image.

      bool
      hold(const point_type& p) const
      {
	return this->exact().hold_(p);
      }

      /// Return true if \a p belongs to the image or the image border.

      bool
      hold_large(const point_type& p) const
      {
	return this->exact().hold_large_(p);
      }

      /// Use the function for debugging purpose.

      void
      precondition_hold_large(const point_type& p) const
      {
# ifndef NDEBUG
	if (! hold_large(p))
	  std::cerr << "image overflow at (" << p << ")" << std::endl;
# else
	(void) p;
# endif
      }


      /// Perform a deep copy from the data array to output_data.

      void
      clone_to(exact_type* output_data) const
      {
	return this->exact().clone_to_(output_data);
      }

      /// Return the number of point in the image.

      const
      size_type& size() const
      {
	return size_;
      }

      size_type&
      size()
      {
	return size_;
      }

      // borders

      /*! \brief Reallocate the border regarding to the value of \a
      ** new_border.
      */

      void
      border_reallocate_and_copy(coord new_border, bool
				 copy_border)
      {
	this->exact().border_reallocate_and_copy_(new_border, copy_border);
      }

      /*! \brief The border points are all set to
      ** the value of the closest image point.
      */

      void
      border_replicate(void)
      {
	this->exact().border_replicate_();
      }

      /*! \brief The border points are set by mirroring
      ** the image edges.
      */

      void
      border_mirror(void)
      {
	this->exact().border_mirror_();
      }

      /// The border points are set to \a val.

      void
      border_assign(value_type val)
      {
	this->exact().border_assign_(val);
      }

    private:

      mutable unsigned refcount_;

    protected:

      size_type size_;

    };

  } // end of namespace impl

} // end of namespace oln

#endif // ! OLENA_CORE_IMPL_IMAGE_IMPL_HH
