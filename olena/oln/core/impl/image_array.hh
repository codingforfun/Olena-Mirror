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

#ifndef OLENA_CORE_IMPL_IMAGE_ARRAY_HH
# define OLENA_CORE_IMPL_IMAGE_ARRAY_HH

# include <oln/core/internal/fake.hh>
# include <mlc/contract.hh>
# include <oln/core/coord.hh>
# include <oln/core/impl/image_impl.hh>
# include <iostream>
# include <ntg/all.hh>

namespace oln {

  /*! \brief Allocate an array of \a s elements of \a T
  ** type.
  **
  ** \pre s > 0
  */

  template<class T>
  void
  allocate_data_(T*& buffer, size_t s)
  {
    precondition(s > 0);
    buffer = new T[s];
  }

  /*! \brief Free memory pointed to by buffer, then set buffer to 0.
  **
  ** \pre buffer != 0
  */

  template<class T>
  void
  desallocate_data_(T*& buffer)
  {
    precondition(buffer != 0);
    delete[] buffer;
    buffer = 0; // security
  }

  namespace impl {
    template<class T, class Exact>
    class image_array;
  } // end of impl


  /*! \class impl_traits<impl::image_array<T, Exact> >
  **
  ** The specialized version for impl::image_array<T, Exact>
  */

  template<class T, class Exact>
  struct impl_traits<impl::image_array<T, Exact> >: public impl_traits<impl::image_impl<Exact> >
  {

  };

  /*! \brief Representation of the image in memory.
  */
  namespace impl {

    /*! \class image_array
    **
    ** Array implementation of image data.
    */

    template<class T, class Exact>
    class image_array: public image_impl<Exact>
    {

    public:

      typedef typename impl_traits<Exact>::point_type point_type;
      typedef typename impl_traits<Exact>::value_type value_type;
      typedef typename impl_traits<Exact>::size_type size_type;
      typedef Exact exact_type;


      typedef image_impl<Exact> super_type;
      typedef image_array<T, Exact> self_type;

      enum { dim = impl_traits<Exact>::dim };

      friend class image_impl<Exact>;

      /*! \brief Constructor that allocates \a buffer_ to be
      ** an array of \a s \a value_type.
      **
      ** \pre s > 0
      */

      image_array(const size_type& s): super_type(s), buffer_(0)
      {
	allocate_data_(buffer_, len(s));
      }

      image_array() : buffer_(0)
      {}

      image_array(const self_type&);     // cpy ctor  w/o impl

      void
      operator=(const self_type&); // assign op w/o impl

      /*! \brief Return a constant pointer to the data array.
      **
      ** \invariant buffer_ != 0
      */

      const T*
      buffer() const
      {
	invariant(buffer_ != 0);
	return buffer_;
      }

      /*! \brief Return a point to the data array.
      **
      ** \invariant buffer_ != 0
      */

      T*
      buffer()
      {
	invariant(buffer_ != 0);
	return buffer_;
      }

      /// Return the length of the data array.
      size_t
      len() const
      {
	return len(this->size());
      }

      /// Return the length of the data array.
      size_t
      len(const size_type& s) const
      {
	return this->exact().len_(s);
      }

    protected:


      ~image_array()
      {
	if (buffer_)
	  desallocate_data_(buffer_);
      }

      /*! \brief Perform a deep copy of the current data array.
      ** This copy is pointed to by \a output_data.
      **
      ** \pre output_data != 0
      **
      ** \pre output_data->len() == len()
      */

      void
      clone_to_(exact_type* output_data) const
      {
	precondition(output_data != 0);
	precondition(output_data->len() == len());
	memcpy(output_data->buffer(),
	       buffer_,
	       len() * sizeof(T));
      }

      T* buffer_;

    };

  } // end of namespace impl

} // end of namespace oln

#endif // ! OLENA_CORE_IMPL_IMAGE_ARRAY_HH
