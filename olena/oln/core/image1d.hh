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

# include <oln/core/internal/real_image1d.hh>
# include <oln/core/pred_image.hh>
# include <ntg/bin.hh>
# include <iostream>
# include <stdlib.h>

namespace oln {

  using ntg::bin;
  using ntg::optraits;

  // client can use image1d; instances are real images, that is,
  // images with data ---conversely to proxy images

  template<class T, class Exact = mlc::final>
  class image1d : public internal::_real_image1d< T, typename mlc::exact_vt<image1d<T, Exact>, Exact>::ret >
  {
  public:

    typedef image1d<T, Exact> self;
    typedef internal::_real_image1d< T, typename mlc::exact_vt<image1d<T, Exact>, Exact>::ret > super;

    image1d() :
      super()
    {}

    image1d(coord ncols, coord border = 2) :
      super(ncols, border)
    {}

    image1d(const image1d_size& size, coord border = 2) :
      super(size, border)
    {}

    image1d(self& rhs) : // shallow copy
      super(rhs)
    {}

    self& operator=(self rhs) // shallow assignment
    {
      this->super::operator=(rhs);
      return *this;
    }

    self clone() const // deep copy
    {
      self output(ncols(), this->border());
      _clone_to(output.data());
      return output;
    }

    static std::string name()
    {
      return
	std::string("image1d<")
	+ T::name() + ","
	+ Exact::name() + ">";
    }

    template<class U>
    struct mute
    {
      typedef image1d<U> ret;
    };

    image1d(const self& rhs); // w/o impl
  };

  _ImageForDim(1, image1d)

  // specialization for bin data

  // image1d<bin> is also a pred_image, that is, an image type that
  // can be used as a predicate having the structure of an image


  template<class Exact>
  class image1d<bin, Exact> : public internal::_real_image1d< bin, typename mlc::exact_vt<image1d<bin, Exact>, Exact>::ret >,
			      public pred_image< typename mlc::exact_vt<image1d<bin, Exact>, Exact>::ret >
  {
  public:

    typedef image1d<bin, Exact> self;
    typedef internal::_real_image1d< bin, typename mlc::exact_vt<image1d<bin, Exact>, Exact>::ret > super;
    typedef pred_image< typename mlc::exact_vt<image1d<bin, Exact>, Exact>::ret > super_pred;

    image1d() :
      super()
    {}

    image1d(coord ncols, coord border = 2) :
      super(ncols, border)
    {}

    image1d(const image1d_size& size, coord border = 2) :
      super(size, border)
    {}

    image1d(self& rhs) : // shallow copy
      super(rhs), super_pred()
    {}

    self& operator=(self rhs) // shallow assignment
    {
      super::operator=(rhs);
      return *this;
    }

    self clone() const // deep copy
    {
      self output(ncols(), this->border());
      _clone_to(output.data());
      return output;
    }

    static std::string name()
    {
      return
	std::string("image1d<")
	+ optraits<bin>::name() + ","
	+ Exact::name() + ">";
    }

    template<class U>
    struct mute
    {
      typedef image1d<U> ret;
    };

    image1d(const self& rhs); // w/o impl
  };

} // end of oln

#endif // ! OLENA_CORE_IMAGE1D_HH
