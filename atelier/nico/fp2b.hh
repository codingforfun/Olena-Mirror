// Copyright (C) 2007 EPITA Research and Development Laboratory.
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef FP2B_HH_
# define FP2B_HH_

namespace oln
{

# define classname  fp2b
# define current    fp2b<Exact>
# define super      any<Exact>
# define templ      template <typename Exact>

  template <typename Exact> struct fp2b;

  //virtual type of function point to bool
  template <typename Exact>
  struct vtypes<fp2b<Exact> >
  {
    typedef mlc::none super_type;
  };


  /*!
  ** \class fp2b
  ** \brief object function point to bool
  ** object function that take a point and return a bool
  **
  */
  template <typename Exact>
  struct fp2b : any<Exact>
  {
    stc_typename(point);	/*!< type of point that function takes*/

    //base operators
    bool operator()(const point& p) const { return this->exact().impl_valid_access(p); }
  };
# include "../local/undefs.hh"


# define classname  chessboard_t
# define current    chessboard_t
# define super      fp2b<chessboard_t>
  struct chessboard_t;

  //virtual types of chess_board_t
  template <>
  struct vtypes<chessboard_t>
  {
    typedef mlc::none super_type;
    typedef point2d point;
  };

  /*!
  ** \class chessboard_t
  ** \brief function that take a point2d and return true if this point can be part of a chessboard
  **
  */
  struct  chessboard_t : super
  {
    typedef point2d point;

    bool operator()(const point& p) const { return (p.row_ + p.col_) % 2 == 0; }
  };
# include "../local/undefs.hh"
}

#endif /* !FP2B_HH_ */
