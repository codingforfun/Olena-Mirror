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

#ifndef OLENA_CORE_PW_VALUE_HH
# define OLENA_CORE_PW_VALUE_HH

# include <oln/core/box.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/pw/abstract/function.hh>


namespace oln {


  // fwd decl
  namespace pw {
    template <typename I> struct value;
  }

  // super type
  template <typename I>
  struct set_super_type < pw::value<I> > { typedef pw::abstract::function< pw::value<I> > ret; };

  // props
  template <typename I>
  struct set_props < category::pw, pw::value<I> >
  {
    typedef oln_type_of(I, point) point_type;
    typedef oln_type_of(I, value) value_type;
    typedef oln_type_of(I, size)  size_type;
  };


  namespace pw { // means "point-wise"

    template <typename I>
    struct value : public pw::abstract::function < value<I> >
    {
      oln::box<const I> ima;

      value(const oln::abstract::image<I>& ima) :
	ima(ima)
      {
      }

      typedef oln_type_of(I, point) point_type;
      typedef oln_type_of(I, value) value_type;
      typedef oln_type_of(I, size)  size_type;

      const size_type& impl_size() const
      {
	return this->ima.size();
      }

      const value_type impl_get(const point_type& p) const
      {
	return this->ima.get(p);
      }

      bool impl_hold(const point_type& p) const
      {
	return this->ima.hold(p);
      }

      bool impl_hold_large(const point_type& p) const
      {
	return this->ima.hold_large(p);
      }

    };

  } // end of namespace oln::pw


  /// Routine that takes an image and outputs a "point value" object

  template <typename I>
  pw::value<I> pw_value(const abstract::image<I>& ima)
  {
    pw::value<I> tmp(ima);
    return tmp;
  }


  /// Specializations of pw_value to produce compile-time errors.
  // FIXME: struct OLENA_ERROR__arg_of__pw_value__should_be_a_image();

  template <typename P>
  void pw_value(const abstract::point<P>&)
  {}

  // FIXME: add abstract::piter, etc.


} // end of namespace oln



#endif // ! OLENA_CORE_PW_VALUE_HH
