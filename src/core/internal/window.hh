// Copyright 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_INTERNAL_WINDOW_HH
# define OLENA_CORE_INTERNAL_WINDOW_HH

# include "core/contract.hh"
# include "core/window.hh"
# include "core/point.hh"
# include "core/dpoint.hh"
# include <iostream>
# include <vector>
# include <utility>

namespace oln {

  namespace internal {

    /* Don't always inherit from window<>.  See _neighborhood for
       an example where we use another base class.  */
    template <unsigned Dim, class Inferior = type::bottom,
	      template <class> class Window = window>
    class _window : public Window< _window< Dim, Inferior > >
    {
    public:
      typedef Inferior inferior;
      enum { dim = Dim };
      typedef typename point_for_dim<Dim>::ret      point;
      typedef typename dpoint_for_dim<Dim>::ret     dpoint;
      typedef _window self;

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

      static std::string name()
      {
	return std::string("_window<") + Inferior::name() + ">";
      }

    protected:
      _window()
      {
	_centered = false;
      }

      _window(unsigned size)
      {
	_dp.reserve(size);
	_centered = false;
      }

      void add(const dpoint& dp)
      {
	precondition(! has(dp));
	if (dp.is_centered())
	  _centered = true;
	_dp.push_back(dp);
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

      std::vector<dpoint> _dp;
      bool _centered;
    };

  } // internal

} // oln

template< unsigned U, class T >
std::ostream& operator<<(std::ostream& o, const oln::internal::_window<U,T>& w)
{
  unsigned c = w.card();
  o << "[";
  for (unsigned i = 0; i < c; ++i)
    o << w.dp(i);
  o << "]";
  return o;
}

#endif // OLENA_CORE_INTERNAL_WINDOW_HH
