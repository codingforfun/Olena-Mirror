// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_APPLY_HH
# define OLENA_CORE_APPLY_HH

# include <mlc/contract.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/image.hh>
# include <oln/core/abstract/iter.hh>
# include <oln/core/macros.hh>

namespace oln {

  /*------.
  | Unary |
  `------*/

  /*! \brief Standard unary \a apply procedure. Apply a function \a f to each
  ** element of \a input.
  **
  ** Sample of code : Threshold the value of the image.
  **
  ** \code
  ** #include <oln/basics2d.hh>
  ** #include <oln/level/threshold.hh>
  ** #include <ntg/all.hh>
  ** using namespace ntg;
  ** int main()
  ** {
  **   oln::image2d<int_u8> in = oln::load(IMG_IN "lena256.pgm");
  **   int_u8 th	= 127;
  **   rgb_8 low	= rgb_8(100, 0, 0);
  **   rgb_8 height	= rgb_8(0, 200, 255);
  **
  **   oln::image2d<rgb_8> out
  **     = apply(oln::level::threshold<int_u8, rgb_8 >(th, low, height),
  ** 	    in);
  **   save(out, IMG_OUT "oln_level_threshold.ppm");
  ** }
  ** \endcode
  ** \image html lena256_pgm.png
  ** \image latex lena256_pgm.png
  ** =>
  ** \image html oln_level_threshold.png
  ** \image latex oln_level_threshold.png
  */

  template<class AdaptableUnaryFun, class I> inline
  typename mute<I, typename AdaptableUnaryFun::result_type>::ret
  apply(AdaptableUnaryFun f, const abstract::image<I>& input)
  {
    typename mute<I, typename AdaptableUnaryFun::result_type>::ret
      output(input.size());
    oln_iter_type(I) p(input);
    for_all(p) output[p] = f(input[p]);
    return output;
  }


  /*! \brief Standard unary \a apply procedure. Apply a function \a f to each
  ** element of \a input, the function is passed as a type
  ** and is instantiated.
  */
  template<class AdaptableUnaryFun, class I> inline
  typename mute<I, typename AdaptableUnaryFun::result_type>::ret
  apply(const abstract::image<I>& input)
  {
    return apply(AdaptableUnaryFun(), input);
  }


  /*! \brief Standard unary \a apply procedure. Apply function \a f to each
  ** element of \a input, the function is passed as a type
  ** and is instantiated. For template functions passed as template-id,
  ** one need to instantiate the function for the type of the abstract::image.
  */
  template<template<class> class AdaptableUnaryFun,
	   class I> inline
  typename mute<I, typename AdaptableUnaryFun<oln_value_type(I)>::result_type>::ret
  apply(const abstract::image<I>& input)
  {
    // Workaround for g++-2.95 bug.
    AdaptableUnaryFun<oln_value_type(I)> tmp;
    return apply(tmp, input);
  }


  /*-------.
  | binary |
  `-------*/

  /*!
  ** \todo FIXME: Don't we want to name these functions 'apply()' too? 
  */
  
  /*! \brief Standard binary \a apply procedure. Apply function \a f to each
  ** element of \a input1 and \a input2. 
  **
  ** \todo FIXME: Don't we want to name these functions 'apply()' too?
  */
  template<class AdaptableBinaryFun, class I1, class I2> inline
  typename mute<I1, typename AdaptableBinaryFun::result_type>::ret
  apply2(AdaptableBinaryFun f,
	 const abstract::image<I1>& input1, const abstract::image<I2>& input2)
  {
    precondition(input1.size() == input2.size());
    typename mute<I1, typename AdaptableBinaryFun::result_type>::ret
      output(input1.size());
    oln_iter_type(I1) p(input1);
    for_all(p) output[p] = f(input1[p], input2[p]);
    return output;
  }


  /*! \brief Standard binary \a apply procedure. Apply function \a f to each
  ** element of \a input1 and \a input2. The function is passed as a type
  ** and is instantiated.
  **
  ** \todo FIXME: Don't we want to name these functions 'apply()' too?
  */
  template<class AdaptableBinaryFun, class I1, class I2> inline
  typename mute<I1, typename AdaptableBinaryFun::result_type>::ret
  apply2(const abstract::image<I1>& input1, const abstract::image<I2>& input2)
  {
    return apply2(AdaptableBinaryFun(), input1, input2);
  }
  
