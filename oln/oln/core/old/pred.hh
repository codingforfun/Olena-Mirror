// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_PRED_IMAGE_HH
# define OLENA_CORE_PRED_IMAGE_HH

# include <oln/core/image.hh>

namespace oln {


  // here, a Predicate is something like:
  template<class Image,
	   class Predicate,
	   class Inferior = type::bottom>
  class pred_image :
    public image< pred_image<Image,Predicate,Inferior> >,
    public predicate< pred_image<Image,Predicate,Inferior> >
  {
  public:
    typedef Inferior inferior;

    enum { dim = Image::dim };
    typedef bin value;
    typedef typename Image::point point;
    typedef typename Image::dpoint dpoint;
    typedef typename Image::fwd_iter iter;
    typedef typename Image::fwd_iter fwd_iter;
    typedef typename Image::bkd_iter bkd_iter;
    typedef typename Image::image_size image_size;

    pred_image(const Image& image) :
      _image(image)
    {
    }

    image_size size() const { return _image.size(); }
    size_t npoints() const { return _image.npoints(); }

    bool hold(const point& p) const { return _image.hold(p); }

    const bin operator[](const point& p) const { return _pred(p, _image); }

    static std::string name()
    {
      return std::string("pred_image< ")
	+ Image::name() + ", "
	+ "a_predicate, "
	+ Inferior::name() + " >";
    }

    template<class U>
    struct mute {
      typedef typename Image::mute<U>::ret ret;
    };

  private:

    const Image& _image;
    Predicate _pred;
  };




  template<template<class> class Predicate,
	   class Image>
  pred_image< Image, Predicate<typename Image::value> >&
  pred(const Image& input)
  {
    return *new pred_image< Image, Predicate<typename Image::value> >(input);
    // FIXME: memory leak
  }

  template<class Predicate,
	   class Image>
  pred_image< Image, Predicate >&
  pred(const Image& input)
  {
    return *new pred_image< Image, Predicate >(input);
    // FIXME: memory leak
  }


} // end of oln



#endif // ! OLENA_CORE_PRED_IMAGE_HH
