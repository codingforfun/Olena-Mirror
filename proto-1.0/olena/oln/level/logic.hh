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

#ifndef OLENA_LEVEL_LOGIC_HH
# define OLENA_LEVEL_LOGIC_HH

# include <oln/core/abstract/image_entry.hh>
# include <oln/core/apply.hh>
# include <oln/funobj/logic.hh>



namespace oln
{

  namespace level
  {

    // f_and

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    f_and (const abstract::binary_image<I1>& input1,
	   const abstract::binary_image<I2>& input2)
    {
      // FIXME: recording(?)
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      return apply2(oln::f_and, input1, input2);
    }


    // f_nand

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    f_nand (const abstract::binary_image<I1>& input1,
	    const abstract::binary_image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      return apply2(oln::f_nand, input1, input2);
    }


    // f_or

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    f_or (const abstract::binary_image<I1>& input1,
	  const abstract::binary_image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      return apply2(oln::f_or, input1, input2);
    }


    // f_nor

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    f_nor (const abstract::binary_image<I1>& input1,
	   const abstract::binary_image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      return apply2(oln::f_nor, input1, input2);
    }



    // f_xor

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    f_xor (const abstract::binary_image<I1>& input1,
	   const abstract::binary_image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      return apply2(oln::f_xor, input1, input2);
    }


    // f_nxor

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    f_xnor (const abstract::binary_image<I1>& input1,
	    const abstract::binary_image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      return apply2(oln::f_xnor, input1, input2);
    }


  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLENA_LEVEL_LOGIC_HH
