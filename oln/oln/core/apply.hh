// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

# include <oln/core/contract.hh>
# include <oln/core/image.hh>
# include <oln/core/iter.hh>
# include <oln/core/macros.hh>

namespace oln {

  /*------.
  | Unary |
  `------*/

  /* Standard unary 'apply' procedure.  Apply function f to each
     element of _input.  */
  template<class AdaptableUnaryFun, class I_> inline
  typename mute<I_, typename AdaptableUnaryFun::result_type>::ret
  apply(AdaptableUnaryFun f, const image<I_>& _input)
  {
    Exact_cref (I, input);
    typename mute<I, typename AdaptableUnaryFun::result_type>::ret
      output(input.size());
    Iter(I) p(input);
    for_all(p) output[p] = f(input[p]);
    return output;
  }


  /* Same as above, but the function is passed as a type
     and we build it ourself.  */
  template<class AdaptableUnaryFun, class I> inline
  typename mute<I, typename AdaptableUnaryFun::result_type>::ret
  apply(const image<I>& input)
  {
    return apply(AdaptableUnaryFun(), input);
  }


  /* Same as above, but for template functions passed as template-id.
     We need to instantiate the function for the type of the image.  */
  template<template<class> class AdaptableUnaryFun,
	   class I> inline
  typename mute<I, typename AdaptableUnaryFun<Value(I)>::result_type>::ret
  apply(const image<I>& input)
  {
    // Workaround for g++-2.95 bug.
    AdaptableUnaryFun<Value(I)> tmp;
    return apply(tmp, input);
  }


  /*-------.
  | binary |
  `-------*/

  /* FIXME: Don't we want to name these functions 'apply()' too? */

  /* Standard binary 'apply' procedure.  Apply function f to each
     element of _input1 and _inpu2.  */
  template<class AdaptableBinaryFun, class I1_, class I2_> inline
  typename mute<I1_, typename AdaptableBinaryFun::result_type>::ret
  apply2(AdaptableBinaryFun f,
	 const image<I1_>& _input1, const image<I2_>& _input2)
  {
    Exact_cref (I1, input1);
    Exact_cref (I2, input2);
    precondition(input1.size() == input2.size());
    typename mute<I1, typename AdaptableBinaryFun::result_type>::ret
      output(input1.size());
    Iter(I1) p(input1);
    for_all(p) output[p] = f(input1[p], input2[p]);
    return output;
  }


  /* Same as above, but the function is passed as a type
     and we build it ourself.  */
  template<class AdaptableBinaryFun, class I1, class I2> inline
  typename mute<I1, typename AdaptableBinaryFun::result_type>::ret
  apply2(const image<I1>& input1, const image<I2>& input2)
  {
    return apply2(AdaptableBinaryFun(), input1, input2);
  }

  /* Same as above, but for template functions passed as template-id.
     We need to instantiate the function for the type of the images.  */
  template<template <class, class> class AdaptableBinaryFun,
           class I1, class I2> inline
  typename mute<I1,
    typename AdaptableBinaryFun<Value(I1), Value(I2)>::result_type>::ret
  apply2(const image<I1>& input1, const image<I2>& input2)
  {
    // Workaround for g++-2.95 bug.
    AdaptableBinaryFun<Value(I1), Value(I2)> tmp;
    return apply2(tmp, input1, input2);
  }

  /* Same as above, when I1 == I2 and the AdaptableBinaryFun template
     has only one parameter.  */
  template<template <class> class AdaptableBinaryFun,
           class I> inline
  typename mute<I,
    typename AdaptableBinaryFun<Value(I)>::result_type>::ret
  apply2(const image<I>& input1, const image<I>& input2)
  {
    // Workaround for g++-2.95 bug.
    AdaptableBinaryFun<Value(I)> tmp;
    return apply2(tmp, input1, input2);
  }


  /*-----------.
  | self unary |
  `-----------*/

  /* Main apply_self() function.  Note we require a UnaryFun only,
     not a AdaptableUnaryFunc, because as we overwrite an image
     we already know the output type.  */
  template<class UnaryFun, class I_> inline
  image<I_>& apply_self(UnaryFun f, image<I_>& _input)
  {
    Exact_ref(I, input);
    Iter(I) p(input);
    for_all(p) input[p] = f(input[p]);
    return input;
  }


  /* Same as above, but we instantiate the function ourself.  */
  template<class UnaryFun, class I> inline
  image<I>& apply_self(image<I>& input)
  {
    return apply_self(UnaryFun(), input);
  }


  /* If the function is passed as a template-id.  Instantiate it
     for the type of the input elements.  */
  template<template<class> class UnaryFun, class I> inline
  image<I>& apply_self(image<I>& input)
  {
    // Workaround for g++-2.95 bug.
    UnaryFun<Value(I)> tmp;
    return apply_self(tmp, input);
  }


  /*------------.
  | self binary |
  `------------*/

  /* Main apply2_self() function.  See also the comment for apply_self().  */
  template<class UnaryFun, class I1_, class I2_>
  image<I1_>& apply2_self(UnaryFun f,
			  image<I1_>& _input1, const image<I2_>& _input2)
  {
    Exact_ref(I1, input1);
    Exact_ref(I2, input2);
    precondition(input1.size() == input2.size());
    Iter(I1) p(input1);
    for_all(p) input1[p] = f(input1[p], input2[p]);
    return input1;
  }


  /* Same as above, but we instantiate the function ourself.  */
  template<class UnaryFun, class I1, class I2> inline
  image<I1>& apply2_self(image<I1>& input1, const image<I1>& input2)
  {
    return apply_self(UnaryFun(), input1, input2);
  }


  /* If the function is passed as a template-id.  Instantiate it
     for the type of the input elements.  */
  template<template<class, class> class UnaryFun, class I1, class I2> inline
  image<I1>& apply2_self(image<I1>& input1, const image<I2>& input2)
  {
    // Workaround for g++-2.95 bug.
    UnaryFun<Value(I1), Value(I2)> tmp;
    return apply2_self(tmp, input1, input2);
  }


  /* Same as above, but I1==I2 and the UnaryFun has only one parameter.  */
  template<template<class> class UnaryFun, class I> inline
  image<I>& apply2_self(image<I>& input1, const image<I>& input2)
  {
    // Workaround for g++-2.95 bug.
    UnaryFun<Value(I)> tmp;
    return apply2_self(tmp, input1, input2);
  }

} // end of oln

#endif // ! OLENA_CORE_APPLY_HH
