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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_BY_DELEGATION_HH
# define OLENA_CORE_ABSTRACT_IMAGE_BY_DELEGATION_HH

# include <oln/core/box.hh>
# include <oln/core/abstract/image_entry.hh>


namespace oln {


  // fwd decl
  namespace abstract {
    template <typename I, typename E> struct image_by_delegation;
  }

  // super type

  template <typename I, typename E>
  struct set_super_type < abstract::image_by_delegation<      I, E> > { typedef abstract::image_entry<E> ret; };
  template <typename I, typename E>
  struct set_super_type < abstract::image_by_delegation<const I, E> > { typedef abstract::image_entry<E> ret; };

  // props

  template <typename I, typename E>
  struct set_props < category::image, abstract::image_by_delegation<I, E> >
    : public get_props< category::image, I >
  {
    typedef I delegated_type;
  };

  template <typename I, typename E>
  struct set_props < category::image, abstract::image_by_delegation<const I, E> >
    : public get_props< category::image, I >
  {
    typedef I delegated_type;
  };




  namespace abstract {

    /// Mutable version of image_by_delegation

    template <typename I, typename E>
    struct image_by_delegation : public image_entry<E>
    {
    protected:

      image_by_delegation ()
      {
 	this->exact_ptr = (E*)(void*)(this);
      }

      image_by_delegation(abstract::image<I>& image) :
	image_(image.exact())
      {
	this->exact_ptr = (E*)(void*)(this);
      }

      image_by_delegation(const image_by_delegation& rhs) :
	image_(rhs.image_)
      {
	this->exact_ptr = (E*)(void*)(this);
      }

      box<I> image_;

    public:

      I& image() { return this->image_.unbox(); }
      I& impl_delegate() { return this->image_.unbox(); }
      const I& impl_delegate() const { return this->image_.unbox(); }
    };

    /// Const version of image_by_delegation

    template <typename I, typename E>
    struct image_by_delegation<const I, E> : public image_entry<E>
    {
    protected:

      image_by_delegation()
      {
	this->exact_ptr = (E*)(void*)(this);
      }

      image_by_delegation(const abstract::image<I>& ima) :
	image_(ima.exact())
      {
	this->exact_ptr = (E*)(void*)(this);
      }

      image_by_delegation(const image_by_delegation& rhs) :
	image_(rhs.image())
      {
	this->exact_ptr = (E*)(void*)(this);
      }

      box<const I> image_;

    public:

      const I& image() const { return this->image_.unbox(); }
      const I& impl_delegate() const { return this->image_.unbox(); }
    };

  }

}


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_BY_DELEGATION_HH
