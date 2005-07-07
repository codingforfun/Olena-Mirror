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

# include <oln/core/abstract/image_with_data.hh>
# include <oln/core/abstract/image_like_.hh>
# include <oln/core/traits.hh>
# include <oln/core/2d/array2d.hh>
# include <oln/core/2d/niter2d.hh>
# include <oln/utils/record.hh>



/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {

  namespace io {

    struct filename;

    template <typename I>
    void do_read(abstract::image<I>&, const filename&);
  }



  // fwd decls
  struct fwd_piter2d;
  struct bkd_piter2d;
  template <typename T> class image2d;

  // super
  template <typename T>
  struct set_super_type < image2d<T> > { typedef abstract::image_with_data< image2d<T> > ret; };

  // props
  template <typename T>
  struct set_props < category::image, image2d<T> >
  {
    typedef grid2d grid_type;

    // intrusive property:
    typedef is_a<abstract::image2d> image_dimension_type;
    // FIXME: should be generalized

    typedef mlc::no_type delegated_type;

    typedef size2d  size_type;
    typedef point2d point_type;
    typedef T       value_type;

    typedef fwd_piter2d piter_type;
    typedef fwd_piter2d fwd_piter_type;
    typedef bkd_piter2d bkd_piter_type;
    typedef fwd_niter2d fwd_niter_type;

    // please note that value_storage_type means data_type
    // since image2d is an image_with_data
    typedef oln_storage_type(T) value_storage_type;

    // please note that value_container_type means
    // data_container_type (or value_storage_container_type)
    // FIXME: find a better name...
    typedef array2d<value_storage_type> storage_type;

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

    image2d(const std::string& name)
    {
      this->exact_ptr = this;
      registering(*this, name);
    }

    image2d(const size2d& size) :
      super_type(size)
    {
      this->exact_ptr = this;
    }

    image2d(const size2d& size,
	    const std::string& name) :
      super_type(size)
    {
      this->exact_ptr = this;
      registering(*this, name);
    }

    image2d(coord_t nrows, coord_t ncols, size_t border = 2) :
      super_type(size2d(nrows, ncols, border))
    {
      this->exact_ptr = this;
    }

    image2d(coord_t nrows, coord_t ncols, size_t border,
	    const std::string& name) :
      super_type(size2d(nrows, ncols, border))
    {
      this->exact_ptr = this;
      registering(*this, name);
    }

    image2d(image2d& rhs) :
      super_type(rhs)
    {
      this->exact_ptr = this;
    }

    image2d& operator=(image2d rhs) // no '&' here is fine!
    {
      this->super_type::operator=(rhs);
      return *this;
    };

    image2d& operator=(const io::filename& rhs)
    {
      // FIXME HERE: hack to avoid calling io
      io::do_read(*this, rhs);
//       image2d tmp(4, 4);
//       *this = tmp;
      return *this;
    }

    // FIXME: idem with abstract::image2d<E> (?)


    // without impl
    image2d(const image2d&);


  public:

    /*! Return a reference to the value stored at coordinate
    **  (\a row, \a col) in the current (const) image.
    */
    value_box< const image2d<T> > operator()(coord_t row, coord_t col) const
    {
      return (*this)[point2d(row, col)];
    }

    /*! Return a reference to the value stored at coordinate
    **  (\a row, \a col) in the current image.
    */
    value_box< image2d<T> > operator()(coord_t row, coord_t col)
    {
      return (*this)[point2d(row, col)];
    }

  };


} // end of namespace oln


# include <oln/core/2d/fwd_piter2d.hh>
# include <oln/core/2d/bkd_piter2d.hh>


#endif // ! OLENA_CORE_IMAGE2D_HH
