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
# include <oln/core/abstract/iter.hh>

# include <sstream>

namespace oln {

  template<unsigned Dim, class T, class Impl, class Exact = mlc::final>
  class image; //fwd_decl

  template<unsigned Dim, class T, class Impl, class Exact>
  struct image_id<image<Dim, T, Impl, Exact> >
  {
    enum{dim = Dim};
    typedef T value_type;
    typedef Impl impl_type;
    typedef typename mlc::exact_vt<image<Dim, T, Impl, Exact>, Exact>::ret exact_type;
  };

  template<unsigned Dim, class T, class Impl, class Exact>
  struct image_traits<image<Dim, T, Impl, Exact> >:
    public image_traits<abstract::image_with_impl<Impl,
						  typename mlc::exact_vt<image<Dim, T, Impl, Exact>, Exact>::ret> >
  {

  };

  // image

  template<unsigned Dim, class T, class Impl, class Exact>
  class image:
    public abstract::image_with_impl<Impl,
				     typename mlc::exact_vt<image<Dim, T, Impl, Exact>, Exact>::ret>
  {
  public:
    typedef typename image_traits<Exact>::point_type point_type;
    typedef typename image_traits<Exact>::dpoint_type dpoint_type;
    typedef typename image_traits<Exact>::iter_type iter_type;
    typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type;
    typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type;
    typedef typename image_traits<Exact>::value_type value_type;
    typedef typename image_traits<Exact>::size_type size_type;
    typedef typename image_traits<Exact>::impl_type impl_type;

    typedef image<Dim, T, Impl, Exact> self_type;
    typedef typename mlc::exact_vt<image<Dim, T, Impl, Exact>, Exact>::ret exact_type;
    typedef typename abstract::image_with_impl<Impl,
					       exact_type> super_type;
    typedef abstract::image<Exact> super_image;


    image() : super_image(), super_type() {}

    image(self_type& rhs): super_image(), super_type(rhs) {}

    static std::string name()
    {
      std::ostringstream s;
      s << "image<" << Dim << ", " << T::name() << ", " << Impl::name() 
	<< ", " << Exact::name() << ">";
      return s.str();
    }

    image(impl_type* i) : super_type(i) {}
  };

  // mute

  template<class I, class T = typename mlc::exact<I>::ret::value_type>
  struct mute
  {
    typedef typename mlc::exact<I>::ret::template mute<T>::ret ret;
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
