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

#ifndef OLENA_CORE_INTERNAL_ITER2D_HH
# define OLENA_CORE_INTERNAL_ITER2D_HH

# include <oln/core/coord.hh>
# include <oln/core/point2d.hh>
# include <oln/core/dpoint2d.hh>
# include <oln/core/image2d.hh>
# include <oln/core/iter.hh>

# include <mlc/contract.hh>
# include <mlc/type.hh>
# include <mlc/objs.hh>

namespace oln {

  namespace internal {


    template<class Exact>
    class _iter2d : public virtual iter< Exact >
    {
    public:

//       typedef _iter2d<Inferior> self;
//       typedef typename mlc::exact<self>::ret exact;

      const point2d& point_ref() const
      {
	return _p;
      }

      bool operator==(const point2d& p) const
      {
	return _p == p;
      }

      bool operator!=(const point2d& p) const
      {
	return _p != p;
      }

      point2d operator+(const dpoint2d& dp) const
      {
	precondition(*this != end);
	return _p + dp;
      }

      point2d operator-(const dpoint2d& dp) const
      {
	precondition(*this != end);
	return _p - dp;
      }

      operator point2d() const
      {
	precondition(*this != end);
	invariant(_p.row() >= 0 &&
		  (_p.row() < _nrows || _p.row() == _nrows) &&
		  _p.col() >= 0 &&
		  _p.col() < _ncols);
	return _p;
      }

      // it's convenient to type `it.cur()' instead of `(point)it' when
      // necessary.
      point2d cur() const
      {
	return *this;
      }

      coord row() const
      {
	return _p.row();
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

      mlc::_begin operator=(mlc::_begin b)
      {
	to_exact(this)->_goto_begin();
	return b;
      }

      mlc::_end operator=(mlc::_end e)
      {
	to_exact(this)->_goto_end();
	return e;
      }

      bool operator==(mlc::_end) const
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

      bool operator!=(mlc::_end e) const
      {
	return ! this->operator==(e);
      }

//       typename mlc::exact<self>::ret operator++(int)
//       {
// 	precondition(*this != end);
// 	typename mlc::exact<self>::ret tmp = to_exact(*this);
// 	this->operator++();
// 	return tmp;
//       }

      static std::string name() { return std::string("_iter2d<") + Exact::name() + ">"; }

    protected:

      point2d _p;
      const coord _nrows;
      const coord _ncols;

      _iter2d(const image2d_size& size) :
	_nrows(size.nrows()),
	_ncols(size.ncols())
      {
	precondition(size.nrows() > 0 &&
		     size.ncols() > 0);
	to_exact(this)->_goto_begin();
      }
    };


  } // end of internal

} // end of oln


#endif // ! OLENA_CORE_INTERNAL_ITER2D_HH
