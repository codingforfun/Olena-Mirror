// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_IMAGE3D_HH
# define OLENA_CORE_IMAGE3D_HH

# include <oln/core/point3d.hh>
# include <oln/core/dpoint3d.hh>
# include <oln/core/fwd_iter3d.hh>
# include <oln/core/bkd_iter3d.hh>
# include <oln/core/impl/image_array3d.hh>
# include <oln/io/readable.hh>
# include <oln/core/image.hh>

# include <iostream>
# include <stdlib.h>

namespace oln {

  template<class T, class Exact = mlc::final>
  class image3d; // fwd_decl
  
  /*! \class image_id<image3d<T, Exact> >
  **
  ** Helper class used by image_traits to retrieve 
  ** the typedef associated to an image.
  */

  template<class T, class Exact>
  struct image_id<image3d<T, Exact> >
  {
    enum{dim = 3};
    typedef T value_type;
    typedef typename mlc::exact_vt<image3d<T, Exact>, Exact>::ret exact_type;
    typedef impl::image_array3d<T> impl_type;
  };
  
  /*! \class image_traits<image3d<T, Exact> >
  **
  ** Helper class usefull to retrieve all the type
  ** relative to an image.
  */
  
  template<class T, class Exact>
  struct image_traits<image3d<T, Exact> >:
    public image_traits<image<image_id<image3d<T, Exact> >::dim,
			      typename image_id<image3d<T, Exact> >::value_type,
			      typename image_id<image3d<T, Exact> >::impl_type,
			      typename image_id<image3d<T, Exact> >::exact_type> >
  {};

  // client can use image3d; instances are real images, that is,
  // images with data ---conversely to proxy images

  /*! \class image3d
  **
  ** To instantiate an image3d with oln::rgb_8 as value_type,
  ** one can write:\n
  ** oln::image3d<ntg::rgb_8> t;
  */
  
  template<class T, class Exact>
  class image3d:
    public image<image_id<image3d<T, Exact> >::dim,
		 typename image_id<image3d<T, Exact> >::value_type,
		 typename image_id<image3d<T, Exact> >::impl_type,
		 typename image_id<image3d<T, Exact> >::exact_type>
  {

  public:

    typedef image3d<T, Exact> self_type;
    typedef typename image_id<image3d<T, Exact> >::value_type value_type;
    typedef typename image_id<image3d<T, Exact> >::exact_type exact_type;
    typedef typename image_id<image3d<T, Exact> >::impl_type impl_type;
    typedef image<image_id<image3d<T, Exact> >::dim,
		  value_type,
		  impl_type,
		  exact_type> super_type;

    friend class abstract::image<exact_type>;

    image3d() :
      super_type()
    {
      mlc_init_static_hierarchy(Exact);
    }

    /*! \brief Allocate memory to contain
    ** an image3d with \a ncols column, 
    ** \a nrows rows, and \a nslices slices
    ** plus a border width equal to 2 by default.
    */
    
    image3d(coord nslices, coord nrows, coord ncols, coord border = 2) :
      super_type(new impl_type(image3d_size(nslices, nrows, ncols, border)))
    {
      mlc_init_static_hierarchy(Exact);
    }
    
    /*! \brief Allocate memory to contain an
    ** image3d with a size equal to \a size.
    */
    
    image3d(const image3d_size& size) :
      super_type(new impl_type(size))
    {
      mlc_init_static_hierarchy(Exact);
    }

    /*! \brief Build a new image3d by performing
    ** a shallow copy of \a rhs, the points are 
    ** not duplicated, but shared between \a rhs
    ** and the new image.
    **
    ** \see abstract::image::clone()
    */
    

    image3d(self_type& rhs) :
      super_type(rhs)
    {
      mlc_init_static_hierarchy(Exact);
    }

    // io
    /*! \brief Perform a shallow copy from \a r to 
    ** the new image, the points are not duplicated,
    ** but shared between the two images.
    **
    ** \see abstract::image::clone()
    */
    
    image3d(const io::internal::anything& r) : super_type()
    {
      mlc_init_static_hierarchy(Exact);
      r.assign(*this);
    }
    
    /*! \brief Perform a shallow copy from \a rhs to
    ** the current image, the points are ot duplicated,
    ** but shared between the two images.
    **
    ** \see abstract::image::clone()
    */

    image3d&
    operator=(const io::internal::anything& r)
    {
      return r.assign(*this);
    }

    /*! \brief Perform a shallow copy from \a r to
    ** the current image, the points are not duplicated
    ** but shared between the two images.
    **
    ** \see abstract::image::clone()
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
	std::string("image3d<")
	+ ntg_name(T) + ","
	+ Exact::name() + ">";
    }

    /// Define ret equal to image3d<U>.
    
    template<class U>
    struct mute
    {
      typedef image3d<U> ret;
    };

    image3d(const self_type& rhs); // w/o impl

  protected:

    /*! \brief Return a deep copy of the current image.
    **
    ** \warning It may be really dangerous to instantiate a self_type
    ** and not an exact_type if Exact != mlc::final.
    **
    ** \todo FIXME: It may be really dangerous to instantiate a self_type
    ** and not an exact_type is Exact != mlc::final.
    */
  
    self_type
    clone_() const // deep copy
    {
      self_type output(this->nslices(), this->nrows(), this->ncols(), this->border());
      clone_to(output.impl());
      return output;
    }

  };

  /*! \class dim_traits<3, T, Exact>
  **
  ** Define img_type equal to image3d<T, Exact>.
  */

  template <class T, class Exact>
  struct dim_traits<3, T, Exact>
  {
    typedef image3d<T, Exact> img_type;
  };
} // end of oln

#endif // ! OLENA_CORE_IMAGE3D_HH
