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

#ifndef PROTO_OLN_CORE_ABSTRACT_INTERNAL_IMAGE_IMPL_HH
# define PROTO_OLN_CORE_ABSTRACT_INTERNAL_IMAGE_IMPL_HH

# include <mlc/any.hh>

# include <oln/core/properties.hh>


namespace oln {

  namespace abstract {

    namespace internal {

      // FIXME: doc!

      template <typename A, typename D, typename E>
      struct get_image_impl_helper;

      // entry point:
      template <typename A, typename E>
      struct get_image_impl : public get_image_impl_helper <A, oln_type_of(E, delegated), E>
      {
      };

      template <typename A, typename E>
      struct get_image_impl_helper <A, mlc::no_type, E> : public virtual mlc::any__best_speed<E>
      {
	// no impl
      };

      template <typename A, typename E>
      struct set_image_impl; // to be specialized...

      template <typename A, typename D, typename E>
      struct get_image_impl_helper : public set_image_impl <A, E>
      {
	// impl comes from  internal::set_image_impl <A, E>
      };

      template <typename E>
      struct image_impl : public mlc::any__best_speed<E>
      {
	typedef oln_type_of(E, delegated) D;
	D& delegate() { return this->exact().impl_delegate(); }
	const D& delegate() const { return this->exact().impl_delegate(); }
      };

    } // end of namespace internal

  } // end of namespace abstract

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_ABSTRACT_IMAGE_HH
