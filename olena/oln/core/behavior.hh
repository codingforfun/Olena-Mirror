// Copyright (C) 2004 EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_CORE_BEHAVIOR_HH
# define OLENA_CORE_BEHAVIOR_HH
# include <oln/core/abstract/behavior.hh>
# include <mlc/type.hh>

namespace oln {
  /*! \class mirror_behavior
  **
  ** Make the border be a mirror of the image.
  **
  ** \see abstract::image_size::border_
  */
  // mirror the image content into the border
  template <class Exact = mlc::final>
  class mirror_behavior:
    public abstract::behavior<mlc_exact_vt_type(mirror_behavior<Exact>, Exact)>
  {
  public:
    typedef mirror_behavior<Exact>			self_type;
    typedef mlc_exact_vt_type(self_type, Exact)	exact_type;

    template <class I>
      void adapt_border_impl(const oln::abstract::image<I> &im,
			     coord border_size) const
      {
	im.border_adapt_mirror(border_size);
      }
  };

  // set the border to a specific value

  /*! \class value_behavior
  **
  ** Set the border to a specific value.
  **
  ** \see abstract::image_size::border_
  */
  template <class T, class Exact = mlc::final>
  class value_behavior:
    public abstract::behavior<mlc_2_exact_vt_type(value_behavior, T, Exact)>
  {
  public:
    typedef value_behavior<T, Exact>		self_type;
    typedef mlc_exact_vt_type(self_type, Exact)	exact_type;
    typedef T					value_type;

    explicit value_behavior(value_type value): value_(value)
      {
      };

      template <class I>
      void adapt_border_impl(const abstract::image<I> &im,
			     coord border_size) const
      {
	im.border_adapt_assign(border_size,
			       ntg::cast::force<oln_value_type(I)>(value_));
      }

      /// Empty constructor for any_with_diamond hierarchy.
      value_behavior() {}

  protected:
    value_type	value_;
  };

  // replicate the border

  /*! \brief replicate_behavior
  **
  ** Replicate the border.
  **
  ** \see abstract::image_size::border_
  */
  template <class Exact = mlc::final>
  class replicate_behavior:
    public abstract::behavior<mlc_exact_vt_type(replicate_behavior<Exact>, Exact)>
  {
  public:
    typedef replicate_behavior<Exact>		self_type;
    typedef mlc_exact_vt_type(self_type, Exact)	exact_type;

    template <class I>
      void adapt_border_impl(const abstract::image<I> &im,
			     coord border_size) const
      {
	im.border_adapt_copy(border_size);
      }

  };

  // tools to call ctors with type inference
  /// To call Ctors with type inference.
  inline mirror_behavior<> mirror_bhv()
  {
    return mirror_behavior<>();
  }

  /// To call Ctors with type inference.
  template <class T>
  inline value_behavior<T> value_bhv(const T &value)
  {
    return value_behavior<T>(value);
  }

  /// To call Ctors with type inference.
  inline replicate_behavior<> replicate_bhv()
  {
    return replicate_behavior<>();
  }
} // !oln

#endif // !OLN_CORE_BEHAVIOR_HH
