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

/// \file
///
/// \brief Safe convertion function for manipulating Kn-immersed
/// images.

#ifndef MLN_WORLD_KN_SAFE_CAST_HH
# define MLN_WORLD_KN_SAFE_CAST_HH

# include <mln/value/int_u8.hh>
# include <mln/value/intsub.hh>
# include <mln/value/interval.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      using namespace mln::value;

      /// \brief Safe convertion function for manipulating Kn-immersed
      /// images.
      template <typename Tdest, typename Tsrc>
      Tdest safe_cast_to(const Tsrc& src);


    } // end of namespace mln::world::kn

  } // end of namespace mln::world

#ifndef MLN_INCLUDE_ONLY


  namespace value
  {

    template <unsigned n>
    void safe_cast_(const int_u8& from, intsub<n>& to)
    {
      to = from.to_equiv();
    }

    template <unsigned n>
    void safe_cast_(const int_u8& from, interval<intsub<n> >& to)
    {
      to = intsub<n>(from.to_interop());
    }

    inline
    void safe_cast_(const int_u8& from, interval<int_u8>& to)
    {
      to = from;
    }

    inline
    void safe_cast_(const int_u8& from, int& to)
    {
      to = from.to_interop();
    }

    template <unsigned n>
    void safe_cast_(const interval<intsub<n> >& from, int_u8& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = intsub<n>(from.first());
    }

    inline
    void safe_cast_(const interval<int_u8>& from, int_u8& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first();
    }

    template <unsigned n>
    void safe_cast_(const interval<int_u8>& from, intsub<n>& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first().to_interop();
    }

    template <unsigned n>
    void safe_cast_(const interval<intsub<n> >& from, int& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first();
    }

    template <unsigned n>
    void safe_cast_(const interval<intsub<n> >& from, intsub<n>& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first();
    }

    template <unsigned n>
    void safe_cast_(const int& from, interval<intsub<n> >& to)
    {
      to = interval<intsub<n> >(from);
    }

    inline
    void safe_cast_(const int& from, int_u8& to)
    {
      to = from;
    }

    template <unsigned n>
    void safe_cast_(const intsub<n>& from, intsub<2*n>& to)
    {
      to = static_cast<intsub<2*n> >(from.to_int());
    }

    template <unsigned n>
    void safe_cast_(const intsub<n>& from, intsub<n/2>& to)
    {
      to = static_cast<intsub<n/2> >(from.to_int());
    }

    template <unsigned n>
    void safe_cast_(const intsub<n>& from, int_u8& to)
    {
      to = from.to_interop();
    }

  } // end of namespace mln::value


  // Default behavior.
  template <typename T1, typename T2>
  void safe_cast_(const T1& from, T2& to)
  {
    mlc_converts_to(T1,T2)::check();
    to = static_cast<T2>(from);
  }

  // Default behavior.
  template <typename T>
  void safe_cast_(const T& from, T& to)
  {
    to = from;
  }


  namespace world
  {

    namespace kn
    {

      template <typename Tdest, typename Tsrc>
      Tdest safe_cast_to(const Tsrc& src)
      {
	Tdest dest;
	safe_cast_(src, dest);
	return dest;
      }


    } // end of namespace mln::world::kn

  } // end of namespace mln::world


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_WORLD_KN_SAFE_CAST_HH
