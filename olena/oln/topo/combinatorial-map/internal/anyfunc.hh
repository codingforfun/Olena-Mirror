// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_ANYFUNC_HH
# define OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_ANYFUNC_HH

# include <mlc/contract.hh>

# include <vector>
namespace oln {

  namespace topo {

    namespace combinatorial_map {

      /// oln::combinatorial_map::internal
      namespace internal {

	/*! any
	**
	** \todo FIXME: totally obsolete.
	*/
	template <class Inf>
	class any
	{
	public:
	  const Inf & self() const { return static_cast<const Inf &>(*this); }
	  Inf & self() { return static_cast<Inf &>(*this); }
	};

	/*! Function stored in a vector.
	**
	**
	** \todo FIXME: It has nothing to do there.
	*/
	template <class U, class V, class Inf>
	class anyfunc : public any<Inf>
	{
	protected:
	  anyfunc() : f_(1) {}
	  /// Construct a function on [0..n].
	  anyfunc(unsigned n) : f_(n+1) { assertion(n); }

	public:
	  /// Retrieve the value f(e).
	  V operator()(const U & e) const
	  {
	    assertion(e < f_.size());
	    return f_[e];
	  }

	  /// Resize the domain of f.
	  void resize(unsigned n)
	  {
	    this->self().resize_(n);
	  }
	  /// Assign a value \e to f(i).
	  void assign(const U & i, const V & e)
	  {
	    assertion(i < f_.size());
	    this->self().assign_(i, e);
	  }

	  /// f(i) = 0.
	  void erase(const U & i)
	  {
	    assertion(i < f_.size());
	    this->self().erase_(i);
	  }
	  /// Print the function.
	  std::ostream & print(std::ostream & ostr) const
	  {
	    for (unsigned i = 1; i < f_.size(); ++i)
	      ostr << this->self().name() << "(" << i << ") = " << f_[i] << std::endl;
	    return ostr;
	  }

	protected:
	  std::vector<V> f_;
	};

      } // end of namespace internal

    } // end of namespace combinatorial_map

  } // end of namespace topo

} // end of namespace oln

template <class U, class V, class Inf>
inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::combinatorial_map::internal::anyfunc<U,V,Inf> & f)
{
  return f.exact().print(ostr);
}

#endif // ! OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_ANYFUNC_HH
