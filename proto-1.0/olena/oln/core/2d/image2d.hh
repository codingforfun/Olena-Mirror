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

#ifndef OLENA_CORE_IMAGE2D_HH
# define OLENA_CORE_IMAGE2D_HH

# include <mlc/traits.hh>

# include <oln/core/abstract/image_identity.hh>
# include <oln/core/abstract/image_with_data.hh>
# include <oln/core/2d/array2d.hh>
# include <oln/core/2d/fwd_piter2d.hh>
# include <oln/io/read_image.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  // fwd decl
  template <typename T> class image2d;

  // category decl
  template <typename T>
  struct category_type< image2d<T> > { typedef cat::image ret; };


  /*! \class props< abstract::image, image2d<T> >
  **
  ** Properties of common 2D images.  Specialization of props<abstraction,type>.
  ** Parameter T is the type of pixel values.
  **
  ** \see image2d<T>, props<category,type>
  */

  template <typename T>
  struct props < cat::image, image2d<T> > : public default_props < cat::image >
  {
    typedef mlc::no_type delegated_type;

    typedef size2d  size_type;
    typedef point2d point_type;
    typedef T       value_type;

    typedef fwd_piter2d piter_type;
    typedef fwd_piter2d fwd_piter_type;

    // please note that value_storage_type means data_type
    // since image2d is an image_with_data
    typedef mlc_encoding_type(T) value_storage_type;

    // please note that value_container_type means
    // data_container_type (or value_storage_container_type)
    // FIXME: find a better name...
    typedef array2d<value_storage_type> value_container_type;

    // tags
    typedef tag::readwrite constness_tag;
    typedef tag::dimension2 dimension_tag;

    // functions

    template <typename U>
    struct ch_value_type
    {
      typedef image2d<U> ret;
    };

  };



  /*! \class image2d<T>
  **
  ** Common class for 2D images.  Pixels values are stored in memory.
  ** FIXME: give details about other properties (border, etc.)
  **
  ** Parameter T is the type of pixel values.
  */

  template <typename T>
  class image2d : public abstract::image_with_data< image2d<T> >
  {
  public:

    typedef abstract::image_with_data< image2d<T> > super_type;

    image2d()
    {
      this->exact_ptr = this;
    }

    image2d(coord_t nrows, coord_t ncols) :
      super_type(size2d(nrows, ncols))
    {
      this->exact_ptr = this;
    }

    image2d(image2d& rhs) :
      super_type(rhs)
    {
      this->exact_ptr = this;
    }

    image2d& operator=(image2d& rhs)
    {
      if (&rhs == this)
	return *this;
      std::cout << "shallow copy" << std::endl;
      super_type::operator=(rhs);
      return *this;
    };

    template <typename I, typename E>
    image2d& operator=(const abstract::image_identity<I, E>& rhs)
    {
      return *this = rhs.image();
    }

    image2d& operator=(const io::filename& rhs)
    {
      io::do_read(*this, rhs);
      return *this;
    }

    // FIXME: idem with abstract::image2d<E> (?)

  };


} // end of namespace oln


#endif // ! OLENA_CORE_IMAGE2D_HH
