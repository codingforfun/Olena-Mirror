// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_GEODESIC_EROSION_HH
# define OLENA_MORPHO_GEODESIC_EROSION_HH

# include <mlc/cmp.hh>
# include <mlc/contract.hh>

# include <oln/utils/record.hh>
# include <oln/arith/cmp.hh>
# include <oln/core/gen/image_with_nbh.hh>
# include <oln/morpho/elementary_erosion.hh>




namespace oln {

  namespace morpho {



    /// Generic geodesic erosion.

    template<typename K, typename I, typename II>
    oln_type_of(I, concrete) geodesic_erosion(const tag::kind<K>& kind,
					      const abstract::image_with_nbh<I>& marker,
					      const abstract::image<II>& mask)
    {
      // FIXME: later, test the short code:
      // FIXME: return max(elementary_erosion(kind, marker), mask)

      // FIXME: and even later:
      // FIXME: return elementary_erosion(kind, marker) _max_ mask

      mlc::eq<oln_type_of(I, grid), oln_type_of(II, grid)>::ensure();

      entering("morpho::geodesic_erosion");

      registering(marker, "marker");
      registering(mask,   "mask");
      precondition(marker.size() == mask.size()
		   and marker >= mask);

      oln_type_of(I, concrete)
	ero("ero"),
	output(marker.size(), "output");

      ero = elementary_erosion(kind, marker);

      // FIXME: replace code below by a pw::max...
      oln_type_of(I, fwd_piter) p(marker.size());
      for_all_p (p)
	output[p] = max(ero[p], mask[p]);

      exiting("morpho::geodesic_erosion");
      return output;
    }


    /// Generic classical geodesic erosion.

    template<typename I, typename II>
    oln_type_of(I, concrete) geodesic_erosion(const abstract::image_with_nbh<I>& marker,
					      const abstract::image<II>& mask)
    {
      return geodesic_erosion(tag::classical, marker, mask);
    }



  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_GEODESIC_EROSION_HH
