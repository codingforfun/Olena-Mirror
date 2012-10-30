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

# include <mln/value/int_u.hh>
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

    // From int_u<n>

    template <unsigned m, unsigned n>
    void safe_cast_(const int_u<m>& from, intsub<n>& to)
    {
      to = from.to_interop();
    }

    template <unsigned m, unsigned n>
    void safe_cast_(const int_u<m>& from, interval<intsub<n> >& to)
    {
      to = intsub<n>(from.to_interop());
    }

    template <unsigned m>
    void safe_cast_(const int_u<m>& from, interval<int_u<m> >& to)
    {
      to = from;
    }

    template <unsigned m>
    void safe_cast_(const int_u<m>& from, interval<int>& to)
    {
      to = from;
    }

    template <unsigned m>
    void safe_cast_(const int_u<m>& from, int& to)
    {
      to = from.to_interop();
    }

    template <unsigned m>
    void safe_cast_(const int_u<m>& from, unsigned& to)
    {
      to = from.to_interop();
    }

    // From interval<int>

    void safe_cast_(const interval<int>& from, int& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first();
    }

    void safe_cast_(const interval<int>& from, unsigned& to)
    {
      if (!from.is_degenerated() || from.first() < 0)
	std::abort();
      to = from.first();
    }

    template <unsigned m>
    void safe_cast_(const interval<int>& from, int_u<m>& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first();
    }

    template <unsigned n>
    void safe_cast_(const interval<int>& from, intsub<n>& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first();
    }

    // From interval< int_u<m> >

    template <unsigned m>
    void safe_cast_(const interval<int_u<m> >& from, int_u<m>& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first();
    }

    template <unsigned m, unsigned n>
    void safe_cast_(const interval<int_u<m> >& from, intsub<n>& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first().to_interop();
    }

    template <unsigned m>
    void safe_cast_(const interval<int_u<m> >& from, int& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first().to_interop();
    }

    template <unsigned m>
    void safe_cast_(const interval<int_u<m> >& from, unsigned& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first().to_interop();
    }

    // From interval<intsub<n> >

    template <unsigned n, unsigned m>
    void safe_cast_(const interval<intsub<n> >& from, int_u<m>& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = intsub<n>(from.first());
    }

    template <unsigned n>
    void safe_cast_(const interval<intsub<n> >& from, int& to)
    {
      if (!from.is_degenerated())
	std::abort();
      to = from.first();
    }

    template <unsigned n>
    void safe_cast_(const interval<intsub<n> >& from, unsigned& to)
    {
      if (!from.is_degenerated() || from.first() < 0)
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

    // From int

    template <unsigned n>
    void safe_cast_(const int& from, interval<intsub<n> >& to)
    {
      to = interval<intsub<n> >(from);
    }

    void safe_cast_(const int& from, interval<int>& to)
    {
      to = interval<int>(from);
    }

    void safe_cast_(const int& from, unsigned& to)
    {
      if (from < 0)
	std::abort();
      to = from;
    }

    template <unsigned m>
    void safe_cast_(const int& from, int_u<m>& to)
    {
      to = from;
    }

    template <unsigned n>
    void safe_cast_(const int& from, intsub<n>& to)
    {
      to = from;
    }

    // From intsub<n>

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

    template <unsigned n, unsigned m>
    void safe_cast_(const intsub<n>& from, int_u<m>& to)
    {
      to = from.to_interop();
    }

    template <unsigned n>
    void safe_cast_(const intsub<n>& from, int& to)
    {
      to = from;
    }

    template <unsigned n>
    void safe_cast_(const intsub<n>& from, unsigned& to)
    {
      if (from < 0)
	std::abort();
      to = from;
    }

    template <unsigned n, unsigned m>
    void safe_cast_(const intsub<n>& from, interval<int_u<m> >& to)
    {
      to = interval<int_u<m> >(from.to_interop());
    }

    template <unsigned n>
    void safe_cast_(const intsub<n>& from, interval<int>& to)
    {
      to = interval<int>(from.to_interop());
    }

    template <unsigned n>
    void safe_cast_(const intsub<n>& from, interval<intsub<n> >& to)
    {
      to = interval<intsub<n> >(from);
    }

    // From unsigned

    template <unsigned n>
    void safe_cast_(const unsigned& from, interval<intsub<n> >& to)
    {
      // FIXME: check if unsigned value fits in intsub.
      to = interval<intsub<n> >((int)from);
    }

    void safe_cast_(const unsigned& from, interval<int>& to)
    {
      if (from > static_cast<unsigned>(mln_max(int)))
	std::abort();
      to = interval<int>(from);
    }

    void safe_cast_(const unsigned& from, int& to)
    {
      if (from > static_cast<unsigned>(mln_max(int)))
	std::abort();
      to = from;
    }

    template <unsigned m>
    void safe_cast_(const unsigned& from, int_u<m>& to)
    {
      if (from > mln_max(int_u<m>))
	std::abort();
      to = from;
    }

    template <unsigned n>
    void safe_cast_(const unsigned& from, intsub<n>& to)
    {
      // FIXME: check if unsigned value fits in intsub.
      to = (int)from;
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
