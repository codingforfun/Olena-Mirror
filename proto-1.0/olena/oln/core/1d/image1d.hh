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

#ifndef OLENA_CORE_1D_IMAGE1D_HH
# define OLENA_CORE_1D_IMAGE1D_HH

# include <oln/core/abstract/image_with_data.hh>
# include <oln/core/1d/array1d.hh>



/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  // fwd decls
  struct fwd_piter1d;
  template <typename T> class image1d;

  // super type
  template <typename T>
  struct set_super_type < image1d<T> > { typedef abstract::image_with_data< image1d<T> > ret; };

  // props
  template <typename T>
  struct set_props < category::image, image1d<T> >
  {
    // intrusive property:
    typedef is_a<abstract::image1d> image_dimension_type;
    // FIXME: should be generalized

    typedef mlc::no_type delegated_type;

    typedef size1d  size_type;
    typedef point1d point_type;
    typedef T       value_type;

    typedef fwd_piter1d piter_type;
    typedef fwd_piter1d fwd_piter_type;

    // please note that value_storage_type means data_type
    // since image1d is an image_with_data
    typedef T value_storage_type;
    // FIXME: above, that should be mlc_encoding_type(T)

    // please note that value_container_type means
    // data_container_type (or value_storage_container_type)
    // FIXME: find a better name...
    typedef array1d<value_storage_type> storage_type;

    // functions

    template <typename U>
    struct ch_value_type
    {
      typedef image1d<U> ret;
    };
  };






  /*! \class image1d<T>
  **
  ** Common class for 1D images.  Pixels values are stored in memory.
  ** FIXME: give details about other properties (border, etc.)
  **
  ** Parameter T is the type of pixel values.
  */

  template <typename T>
  class image1d : public abstract::image_with_data< image1d<T> >
  {
  public:

    typedef abstract::image_with_data< image1d<T> > super_type;

    image1d()
    {
      this->exact_ptr = this;
    }

    image1d(const size1d& size) : super_type(size)
    {
      this->exact_ptr = this;
    }

    image1d(coord_t nindices, size_t border = 2) :
      super_type(size1d(nindices, border))
    {
      this->exact_ptr = this;
    }

    image1d(image1d& rhs) :
      super_type(rhs)
    {
      this->exact_ptr = this;
    }

  };


} // end of namespace oln


#endif // ! OLENA_CORE_1D_IMAGE1D_HH
