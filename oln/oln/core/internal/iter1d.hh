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

#ifndef OLENA_CORE_INTERNAL_ITER1D_HH
# define OLENA_CORE_INTERNAL_ITER1D_HH

# include <oln/core/contract.hh>
# include <oln/core/type.hh>
# include <oln/core/coord.hh>
# include <oln/core/point1d.hh>
# include <oln/core/dpoint1d.hh>
# include <oln/core/image1d.hh>
# include <oln/core/objs.hh>

namespace oln {

  namespace internal {

    template<class Exact>
    class _iter1d : public virtual iter< Exact >
    {
    public:

//       typedef _iter1d<Inferior> self;
//       typedef typename type::exact<self>::ret exact;

      const point1d& point_ref() const
      {
	return _p;
      }

      bool operator==(const point1d& p) const
      {
	return _p == p;
      }

      bool operator!=(const point1d& p) const
      {
	return _p != p;
      }

      point1d operator+(const dpoint1d& dp) const
      {
	precondition(*this != end);
	return _p + dp;
      }

      point1d operator-(const dpoint1d& dp) const
      {
	precondition(*this != end);
	return _p - dp;
      }

      operator point1d() const
      {
	precondition(*this != end);
	invariant(_p.col() >= 0 &&
		  _p.col() < _ncols);
	return _p;
      }

      // it's convenient to type `it.cur()' instead of `(point)it' when
      // necessary.
      point1d cur() const
      {
	return *this;
      }

      coord col() const
      {
	return _p.col();
      }

      // deferred methods are:
      //
      //   void _goto_begin();
      //   void _goto_end();
      //   bool _is_at_end() const;
      //   void _goto_next();

      internal::_begin operator=(internal::_begin b)
      {
	to_exact(this)->_goto_begin();
	return b;
      }

      internal::_end operator=(internal::_end e)
      {
	to_exact(this)->_goto_end();
	return e;
      }

      bool operator==(internal::_end) const
      {
	return to_exact(this)->_is_at_end();
      }

      void operator++()
      {
	precondition(*this != end);
	to_exact(this)->_goto_next();
// 	return to_exact(*this);
      }

      // deduced methods:

      bool operator!=(internal::_end e) const
      {
	return ! this->operator==(e);
      }

//       typename type::exact<self>::ret operator++(int)
//       {
// 	precondition(*this != end);
// 	typename type::exact<self>::ret tmp = to_exact(*this);
// 	this->operator++();
// 	return tmp;
//       }

      static std::string name() { return std::string("_iter1d<") + Exact::name() + ">"; }

    protected:

      point1d _p;
      const coord _ncols;

      _iter1d(const image1d_size& size) :
	_ncols(size.ncols())
      {
	precondition(size.ncols() > 0);
	to_exact(this)->_goto_begin();
      }
    };


  } // end of internal

} // end of oln


#endif // ! OLENA_CORE_INTERNAL_ITER1D_HH
