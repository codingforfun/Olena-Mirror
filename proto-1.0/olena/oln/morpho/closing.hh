// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_CLOSING_HH
# define OLENA_MORPHO_CLOSING_HH

# include <mlc/cmp.hh>
# include <oln/morpho/erosion.hh>
# include <oln/morpho/dilation.hh>


namespace oln {

  namespace morpho {



    /// Closing.

    template<typename I, typename W>
    oln_type_of(I, concrete) closing(const abstract::image<I>& input,
				     const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      entering("morpho::closing");
      registering(input, "input");

      oln_type_of(I, concrete) output("output");
      output = erosion(dilation(input, win), -win);

      exiting("morpho::closing");
      return output;
    }



  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_CLOSING_HH
