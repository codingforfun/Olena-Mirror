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

#ifndef OLENA_CORE_INTERNAL_W_WINDOW_HH
# define OLENA_CORE_INTERNAL_W_WINDOW_HH

# include <mlc/contract.hh>
# include <oln/core/w_window.hh>
# include <oln/core/point.hh>
# include <oln/core/dpoint.hh>
# include <iostream>
# include <vector>
# include <utility>

namespace oln {

  namespace internal {

    template <unsigned Dim, class Weight, class Inferior = type::bottom>
    class _w_window : public w_window< _w_window< Dim, Weight, Inferior > >
    {
    public:
      typedef Inferior inferior;
      enum { dim = Dim };
      typedef typename point_for_dim<Dim>::ret      point;
      typedef typename dpoint_for_dim<Dim>::ret     dpoint;
      typedef Weight weight;
      typedef _w_window self;

      bool has(const dpoint& dp) const
      {
	return std::find(_dp.begin(), _dp.end(), dp) != _dp.end();
      }

      unsigned card() const
      {
	return _dp.size();
      }

      bool is_centered() const
      {
	return _centered;
      }

      dpoint dp(unsigned i) const
      {
	return (*this)[i];
      }

      Weight w(unsigned i) const
      {
	precondition(i < card());
	return _w[i];
      }

      static std::string name()
      {
	return std::string("_w_window<") + type::name_of<dpoint>() + "," +
	  type::name_of<Weight>() + "," + inferior::name() + ">" ;
      }

    protected:
      _w_window()
      {
	_centered = false;
      }

      _w_window(unsigned size)
      {
	_dp.reserve(size);
	_w.reserve(size);
	_centered = false;
      }

      void add(const dpoint& dp, const Weight& w)
      {
	precondition(! has(dp));
	if (dp.is_centered())
	  _centered = true;
	_dp.push_back(dp);
	_w.push_back(w);
      }

      const dpoint operator[](unsigned i) const
      {
	precondition(i < card());
	return _dp[i];
      }

      void sym()
      {
	for (unsigned i = 0; i < card(); ++i)
	  _dp[i] = - _dp[i];
      }

      std::vector< dpoint > _dp;
      std::vector< Weight > _w;
      bool _centered;
    };

  } // internal

} // oln

template< unsigned V, class U, class T >
std::ostream&
operator<<(std::ostream& o, const oln::internal::_w_window<V,U,T>& w)
{
  unsigned c = w.card();
  o << "[";
  for (unsigned i = 0; i < c; ++i)
    o << "(" << w.dp(i) << "," << w.w(i) << ")";
  o << "]";
  return o;
}

#endif // OLENA_CORE_INTERNAL_W_WINDOW_HH
