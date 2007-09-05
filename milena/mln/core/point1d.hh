// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_POINT1D_HH
# define MLN_CORE_POINT1D_HH

/*! \file mln/core/point1d.hh
 *
 * \brief Definition of the mln::point1d alias and of its construction
 * routine.
 */

# include <mln/core/point.hh>


namespace mln
{

  /*! \brief Type alias for a point defined on the 1D square grid with
   * integer coordinates.
   */
  typedef point_<1,int> point1d;


} // end of namespace mln


# include <mln/make/point1d.hh>
# include <mln/core/dpoint1d.hh>


#endif // ! MLN_CORE_POINT1D_HH
