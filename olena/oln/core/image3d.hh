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

#ifndef OLENA_CORE_IMAGE3D_HH
# define OLENA_CORE_IMAGE3D_HH

# include <oln/core/internal/real_image3d.hh>
# include <oln/core/pred_image.hh>
# include <ntg/bin.hh>
# include <oln/io/readable.hh>
# include <iostream>
# include <stdlib.h>

namespace oln {

  using ntg::bin;
  using ntg::optraits;

  // client can use image3d; instances are real images, that is,
  // images with data ---conversely to proxy images


  template<class T, class Exact = mlc::final>
  class image3d : public internal::_real_image3d< T, typename mlc::exact_vt<image3d<T, Exact>, Exact>::ret>
  {
  public:

    typedef image3d<T, Exact> self;
    typedef internal::_real_image3d< T, typename mlc::exact_vt<image3d<T, Exact>, Exact>::ret> super;

    image3d() :
      super()
    {}

    image3d(coord nslices, coord nrows, coord ncols, coord border = 2) :
      super(nslices, nrows, ncols, border)
    {}

    image3d(const image3d_size& size, coord border = 2) :
      super(size, border)
    {}

    image3d(self& rhs) : // shallow copy
      super(rhs)
    {}

    self& operator=(self rhs) // shallow assignment
    {
      this->super::operator=(rhs);
      return *this;
    }

    // io
    image3d(const io::internal::anything& r) : super()
    {
      r.assign(*this);
    }
    image3d& operator=(const io::internal::anything& r)
    {
      return r.assign(*this);
    }

    self clone() const // deep copy
    {
      self output(nslices(), nrows(), ncols(), this->border());
      _clone_to(output.data());
      return output;
    }

    static std::string name()
    {
      return
	std::string("image3d<")
	+ T::name() + ","
	+ Exact::name() + ">";
    }

    template<class U>
    struct mute
    {
      typedef image3d<U> ret;
    };

    image3d(const self& rhs); // w/o impl
  };

  _ImageForDim(3, image3d)


  // specialization for bin data

  // image3d<bin> is also a pred_image, that is, an image type that
  // can be used as a predicate having the structure of an image


  template<class Exact>
  class image3d<bin, Exact> : public internal::_real_image3d< bin, typename mlc::exact_vt<image3d<bin, Exact>, Exact>::ret >,
			      public pred_image<typename mlc::exact_vt<image3d<bin, Exact>, Exact>::ret >
  {
  public:

    typedef image3d<bin, Exact> self;
    typedef internal::_real_image3d< bin, typename mlc::exact_vt<image3d<bin, Exact>, Exact>::ret > super;
    typedef pred_image<typename mlc::exact_vt<image3d<bin, Exact>, Exact>::ret > super_pred;

    image3d() :
      super()
    {}

    image3d(coord nslices, coord nrows, coord ncols, coord border = 2) :
      super(nslices, nrows, ncols, border)
    {}

    image3d(const image3d_size& size, coord border = 2) :
      super(size, border)
    {}

    image3d(self& rhs) : // shallow copy
      super(rhs), super_pred()
    {}

    self& operator=(self rhs) // shallow assignment
    {
      super::operator=(rhs);
      return *this;
    }

    // io
    image3d(const io::internal::anything& r) : super()
    {
      r.assign(*this);
    }
    image3d& operator=(const io::internal::anything& r)
    {
      return r.assign(*this);
    }

    self clone() const // deep copy
    {
      self output(nslices(), nrows(), ncols(), this->border());
      _clone_to(output.data());
      return output;
    }

    static std::string name()
    {
      return
	std::string("image3d<")
	+ optraits<bin>::name() + ","
	+ Exact::name() + ">";
    }

    template<class U>
    struct mute
    {
      typedef image3d<U> ret;
    };

    image3d(const self& rhs); // w/o impl
  };



} // end of oln


#endif // ! OLENA_CORE_IMAGE3D_HH
