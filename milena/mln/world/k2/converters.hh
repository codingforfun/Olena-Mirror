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
/// \brief Fill 1 faces in a K2 2D image using its 2 faces.

#ifndef MLN_WORLD_K2_CONVERTERS_HH
# define MLN_WORLD_K2_CONVERTERS_HH

# include <mln/value/int_u8.hh>
# include <mln/value/intsub.hh>
# include <mln/value/interval.hh>

namespace mln
{

  namespace world
  {

    namespace k2
    {

      using namespace mln::value;

      template <unsigned n>
      struct int_sub_n_to_intsub_n_2
      {
	intsub<n/2> operator()(const intsub<n>& from) const;
	intsub<n> operator()(const intsub<n/2>& from) const;
      };

      template <unsigned n>
      struct int_sub_n_to_intsub_2n
      {
	intsub<2*n> operator()(const intsub<n>& from) const;
	intsub<n> operator()(const intsub<2*n>& from) const;
      };

      template <unsigned n>
      struct int_u8_to_intsub_n
      {
	intsub<n> operator()(const value::int_u8& from) const;
	value::int_u8 operator()(const intsub<n>& from) const;
      };

      template <unsigned n>
      struct int_u8_to_interval_intsub_n
      {
	interval<intsub<n> > operator()(const value::int_u8& from) const;
	value::int_u8 operator()(const interval<intsub<n> >& from) const;
      };

      template <unsigned n>
      struct interval_intsub_n_to_int
      {
	int operator()(const interval<intsub<n> >& from) const;
	interval<intsub<n> > operator()(const int& from) const;
      };

      template <unsigned n>
      struct interval_intsub_n_to_int_u8
      {
	value::int_u8 operator()(const interval<intsub<n> >& from) const;
	interval<intsub<n> > operator()(const value::int_u8& from) const;
      };

      template <unsigned n>
      struct interval_intsub_n_to_intsub_n_2
      {
	intsub<n/2> operator()(const interval<intsub<n> >& from) const;
	interval<intsub<n> > operator()(const intsub<n/2>& from) const;
      };

      template <typename T1, typename T2>
      struct generic_converter
      {
	T2 operator()(const T1& from) const;
	T1 operator()(const T2& from) const;
      };



#ifndef MLN_INCLUDE_ONLY

      template <unsigned n>
      intsub<n/2>
      int_sub_n_to_intsub_n_2<n>::operator()(const intsub<n>& from) const
      {
	return intsub<n/2>(from);
      }

      template <unsigned n>
      intsub<n>
      int_sub_n_to_intsub_n_2<n>::operator()(const intsub<n/2>& from) const
      {
	return intsub<n>(from);
      }


      template <unsigned n>
      intsub<2*n>
      int_sub_n_to_intsub_2n<n>::operator()(const intsub<n>& from) const
      {
	return static_cast<intsub<2*n> >(from.to_int());
      }

      template <unsigned n>
      intsub<n>
      int_sub_n_to_intsub_2n<n>::operator()(const intsub<2*n>& from) const
      {
	return static_cast<intsub<n> >(from.to_int());
      }

      template <unsigned n>
      intsub<n>
      int_u8_to_intsub_n<n>::operator()(const int_u8& from) const
      {
	return from.to_equiv();
      }

      template <unsigned n>
      value::int_u8
      int_u8_to_intsub_n<n>::operator()(const intsub<n>& from) const
      {
	return from.to_interop();
      }

      template <unsigned n>
      interval<intsub<n> >
      int_u8_to_interval_intsub_n<n>::operator()(const int_u8& from) const
      {
	return intsub<n>(from.to_interop());
      }

      template <unsigned n>
      value::int_u8
      int_u8_to_interval_intsub_n<n>::operator()(const interval<intsub<n> >& from) const
      {
	return from.first().to_interop();
      }

      template <unsigned n>
      int
      interval_intsub_n_to_int<n>::operator()(const interval<intsub<n> >& from) const
      {
	if (!from.is_degenerated())
	  abort();
	return from.first();
      }

      template <unsigned n>
      interval<intsub<n> >
      interval_intsub_n_to_int<n>::operator()(const int& from) const
      {
	return interval<intsub<n> >(from);
      }

      template <unsigned n>
      value::int_u8
      interval_intsub_n_to_int_u8<n>::operator()(const interval<intsub<n> >& from) const
      {
	if (!from.is_degenerated())
	  abort();
	return from.first().to_interop();
      }

      template <unsigned n>
      interval<intsub<n> >
      interval_intsub_n_to_int_u8<n>::operator()(const value::int_u8& from) const
      {
	return intsub<n>(from.to_interop());
      }

      template <unsigned n>
      intsub<n/2>
      interval_intsub_n_to_intsub_n_2<n>::operator()(const interval<intsub<n> >& from) const
      {
	if (!from.is_degenerated())
	  abort();
	return from.first().to_interop();
      }

      template <unsigned n>
      interval<intsub<n> >
      interval_intsub_n_to_intsub_n_2<n>::operator()(const intsub<n/2>& from) const
      {
	return interval<intsub<n> >(from);
      }

      template <typename T1, typename T2>
      T2
      generic_converter<T1,T2>::operator()(const T1& from) const
      {
	return static_cast<T2>(from);
      }

      template <typename T1, typename T2>
      T1
      generic_converter<T1,T2>::operator()(const T2& from) const
      {
	return static_cast<T1>(from);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k2

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K2_CONVERTERS_HH
