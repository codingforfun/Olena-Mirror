// Copyright (C) 2001, 2002, 2003, 2004 EPITA Research and Development
// Laboratory
//
// This  file is  part of  the Olena  Library.  This  library  is free
// software; you can redistribute it  and/or modify it under the terms
// of the  GNU General  Public License version  2 as published  by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with  this library; see the  file COPYING.  If  not, write to
// the Free Software Foundation, 59  Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a  special exception, you  may use this  file as part of  a free
// software library without restriction.  Specifically, if other files
// instantiate templates  or use macros or inline  functions from this
// file, or  you compile  this file  and link it  with other  files to
// produce  an executable,  this file  does  not by  itself cause  the
// resulting  executable  to be  covered  by  the  GNU General  Public
// License.   This exception  does  not however  invalidate any  other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_CORE_NEIGHBORHOOD2D_HH
# define OLN_CORE_NEIGHBORHOOD2D_HH

# include <oln/core/neighborhood_nd.hh>
# include <oln/core/dpoint2d.hh>


namespace oln {

  template <>
  struct neighborhood<2> : public abstract::neighborhood_nd< neighborhood<2> >
  {
    neighborhood& add(coord row, coord col)
    {
      // FIXME: below, "this->super::" is required for the call
      // to "add" to be resolved!
      return this->add(dpoint2d(row, col));
    }

  };

  typedef neighborhood<2>	neighborhood2d;

  const neighborhood2d c4()
  {
    static bool flower = true;
    static neighborhood2d nbh;
    if (flower)
      {
	nbh.add(-1,0).add(0,-1).add(0,1).add(1,0);
	flower = false;
      }
    return nbh;
  }

  const neighborhood2d c8()
  {
    static bool flower = true;
    static neighborhood2d nbh;
    if (flower)
      {
	nbh = c4();
	nbh.add(-1,-1).add(-1,1).add(1,-1).add(1,1);
	flower = false;
      }
    return nbh;
  }


} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_NEIGHBORHOOD2D_HH
