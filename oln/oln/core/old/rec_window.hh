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

#ifndef OLENA_CORE_REC_WINDOW_HH
# define OLENA_CORE_REC_WINDOW_HH

# include <iostream>
# include <utility>
# include <cmath>
# include <vector>

# include <oln/core/contract.hh>
# include <oln/core/dpoint.hh>
# include <oln/stat/debase.hh>



//
//  rec_window
//
/////////////////////////////


namespace oln {

  namespace internal {


    template<class Self>
    class rec_window2d
    {

    protected:

      rec_window2d()
      {
	_centered = false;
	_checked = false;
      }

      rec_window2d(unsigned size)
      {
	_dp.reserve(size);
	_centered = false;
	_checked = false;
      }

      rec_window2d(unsigned n, const coord crd[])
      {
	precondition(sizeof(crd) / sizeof(coord) == 2 * n);
	_dp.reserve(n);
	_centered = false;
	for (unsigned i = 0; i < 2 * n; i += 2)
	  add(dpoint(crd[i], crd[i+1]));
      }

      void add(const dpoint2d& dp)
      {
	precondition(! has(dp));
	if (dp == dpoint2d(0,0))
	  _centered = true;
	_dp.push_back(dp);
	_stat_row(dp.row());
	_stat_col(dp.col());
	_stat_delta(abs(dp.row()));
	_stat_delta(abs(dp.col()));
	_checked = false;
      }

      void add(coord row, coord col)
      {
	add(dpoint2d(row, col));
      }

      dpoint2d operator[](unsigned i) const
      {
	precondition(i < card());
	return _dp[i];
      }

      void sym() // FIXME: find a better name
      {
	for (unsigned i = 0; i < card(); ++i)
	  _dp[i] = - _dp[i];
      }

      void _check() const
      {
	// by default, do nothing
      }

      std::vector<dpoint2d> _dp; // data are dpoints

    public:

      void check() const
      {
	if (! _checked)
	  {
	    static_cast<const Self&>(*this)._check();
	    _checked = true;
	  }
      }

      dpoint2d dp(unsigned i) const
      {
	return this->operator[](i);
      }

      bool has(const dpoint2d& dp) const
      {
	return std::find(_dp.begin(), _dp.end(), dp) != _dp.end();
      }

      bool has(coord row, coord col) const
      {
	return has(dpoint2d(row, col));
      }

      unsigned card() const
      {
	return _dp.size();
      }

      coord minrow() const { return _stat_row.min(); }
      coord maxrow() const { return _stat_row.max(); }
      coord mincol() const { return _stat_col.min(); }
      coord maxcol() const { return _stat_col.max(); }

      coord delta()  const { return _stat_delta.max(); }

      bool is_centered() const { return _centered; }

      Self operator-() const
      {
	Self win = static_cast<const Self&>(*this);
	win.sym();
	return win;
      }

    private:

      stat::f_minmax<coord> _stat_row;
      stat::f_minmax<coord> _stat_col;
      stat::f_minmax<coord> _stat_delta; // FIXME: f_max?
      bool _centered;
      bool _checked;
    };


  } // end of internal

} // end of oln


#endif // ! OLENA_CORE_REC_WINDOW_HH
