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

#ifndef OLENA_CORE_INTERNAL_NEIGHBORHOOD_HH
# define OLENA_CORE_INTERNAL_NEIGHBORHOOD_HH

# include <oln/core/internal/window.hh>
# include <oln/core/neighborhood.hh>

namespace oln {

  namespace internal {

    template <unsigned Dim, class Exact>
    class _neighborhood :
      /* The inheritence is quite tricky here.

	 Basically, we want to reuse all methods from _window, but
	 without being seen as a window<>. Instead we want to be seen
	 as a neighborhood<>.  So we instruct _window to inherit from
	 neighborhood<> instead of window<> using the third parameter.
	 */
      public _window< Dim,
		      Exact,
		      oln::neighborhood >
    {
    public:

      typedef _window< Dim, Exact,
		       oln::neighborhood > super;

      _neighborhood() : super() {}
      _neighborhood(unsigned size) : super(size) {}

      static std::string name()
      {
	return std::string("_neighborhood<") + Exact::name() + ">";
      }
    };

  } // internal

} // oln

template< unsigned U, class T >
std::ostream& operator<<(std::ostream& o, const oln::internal::_neighborhood<U,T>& w)
{
  unsigned c = w.card();
  o << "N[";
  for (unsigned i = 0; i < c; ++i)
    o << w.dp(i);
  o << "]";
  return o;
}

#endif // OLENA_CORE_INTERNAL_WINDOW_HH
