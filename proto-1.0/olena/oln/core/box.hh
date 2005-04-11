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

#ifndef OLENA_CORE_BOX_HH
# define OLENA_CORE_BOX_HH

# include <oln/core/abstract/image_entry.hh>

namespace oln {


  // fwd decl
  template <typename I> struct box;

  // category

  template <typename I>
  struct set_category< box<I> > {
    typedef category::image ret;
  };

  template <typename I>
  struct set_category< box<const I> > {
    typedef category::image ret;
  };

  // props

  template <typename I>
  struct set_props < category::image, box<I> >
    : public get_props< category::image, I >
  {
    typedef I delegated_type;
  };

  template <typename I>
  struct set_props < category::image, box<const I> >
    : public get_props< category::image, I >
  {
    typedef I delegated_type;
  };


  /// Class oln::box...

  template <typename I>
  struct box : public abstract::image_entry< box<I> >
  {
  public:

    box () :
      image_()
    {
      this->exact_ptr = this;
    }

    box(abstract::image<I>& ima) :
      image_(ima.exact())
    {
      this->exact_ptr = this;
    }

    box<I>& operator=(abstract::image<I>& rhs)
    {
      this->image_ = rhs.exact();
      return *this;
    }

    box(const box<I>& rhs) :
      image_(const_cast<I&>(rhs.image_))
    {
      this->exact_ptr = this;
    }

    box<I> operator=(const box<I>& rhs)
    {
      this->image_ = const_cast<I&>(rhs.image_);
      return *this;
    }

    // FIXME: add versions for I2 (neq I) to produce explicit errors

    I& impl_delegate()
    {
      return this->image_;
    }

    const I& impl_delegate() const
    {
      return this->image_;
    }

    /// Hooks.

    I& unbox()
    {
      return this->image_;
    }

    const I& unbox() const
    {
      return this->image_;
    }

  private:

    /// attribute
    mutable I image_;

  };


  template <typename I>
  struct box<const I> : public abstract::image_entry< box<const I> >
  {
  public:

    box() :
      image_()
    {
      this->exact_ptr = this;
    }

    box(const abstract::image<I>& ima) :
      image_(const_cast<I&>(ima.exact()))
    {
      this->exact_ptr = this;
    }

    box<const I> operator=(const abstract::image<I>& rhs)
    {
      this->image_ = const_cast<I&>(rhs.exact());
      return *this;
    }

    /// Classical copy ctor (image remains const).
    box(const box<const I>& rhs) :
      image_(const_cast<I&>(rhs.image_))
    {
      this->exact_ptr = this;
    }

    /// Classical op= (image remains const).
    box<const I> operator=(const box<const I>& rhs)
    {
      this->image_ = const_cast<I&>(rhs.image_);
      return *this;
    }

    /// Copy ctor with promotion (image becomes const).
    box(const box<I>& rhs) :
      image_(const_cast<I&>(rhs.image_))
    {
      this->exact_ptr = this;
    }

    /// Op= with promotion (image becomes const).
    box<const I> operator=(const box<I>& rhs)
    {
      this->image_ = const_cast<I&>(rhs.image_);
      return *this;
    }

    /// Delegation relies on a constant image. 
    const I& impl_delegate() const
    {
      return this->image_;
    }

    /// Hook.

    const I& unbox() const
    {
      return this->image_;
    }

  private:

    /// attribute
    mutable I image_;

  };

}


#endif // ! OLENA_CORE_BOX_HH