  /*! \brief Standard binary \a apply procedure. Apply function \a f to each
  ** element of \a input1 and \a input2. The function is passed as a type
  ** and is instantiated. For template functions passed as template-id,
  ** one need to instantiate the function for the type of the 
  ** abstract::images.
  **
  ** \todo FIXME: Don't we want to name these functions 'apply()' too?
  */
  template<template <class, class> class AdaptableBinaryFun,
           class I1, class I2> inline
  typename mute<I1,
    typename AdaptableBinaryFun<oln_value_type(I1),oln_value_type(I2)>::result_type>::ret
  apply2(const abstract::image<I1>& input1, const abstract::image<I2>& input2)
  {
    // Workaround for g++-2.95 bug.
    AdaptableBinaryFun<oln_value_type(I1),oln_value_type(I2)> tmp;
    return apply2(tmp, input1, input2);
  }
  
  
  /*! \brief Standard binary \a apply procedure. Apply function \a f to each
  ** element of \a input1 and \a input2. The function is passed as a type
  ** and is instantiated. For template functions passed as template-id,
  ** one need to instantiate the function for the type of the 
  ** abstract::images.
  **
  ** \todo FIXME: Don't we want to name these functions 'apply()' too?\n
  */
  template<template <class> class AdaptableBinaryFun,
           class I> inline
  typename mute<I,
    typename AdaptableBinaryFun<oln_value_type(I)>::result_type>::ret
  apply2(const abstract::image<I>& input1, const abstract::image<I>& input2)
  {
    // Workaround for g++-2.95 bug.
    AdaptableBinaryFun<oln_value_type(I)> tmp;
    return apply2(tmp, input1, input2);
  }


  /*-----------.
  | self unary |
  `-----------*/

  /*! \brief Main \a apply_self() function. Note only a UnaryFun is required,
  ** not a AdaptableUnaryFunc, because as an abstract::image is overwritten,
  ** the output type is already known.
  */
  template<class UnaryFun, class I> inline
  abstract::image<I>& apply_self(UnaryFun f, abstract::image<I>& input)
  {
    oln_iter_type(I) p(input);
    for_all(p) input[p] = f(input[p]);
    return input;
  }


  /*! \brief Only a UnaryFun is required,
  ** not a AdaptableUnaryFunc, because as an abstract::image is overwritten,
  ** the output type is already known. The function is instantiated.
  */
  template<class UnaryFun, class I> inline
  abstract::image<I>& apply_self(abstract::image<I>& input)
  {
    return apply_self(UnaryFun(), input);
  }


  /*! \brief Only a UnaryFun is required,
  ** not a AdaptableUnaryFunc, because as an abstract::image is overwritten,
  ** the output type is already know. The function is instantiated.
  */
  template<template<class> class UnaryFun, class I> inline
  abstract::image<I>& apply_self(abstract::image<I>& input)
  {
    // Workaround for g++-2.95 bug.
    UnaryFun<oln_value_type(I)> tmp;
    return apply_self(tmp, input);
  }


  /*------------.
  | self binary |
  `------------*/

  /*! \brief Main \a apply2_exact() function.
  **
  ** \see apply_self()
  */
  template<class UnaryFun, class I1, class I2>
  abstract::image<I1>& apply2_self(UnaryFun f,
			  abstract::image<I1>& input1, const abstract::image<I2>& input2)
  {
    precondition(input1.size() == input2.size());
    oln_iter_type(I1) p(input1);
    for_all(p) input1[p] = f(input1[p], input2[p]);
    return input1;
  }


  /*! \brief The function is instantiated.
  **
  ** \see apply_self()
  */
  template<class UnaryFun, class I1, class I2> inline
  abstract::image<I1>& apply2_self(abstract::image<I1>& input1, const abstract::image<I1>& input2)
  {
    return apply_self(UnaryFun(), input1, input2);
  }


  /*! \brief If the function is passed as a template-id, it is 
  ** instantiated for the type of the input elements.
  **
  ** \see apply_self()
  */
  template<template<class, class> class UnaryFun, class I1, class I2> inline
  abstract::image<I1>& apply2_self(abstract::image<I1>& input1, const abstract::image<I2>& input2)
  {
    // Workaround for g++-2.95 bug.
    UnaryFun<oln_value_type(I1),oln_value_type(I2)> tmp;
    return apply2_self(tmp, input1, input2);
  }

  
  /*! \brief If \a I1 == \a I2 and the UnaryFun has only one parameter.
  **
  ** \see apply_self()
  */
  template<template<class> class UnaryFun, class I> inline
  abstract::image<I>& apply2_self(abstract::image<I>& input1, const abstract::image<I>& input2)
  {
    // Workaround for g++-2.95 bug.
    UnaryFun<oln_value_type(I)> tmp;
    return apply2_self(tmp, input1, input2);
  }

} // end of oln

#endif // ! OLENA_CORE_APPLY_HH
