// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_IMAGE1D_HH
# define OLENA_CORE_IMAGE1D_HH

# include <oln/core/point1d.hh>
# include <oln/core/dpoint1d.hh>
# include <oln/core/fwd_iter1d.hh>
# include <oln/core/bkd_iter1d.hh>
# include <oln/core/impl/image_array1d.hh>
# include <oln/core/image.hh>
# include <oln/io/readable.hh>

# include <iostream>
# include <stdlib.h>

namespace oln {

  template<class T, class Exact = mlc::final>
  class image1d; // fwd_decl

  template<class T, class Exact>
  struct image_id<image1d<T, Exact> >
  {
    enum{dim = 1};
    typedef T value_type;
    typedef typename mlc::exact_vt<image1d<T, Exact>, Exact>::ret exact_type;
    typedef impl::image_array1d<T> impl_type;
  };

  template<class T, class Exact>
  struct image_traits<image1d<T, Exact> >: 
    public image_traits<image<image_id<image1d<T, Exact> >::dim, 
			      typename image_id<image1d<T, Exact> >::value_type,
			      typename image_id<image1d<T, Exact> >::impl_type, 
			      typename image_id<image1d<T, Exact> >::exact_type> >
  {

  };

  // client can use image1d; instances are real images, that is,
  // images with data ---conversely to proxy images

  template<class T, class Exact>
  class image1d: 
    public image<image_id<image1d<T, Exact> >::dim,
		 typename image_id<image1d<T, Exact> >::value_type, 
		 typename image_id<image1d<T, Exact> >::impl_type, 
		 typename image_id<image1d<T, Exact> >::exact_type>
  {
  public:

    typedef image1d<T, Exact> self_type;
    typedef typename image_id<image1d<T, Exact> >::value_type value_type;
    typedef typename image_id<image1d<T, Exact> >::exact_type exact_type; 
    typedef typename image_id<image1d<T, Exact> >::impl_type impl_type;
    typedef image<image_id<image1d<T, Exact> >::dim, 
		  value_type,
		  impl_type, 
		  exact_type> super_type;

    friend class abstract::image<exact_type>;

    image1d() : 
      super_type()
    {
      mlc_init_static_hierarchy(Exact);
    }

    image1d(coord ncols, coord border = 2) : 
      super_type(new impl_type(image1d_size(ncols, border)))
    {
      mlc_init_static_hierarchy(Exact);
    }

    image1d(const image1d_size& size) : 
      super_type(new impl_type(size))
    {
      mlc_init_static_hierarchy(Exact);
    }

    image1d(self_type& rhs) : // shallow copy
      super_type(rhs)
    { mlc_init_static_hierarchy(Exact); }

    exact_type& operator=(self_type rhs)
    {
      return this->exact().assign(rhs.exact());
    }

    // io
    image1d(const io::internal::anything& r) 
      : super_type()
    {
      mlc_init_static_hierarchy(Exact);
      r.assign(*this);
    }

    image1d& operator=(const io::internal::anything& r)
    {
      return r.assign(*this);
    }

    static std::string name()
    {
      return
	std::string("image1d<")
	+ ntg_name(T) + ","
	+ Exact::name() + ">";
    }

    template<class U>
    struct mute
    {
      typedef image1d<U> ret;
    };

    image1d(const self_type& rhs); // w/o impl

  protected:
    self_type clone_() const // deep copy
    {
      // FIXME: it may be really dangerous to instantiate a self_type
      // and not an exact_type is Exact != mlc::final.
      self_type output(this->ncols(), this->border());
      clone_to(output.impl());
      return output;
    }
  };


} // end of oln

#endif // ! OLENA_CORE_IMAGE1D_HH
