// Copyright (C) 2002, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CONVERT_NBH_TO_SE_HH
# define OLENA_CONVERT_NBH_TO_SE_HH

# include <oln/basics.hh>

// because of the internal function in this file
//# include <oln/basics1d.hh>
# include <oln/basics2d.hh>
//# include <oln/basics3d.hh>
//# include <oln/core/1d/neighborhood1d.hh>
# include <oln/core/2d/neighborhood2d.hh>
//# include <oln/core/3d/neighborhood3d.hh>

namespace oln {
  namespace convert {
    /*! Convert a neighborhood to a window.
    **
    ** \see nbh_to_cse
    */
    template<class N>
    oln_type_of(N, window)
    nbh_to_se(const oln::abstract::neighborhood<N>& nbh)
    {
      oln_type_of(N, window) output;

      for (unsigned i = 0; i < nbh.card(); i++)
	output.add(nbh[i]);
      return output;
    }

    void dpoint_zero(dpoint2d& dp)
    {
      dp.row() = 0;
      dp.col() = 0;
    }

    /*! Convert a neighborhood to a window and add the center.
    **
    ** \see nbh_to_cs
    */
    template<class N>
    oln_type_of(N, window)
    nbh_to_cse(const oln::abstract::neighborhood<N>& nbh)
    {
      oln_type_of(N, window) output;

      for (unsigned i = 0; i < nbh.card(); i++)
	output.add(nbh[i]);

      oln_type_of(N, dpoint) zero;
      dpoint_zero(zero);
      output.add(zero);
      return output;
    }


  } // convert
} // oln


#endif // OLENA_CONVERT_NBH_TO_SE_HH
