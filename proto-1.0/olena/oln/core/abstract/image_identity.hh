// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_IDENTITY_HH
# define OLENA_CORE_ABSTRACT_IMAGE_IDENTITY_HH

# include <mlc/box.hh>

# include <oln/core/entry.hh>

namespace oln {

  namespace abstract {

    template <typename I, typename E>
    struct image_identity: public abstract::image_entry<E>
    {
    protected:

      image_identity () {}

      image_identity(abstract::image<I>& image) : image_(image.exact())
      {}

      image_identity(const image_identity& rhs) : image_(rhs.image())
      {
	this->exact_ptr = (E*)(void*)(this);
      }

      mlc::box<I> image_;

    public:

      I& image () const
      {
	return const_cast<I&>(*image_);
      }

      I& impl_delegate() { return *image_; }
      const I& impl_delegate() const { return *image_; }
    };


    template <typename I, typename E>
    struct image_identity<const I, E>: public abstract::image_entry<E>
    {
    protected:

      image_identity() {}

      image_identity(const abstract::image<I>& image_) : image_(image.exact())
      {}

      image_identity(const image_identity& rhs) : image_(rhs.image())
      {
	this->exact_ptr = (E*)(void*)(this);
      }

      mlc::box<const I> image_;

    public:
      const I& image () const
      {
	return *image_;
      }

      I& impl_delegate() { return *image_; }
      const I& impl_delegate() const { return *image_; }
    };

  }

}


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_IDENTITY_HH
