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

# include <oln/core/impl/image_array.hh>
// # include <oln/io/readable.hh>
// # include <oln/core/image.hh>
# include <oln/core/abstract/image_with_impl.hh>
# include <iostream>
# include <stdlib.h>

namespace oln {

  template<class T, class E = mlc::final>
  class image1d; // fwd_decl

  template<class T, class E>
  struct image_traits<image1d<T, E> > : public image_traits<abstract::image_with_impl<1, T, oln::impl::image_array1d<T, typename mlc::exact_vt<image1d<T, E>, E>::ret>, typename mlc::exact_vt<image1d<T, E>, E>::ret> >
  {

  };

  // client can use image1d; instances are real images, that is,
  // images with data ---conversely to proxy images

  template<class T, class E>
  class image1d : public abstract::image_with_impl<1, T, oln::impl::image_array1d<T, typename mlc::exact_vt<image1d<T, E>, E>::ret>, typename mlc::exact_vt<image1d<T, E>, E>::ret>
  {
  public:

    typedef image1d<T, E> self_type;
    typedef typename mlc::exact_vt<image1d<T, E>, E>::ret exact_type; 
    typedef oln::impl::image_array1d<T, exact_type> impl_type;
    typedef oln::abstract::image_with_impl<1, T, impl_type, exact_type> super_type;
 
    image1d() :
      super_type()
    {}

    image1d(coord ncols, coord border = 2) :
      super_type(new impl_type(image1d_size(ncols, border)),
		 image1d_size(ncols, border))
    {
      super_type::impl()->ref();
    }

    image1d(const image1d_size& size) :
      super_type(new impl_type(size), size)
    {
      super_type::impl()->ref(); 
    }

    image1d(self_type& rhs) : // shallow copy
      super_type(rhs)
    {}

//     self& operator=(self rhs) // shallow assignment
//     {
//       this->super::operator=(rhs);
//       return *this;
//     }

//     self clone() const // deep copy
//     {
//       self output(ncols(), this->border());
//       _clone_to(output.data());
//       return output;
//     }

//     static std::string name()
//     {
//       return
// 	std::string("image1d<")
// 	+ T::name() + ","
// 	+ E::name() + ">";
//     }

    template<class U>
    struct mute
    {
      typedef image1d<U> ret;
    };

    image1d(const self_type& rhs); // w/o impl
  };


} // end of oln

#endif // ! OLENA_CORE_IMAGE1D_HH
