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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_WITH_EXTENSION__HH
# define OLENA_CORE_ABSTRACT_IMAGE_WITH_EXTENSION__HH

# include <oln/core/abstract/image_by_delegation.hh>


namespace oln {


  // fwd decl
  namespace abstract {
    template <typename I, typename E> struct image_with_extension_;
  }

  // category

  template <typename I, typename E>
  struct set_category< abstract::image_with_extension_<I, E> > {
    typedef category::image ret;
  };

  // super type

  template <typename I, typename E>
  struct set_super_type < abstract::image_with_extension_<I, E> >
  {
    typedef abstract::image_by_delegation<I, E> ret;
  };

  // props

  template <typename I, typename E>
  struct set_props < category::image, abstract::image_with_extension_<I, E> > : public props_of<category::image>
  {
    typedef oln_type_of(I, concrete) concrete_type;
  };


  namespace abstract {

    /// Mutable version of image_with_extension_.

    template <typename I, typename E>
    struct image_with_extension_ : public image_by_delegation<I, E>
    {
      typedef image_by_delegation<I, E> super_type;

    public:

      image_with_extension_ ()
      {
      }

      image_with_extension_(abstract::image<I>& image) :
	super_type(image)
      {
      }

      const I& real() const
      {
	return this->image_.unbox();
      }

      I& real()
      {
	return this->image_.unbox();
      }

      ~image_with_extension_ ()
      {
	// FIXME : This class must check all properties

// 	mlc_or( mlc_eq(oln_type_of(I,neighb_type), mlc::no_type),
// 		mlc_eq(oln_type_of(I,neighb_type), oln_type_of(E,neighb_type))
// 		)::ensure();

	// if (Prop(I) != no_type)
	//   Prop(E) = Prop(I)
      }


    };

  } // end of namespace oln::abstract



} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_WITH_EXTENSION__HH
