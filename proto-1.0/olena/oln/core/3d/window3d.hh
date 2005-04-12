// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_WINDOW3D_HH
# define OLENA_CORE_WINDOW3D_HH

# include <oln/core/gen/regular_window.hh>
# include <oln/core/3d/dpoint3d.hh>
# include <oln/core/3d/grid3d.hh>


namespace oln {

  typedef  regular_window< grid3d >  window3d;

  // classical 3d windows

  /*!
  ** \brief Create a window (3 dimensions) of 6 elements.
  ** \return The new window.
  */
  inline const window3d&
  win_c6_only()
  {
    static const coord_t crd[6 * 3] = { -1,0,0,  0,-1,0, 0,0,-1, 0,0,1, 0,1,0,  1,0,0 };
    static const window3d win(6, crd);
    return win;
  }

  /*!
  ** \brief Create a window (3 dimensions) of 7 elements.
  ** \return The new window.
  **
  ** It's the same than win_c6_only() plus the (0,0,0) point.
  */
  inline const window3d&
  win_c6p()
  {
    static const coord_t crd[7 * 3] = { -1,0,0,  0,-1,0, 0,0,-1, 0,0,0, 0,0,1, 0,1,0,  1,0,0 };
    static const window3d win(7, crd);
    return win;
  }


  /*!
  ** \brief Create a window (3 dimensions) of 18 elements.
  ** \return The new window.
  */
  inline const window3d&
  win_c18_only()
  {
    static const coord_t crd[18 * 3] =
      { 
	-1,-1,0, -1,0,-1, -1,0,0, -1,0,1, -1,1,0,  
	 0,-1,-1, 0,-1,0, 0,-1,1, 0,0,-1,  0,0,1, 0,1,-1, 0,1,0, 0,1,1, 
	 1,-1,0,  1,0,-1,  1,0,0,  1,0,1,  1,1,0
      };
    static const window3d win(18, crd);
    return win;
  }

  /*!
  ** \brief Create a window (3 dimensions) of 19 elements.
  ** \return The new window.
  **
  ** It's the same than win_c18_only() plus the (0,0,0) point.
  */
  inline const window3d&
  win_c18p()
  {
    static const coord_t crd[19 * 3] =
      { 
	-1,-1,0, -1,0,-1, -1,0,0, -1,0,1, -1,1,0,  
	 0,-1,-1, 0,-1,0, 0,-1,1, 0,0,-1, 0,0,0, 0,0,1, 0,1,-1, 0,1,0, 0,1,1, 
	 1,-1,0,  1,0,-1,  1,0,0,  1,0,1,  1,1,0
      };
    static const window3d win(19, crd);
    return win;
  }

  /*!
  ** \brief Create a window (3 dimensions) of 26 elements.
  ** \return The new window.
  */
  inline const window3d&
  win_c26_only()
  {
    static const coord_t crd[26 * 3] =
      { 
	-1,-1,-1, -1,-1,0, -1,-1,1, -1,0,-1, -1,0,0, -1,0,1, -1,1,-1, -1,1,0, -1,1,1, 
	0,-1,-1, 0,-1,0, 0,-1,1, 0,0,-1,   0,0,1, 0,1,-1, 0,1,0, 0,1,1, 
	1,-1,-1,  1,-1,0,  1,-1,1,  1,0,-1,  1,0,0,  1,0,1,  1,1,-1,  1,1,0,  1,1,1
      };
    static const window3d win(26, crd);
    return win;
  }

  /*!
  ** \brief Create a window (3 dimensions) of 27 elements.
  ** \return The new window.
  **
  ** It's the same than win_26_only() plus the (0,0,0) point.
  */
  inline const window3d&
  win_c26p()
  {
    static const coord_t crd[27 * 3] =
      { 
	-1,-1,-1, -1,-1,0, -1,-1,1, -1,0,-1, -1,0,0, -1,0,1, -1,1,-1, -1,1,0, -1,1,1, 
	0,-1,-1, 0,-1,0, 0,-1,1, 0,0,-1,  0,0,0,  0,0,1, 0,1,-1, 0,1,0, 0,1,1, 
	1,-1,-1,  1,-1,0,  1,-1,1,  1,0,-1,  1,0,0,  1,0,1,  1,1,-1,  1,1,0,  1,1,1
      };
    static const window3d win(27, crd);
    return win;
  }

} // end of namespace oln


#endif // OLENA_CORE_WINDOW3D_HH
