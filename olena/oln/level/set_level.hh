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

#ifndef OLENA_LEVEL_SET_LEVEL_HH
# define OLENA_LEVEL_SET_LEVEL_HH

# include <oln/core/box.hh>
# include <oln/core/dpoint2d.hh>

namespace oln {

  namespace level {

    template <class I>
    void 
    set_level(abstract::image_with_dim<2,I>& inout,
	      const oln_point_type(I)& p1,
	      const oln_point_type(I)& p2,
	      oln_value_type(I) level)
    {
      int iRow1 = p1.row();
      int iCol1 = p1.col();
      int iRow2 = p2.row();
      int iCol2 = p2.col();
      int dRow = ( iRow2 > iRow1 ? iRow2 - iRow1 : iRow1 - iRow2 );
      int ddRow = 2 * dRow;
      int dCol = ( iCol2 > iCol1 ? iCol2 - iCol1 : iCol1 - iCol2 );
      int ddCol = 2 * dCol;
      int sRow = ( iRow2 != iRow1 ? ( iRow2 > iRow1 ? 1 : -1 ) : 0 );
      int sCol = ( iCol2 != iCol1 ? ( iCol2 > iCol1 ? 1 : -1 ) : 0 );
      int i, e;
      int iRow = iRow1;
      int iCol = iCol1;

      if ( dCol > dRow )
	{
	  e = ddRow - dCol;
	  for ( i = 0; i < dCol; ++i )
	    {
	      inout(iRow, iCol) = level;
	      while ( e >= 0 )
		{
		  iRow += sRow;
		  e -= ddCol;
		}
	      iCol += sCol;
	      e += ddRow;
	    }
	}
      else
	{
	  e = ddCol - dRow;
	  for ( i = 0; i < dRow; ++i )
	    {
	      inout(iRow,iCol) = level;
	      while ( e >= 0 )
		{
		  iCol += sCol;
		  e -= ddRow;
		}
	      iRow += sRow;
	      e += ddCol;
	    }
	}
      inout(iRow,iCol) = level;
    }


    template <class I, class BoxType>
    void 
    set_level(abstract::image_with_dim<2,I>& inout,
	      BoxType& box,
	      const oln_value_type(I)& level)
    {
      if (box.card() != 0)
	{
	  dpoint2d drows(box.top().row() - box.bottom().row(), 0);
	  dpoint2d dcols(0, box.top().col() - box.bottom().col());
	  set_level(inout, box.top(), box.top() - drows, level);
	  set_level(inout, box.top(), box.top() - dcols, level);
	  set_level(inout, box.bottom(), box.bottom() + drows, level);
	  set_level(inout, box.bottom(), box.bottom() + dcols, level);
	}
    }

  } // end of namespace level

} // end of namespace oln

#endif // ! OLENA_LEVEL_SET_LEVEL_HH
