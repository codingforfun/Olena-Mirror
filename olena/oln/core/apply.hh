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

  /* Standard unary 'apply' procedure.  Apply function f to each
     element of input.  */
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


  /* Same as above, but the function is passed as a type
     and we build it ourself.  */
  template<class AdaptableUnaryFun, class I> inline
  typename mute<I, typename AdaptableUnaryFun::result_type>::ret
  apply(const abstract::image<I>& input)
  {
    return apply(AdaptableUnaryFun(), input);
  }


  /* Same as above, but for template functions passed as template-id.
     We need to instantiate the function for the type of the abstract::image.  */
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

  /* FIXME: Don't we want to name these functions 'apply()' too? */

  /* Standard binary 'apply' procedure.  Apply function f to each
     element of input1 and input2.  */
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


  /* Same as above, but the function is passed as a type
     and we build it ourself.  */
  template<class AdaptableBinaryFun, class I1, class I2> inline
  typename mute<I1, typename AdaptableBinaryFun::result_type>::ret
  apply2(const abstract::image<I1>& input1, const abstract::image<I2>& input2)
  {
    return apply2(AdaptableBinaryFun(), input1, input2);
  }

  /* Same as above, but for template functions passed as template-id.
     We need to instantiate the function for the type of the abstract::images.  */
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

  /* Same as above, when I1 == I2 and the AdaptableBinaryFun template
     has only one parameter.  */
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

  /* Main apply_self() function.  Note we require a UnaryFun only,
     not a AdaptableUnaryFunc, because as we overwrite an abstract::image
     we already know the output type.  */
  template<class UnaryFun, class I> inline
  abstract::image<I>& apply_self(UnaryFun f, abstract::image<I>& input)
  {
   oln_iter_type(I) p(input);
    for_all(p) input[p] = f(input[p]);
    return input;
  }


  /* Same as above, but we instantiate the function ourself.  */
  template<class UnaryFun, class I> inline
  abstract::image<I>& apply_self(abstract::image<I>& input)
  {
    return apply_self(UnaryFun(), input);
  }


  /* If the function is passed as a template-id.  Instantiate it
     for the type of the input elements.  */
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

  /* Main apply2_exact() function.  See also the comment for apply_self().  */
  template<class UnaryFun, class I1, class I2>
  abstract::image<I1>& apply2_self(UnaryFun f,
			  abstract::image<I1>& input1, const abstract::image<I2>& input2)
  {
    precondition(input1.size() == input2.size());
   oln_iter_type(I1) p(input1);
    for_all(p) input1[p] = f(input1[p], input2[p]);
    return input1;
  }


  /* Same as above, but we instantiate the function ourself.  */
  template<class UnaryFun, class I1, class I2> inline
  abstract::image<I1>& apply2_self(abstract::image<I1>& input1, const abstract::image<I1>& input2)
  {
    return apply_self(UnaryFun(), input1, input2);
  }


  /* If the function is passed as a template-id.  Instantiate it
     for the type of the input elements.  */
  template<template<class, class> class UnaryFun, class I1, class I2> inline
  abstract::image<I1>& apply2_self(abstract::image<I1>& input1, const abstract::image<I2>& input2)
  {
    // Workaround for g++-2.95 bug.
    UnaryFun<oln_value_type(I1),oln_value_type(I2)> tmp;
    return apply2_self(tmp, input1, input2);
  }


  /* Same as above, but I1==I2 and the UnaryFun has only one parameter.  */
  template<template<class> class UnaryFun, class I> inline
  abstract::image<I>& apply2_self(abstract::image<I>& input1, const abstract::image<I>& input2)
  {
    // Workaround for g++-2.95 bug.
    UnaryFun<oln_value_type(I)> tmp;
    return apply2_self(tmp, input1, input2);
  }

} // end of oln

#endif // ! OLENA_CORE_APPLY_HH
