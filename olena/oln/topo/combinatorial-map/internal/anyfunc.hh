// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_ANYFUNC_HH
# define OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_ANYFUNC_HH

# include <mlc/contract.hh>

# include <vector>

namespace oln {

  namespace topo {

    namespace combinatorial_map {

      namespace internal {

	// any
	template <class Inf>
	class any
	{
	public:
	  const Inf & self() const { return static_cast<const Inf &>(*this); }
	  Inf & self() { return static_cast<Inf &>(*this); }
	};

	template <class U, class V, class Inf>
	class anyfunc : public any<Inf>
	{
	protected:
	  anyfunc() : _f(1) {}
	  anyfunc(unsigned n) : _f(n+1) { assertion(n); }

	public:
	  V operator()(const U & e) const
	  {
	    assertion(e < _f.size());
	    return _f[e];
	  }

	  void resize(unsigned n)
	  {
	    self()._resize(n);
	  }

	  void assign(const U & i, const V & e)
	  {
	    assertion(i < _f.size());
	    self()._assign(i, e);
	  }

	  void erase(const U & i)
	  {
	    assertion(i < _f.size());
	    self()._erase(i);
	  }

	  std::ostream & print(std::ostream & ostr) const
	  {
	    for (unsigned i = 1; i < _f.size(); ++i)
	      ostr << self().name() << "(" << i << ") = " << _f[i] << std::endl;
	    return ostr;
	  }

	protected:
	  std::vector<V> _f;
	};

      } // end internal

    } // end combinatorial_map

  } // end topo

} // end oln

template <class U, class V, class Inf>
inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::combinatorial_map::internal::anyfunc<U,V,Inf> & f);

# include <oln/topo/combinatorial-map/internal/anyfunc.hxx>

#endif // !OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_ANYFUNC_HH
