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

#ifndef OLENA_CORE_GEN_IDENTITY_HH
# define OLENA_CORE_GEN_IDENTITY_HH

# include <oln/core/abstract/image_like_.hh>


namespace oln {


  // fwd decls

  namespace internal {
    template <typename I> struct image_identity;
  }

  template <typename I>
  internal::image_identity<I> identity(abstract::image<I>& ima);



  // category

  template <typename I>
  struct set_category< internal::image_identity<I> > {
    typedef category::image ret;
  };

  // super type

  template <typename I>
  struct set_super_type < internal::image_identity<I> >
  {
    typedef abstract::image_like_< I, internal::image_identity<I> > ret;
  };


  namespace internal
  {

    template <typename I>
    struct image_identity : public abstract::image_like_< I, image_identity<I> >
    {
      typedef image_identity<I> self_type;
      typedef abstract::image_like_< I, self_type > super_type;

    public:

      image_identity(abstract::image<I>& rhs) :
	super_type(rhs)
      {
      }

    };

  } // end of namespace oln::internal



  /// the 'identity' method for images

  template <typename I>
  internal::image_identity<I> identity(abstract::image<I>& ima)
  {
    internal::image_identity<I> tmp(ima);
    return tmp;
  }


} // end of namespace oln


#endif // ! OLENA_CORE_GEN_IDENTITY_HH
