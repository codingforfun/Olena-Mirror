// Copyright (C) 2001, 2002, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_THICK_GRADIENT_HH
# define OLENA_MORPHO_THICK_GRADIENT_HH

# include <oln/morpho/dilation.hh>
# include <oln/morpho/erosion.hh>
# include <oln/morpho/temp.hh>
# include <oln/ops/arith.hh>


namespace oln {

  namespace morpho {


    /// Beucher thick gradient.

    template<typename I, typename W>
    oln_type_of(I, concrete) thick_gradient_beucher(const abstract::image<I>& input,
						    const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      entering("morpho::thick_gradient_beucher");
      registering(input, "input");
      oln_type_of(I, concrete) dil("dil"), ero("ero"), output("output");

      dil = dilation(input, win);
      ero = erosion(input, win);
      output = force_value_type_to<oln_type_of(I, concrete)>( dil - ero );

      exiting("morpho::thick_gradient_beucher");
      return output;
    }


    /// Internal thick gradient.

    template<typename I, typename W>
    oln_type_of(I, concrete) thick_gradient_internal(const abstract::image<I>& input,
						     const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      entering("morpho::thick_gradient_internal");
      registering(input, "input");
      oln_type_of(I, concrete) ero("ero"), output("output");

      ero = erosion(input, win);
      output = force_value_type_to<oln_type_of(I, concrete)>( input - ero );

      exiting("morpho::thick_gradient_internal");
      return output;
    }


    /// External thick gradient.

    template<typename I, typename W>
    oln_type_of(I, concrete) thick_gradient_external(const abstract::image<I>& input,
						     const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      entering("morpho::thick_gradient_external");
      registering(input, "input");
      oln_type_of(I, concrete) dil("dil"), output("output");

      dil = dilation(input, win);
      output = force_value_type_to<oln_type_of(I, concrete)>( dil - input );

      exiting("morpho::thick_gradient_external");
      return output;
    }



  } // end of namespace oln::morpho

} // end of namespace oln

#endif // OLENA_MORPHO_THICK_GRADIENT_HH
