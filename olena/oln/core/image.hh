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

#ifndef OLENA_CORE_IMAGE_HH
# define OLENA_CORE_IMAGE_HH

# include <mlc/type.hh>
# include <oln/core/abstract/image_with_impl.hh>

namespace oln {

  template<unsigned Dim, class T, class Impl, class E>
  class image; //fwd_decl

  template<unsigned Dim, class T, class Impl, class E>
  struct image_traits<image<Dim, T, Impl, E> >: public image_traits<abstract::image_with_impl<Dim, T, Impl, E> >
  {
    
  };

  // image

  template<unsigned Dim, class T, class Impl, class E>
  class image : public abstract::image_with_impl<Dim, T, Impl, E>
  {
  public:
    typedef typename image_traits<E>::point_type point_type;
    typedef typename image_traits<E>::iter_type iter_type;
    typedef typename image_traits<E>::fwd_iter_type fwd_iter_type;
    typedef typename image_traits<E>::bkd_iter_type bkd_iter_type;
    typedef typename image_traits<E>::value_type value_type;
    typedef typename image_traits<E>::size_type size_type;
    typedef typename image_traits<E>::impl_type impl_type;

    typedef image<Dim, T, Impl, E> self_type;
    typedef E exact_type;
    typedef typename abstract::image_with_impl<Dim, T, Impl, E> super_type;

    static std::string name() { return std::string("image<") + Dim + ", " + T::name() + ", " + Impl::name() + ", " +  E::name() + ">"; }
    image(impl_type* i) : super_type(i) {}    
  };

  // mute

  template<class I, class T = typename mlc::exact<I>::ret::value>
  struct mute
  {
    typedef typename mlc::exact<I>::ret::mute<T>::ret ret;
  };


# define Value(ImgType)				\
Exact(ImgType)::value_type

# define Concrete(ImgType)			\
typename mute<ImgType>::ret

# define Iter(Iterable)				\
Exact(Iterable)::iter_type

# define Point(Pointable)			\
Exact(Pointable)::point_type

# define DPoint(DPointable)			\
Exact(DPointable)::dpoint_type

} // end of oln

#endif // ! OLENA_CORE_IMAGE_HH
