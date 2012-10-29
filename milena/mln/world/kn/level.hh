// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_WORLD_KN_LEVEL_HH
# define MLN_WORLD_KN_LEVEL_HH

# include <mln/core/concept/value.hh>
# include <mln/value/interval.hh>
# include <mln/data/transform.hh>
# include <mln/fun/v2b/threshold_lt.hh>
# include <mln/fun/v2b/threshold_le.hh>
# include <mln/fun/v2b/threshold_gt.hh>
# include <mln/fun/v2b/threshold_ge.hh>
# include <mln/fun/v2b/compare.hh>

/// \file
///
/// Definition of a level tag.

namespace mln
{

  namespace world
  {

    namespace kn
    {

      /// \brief Level tag structure.
      template <typename T>
      struct level_t
	: public Object<level_t<T> >
      {
	const T& value;
	explicit level_t(const T& value);
      };

      /// \brief construct a level tag.
      template <typename T>
      level_t<T> level(const T& value);


    } // end of namespace mln::world::kn

  } // end of namespace mln::world

  namespace value
  {
      // Comparison operators with intervals

      template <typename T, typename U>
      bool
      operator<=(const interval<T>& i, const world::kn::level_t<U>& v);

      template <typename T, typename U>
      bool
      operator>(const interval<T>& i, const world::kn::level_t<U>& v);

      template <typename T, typename U>
      bool
      operator>=(const interval<T>& i, const world::kn::level_t<U>& v);

      template <typename T, typename U>
      bool
      operator<(const interval<T>& i, const world::kn::level_t<U>& v);

      template <typename T, typename U>
      bool
      operator==(const interval<T>& i, const world::kn::level_t<U>& v);

  } // end of namespace mln::value


  // Comparison operators with images.

  template <typename I, typename U>
  mln_ch_value(I,bool)
    operator<(const Image<I>& ima, const world::kn::level_t<U>& v);

  template <typename I, typename U>
  mln_ch_value(I,bool)
    operator==(const Image<I>& ima, const world::kn::level_t<U>& v);


  template <typename I, typename U>
  mln_ch_value(I,bool)
    operator>(const Image<I>& ima, const world::kn::level_t<U>& v);

  template <typename I, typename U>
  mln_ch_value(I,bool)
    operator>=(const Image<I>& ima, const world::kn::level_t<U>& v);


# ifndef MLN_INCLUDE_ONLY

  namespace world
  {

    namespace kn
    {

      template <typename T>
      level_t<T>::level_t(const T& value)
      : value(value)
      {
      }

      // Helpers

      template <typename T>
      inline
      level_t<T> level(const T& value)
      {
	return level_t<T>(value);
      }

    } // end of namespace mln::world::kn

  } // end of namespace mln::world


  namespace value
  {

    // Comparison operators with intervals

    template <typename T, typename U>
    inline
    bool
    operator<=(const interval<T>& i, const world::kn::level_t<U>& v)
    {
      return i.first() <= v.value;
    }

    template <typename T, typename U>
    inline
    bool
    operator>(const interval<T>& i, const world::kn::level_t<U>& v)
    {
      return ! (i <= v);
    }

    template <typename T, typename U>
    inline
    bool
    operator>=(const interval<T>& i, const world::kn::level_t<U>& v)
    {
      return v.value <= i.last();
    }

    template <typename T, typename U>
    inline
    bool
    operator<(const interval<T>& i, const world::kn::level_t<U>& v)
    {
      return ! (i >= v);
    }

    template <typename T, typename U>
    inline
    bool
    operator==(const interval<T>& i, const world::kn::level_t<U>& v)
    {
      return i >= v && i <= v;
    }

  } // end of namespace mln::value


  // Comparison Operators with images

  template <typename I, typename U>
  mln_ch_value(I,bool)
    operator<(const Image<I>& ima, const world::kn::level_t<U>& v)
  {
    typedef world::kn::level_t<U> T;
    return data::transform(ima, fun::v2b::threshold_lt<mln_value(I),T>(v));
  }

  template <typename I, typename U>
  mln_ch_value(I,bool)
    operator<=(const Image<I>& ima, const world::kn::level_t<U>& v)
  {
    typedef world::kn::level_t<U> T;
    return data::transform(ima, fun::v2b::threshold_le<mln_value(I),T>(v));
  }

  template <typename I, typename U>
  mln_ch_value(I,bool)
    operator>(const Image<I>& ima, const world::kn::level_t<U>& v)
  {
    typedef world::kn::level_t<U> T;
    return data::transform(ima, fun::v2b::threshold_gt<mln_value(I),T>(v));
  }

  template <typename I, typename U>
  mln_ch_value(I,bool)
    operator>=(const Image<I>& ima, const world::kn::level_t<U>& v)
  {
    typedef world::kn::level_t<U> T;
    return data::transform(ima, fun::v2b::threshold_ge<mln_value(I),T>(v));
  }

  template <typename I, typename U>
  mln_ch_value(I,bool)
    operator==(const Image<I>& ima, const world::kn::level_t<U>& v)
  {
    typedef world::kn::level_t<U> T;
    return data::transform(ima, fun::v2b::compare<mln_value(I),T>(v));
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_WORLD_KN_LEVEL_HH
