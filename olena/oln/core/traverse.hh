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

#ifndef OLENA_CORE_TRAVERSE_HH
# define OLENA_CORE_TRAVERSE_HH

# include <mlc/contract.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/iter.hh>
# include <oln/core/macros.hh>

namespace oln {

  // traverse (unary)

  /*! \brief Call the functor \a f on each point of the
  ** input image.
  */
  template<class I, class F>
  const F& 
  traverse(F& f, const abstract::image<I>& input)
  {
    oln_iter_type(I) p(input);
    for_all(p) f(input[p]);
    return f;
  }

  
  /*! \brief Create a functor \a f whose type is F, then
  ** call it on each point of the input image.
  */
  template<class F, class I> 
  inline const F 
  traverse(const abstract::image<I>& input)
  {
    F f;
    return traverse(f, input);
  }

  /*! \brief Create a functor \a f whose type is F<oln_value_type(I)>, 
  ** then call it on each point of the input image.
  */
  template<template<class> class F, class I> 
  inline const F<oln_value_type(I)> 
  traverse(const abstract::image<I>& input)
  {
    F<oln_value_type(I)> f;
    return traverse(f, input);
  }

  
  /*! \brief Create a functor \a f whose type is F<oln_value_type(I), I2>,
  ** the call it on each point of the input image.
  */
  template<template<class, class> class F, class I2, class I> 
  inline const F<oln_value_type(I), I2> 
  traverse(const abstract::image<I>& input)
  {
    F<oln_value_type(I), I2> f;
    return traverse(f, input);
  }

  /*! \brief Create a functor \a f whose type is 
  ** F<oln_value_type(I), F2<oln_value_type(I)> >, then call it on
  ** each point of the input image.
  */
  template<template<class,class> class F,
           template<class> class F2,
           class I> 
  inline const F<oln_value_type(I), F2<oln_value_type(I)> > 
  traverse(const abstract::image<I>& input)
  {
    F<oln_value_type(I), F2<oln_value_type(I)> > f;
    traverse(f, input);
    return f;
  }


  // traverse2 (binary)

  /*! \brief Call functor \a f whose type is F on each point
  ** of the two input images.
  **
  ** \pre input1.size() == input2.size()
  */
  template<class I1, class I2, class F>
  const F& 
  traverse2(F& f,
	    const abstract::image<I1>& input1, 
	    const abstract::image<I2>& input2)
  {
    precondition(input1.size() == input2.size());
    oln_iter_type(I1) p(input1);
    for_all(p) f(input1[p], input2[p]);
    return f;
  }

  /*! \brief Create a functor \a f whose type is F<oln_value_type(I)>,
  ** then call it on each point of the two input images.
  **
  ** \pre input1.size() == input2.size()
  */
  
  template<template<class> class F, class I> 
  inline const F<oln_value_type(I)> 
  traverse2(const abstract::image<I>& input1, const abstract::image<I>& input2)
  {
    F<oln_value_type(I)> f;
    return traverse2(f, input1, input2);
  }

  /*! \brief Create a functor \a f whose type is 
  ** F<oln_value_type(I1), oln_value_type(I2)>, then call it on each point 
  ** of the two input images.
  **
  ** \pre input1.size() == input2.size()
  */
  
  template<template<class,class> class F, class I1, class I2> 
  inline const F<oln_value_type(I1), oln_value_type(I2)> 
  traverse2(const abstract::image<I1>& input1, 
	    const abstract::image<I2>& input2)
  {
    F<oln_value_type(I1), oln_value_type(I2)> f;
    return traverse2(f, input1, input2);
  }

} // end of oln

#endif // ! OLENA_CORE_TRAVERSE_HH
