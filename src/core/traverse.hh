// Copyright 2001  EPITA Research and Development Laboratory
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

# include "core/contract.hh"
# include "core/image.hh"
# include "core/iter.hh"



namespace oln {

  // traverse (unary)


  template<class _I, class F>
  const F& traverse(F& f, const image<_I>& _input)
  {
    Exact_cref(I, input);
    Iter(I) p(input);
    for_all(p) f(input[p]);
    return f;
  }

  template<class F,
	   class I> inline
  const F traverse(const image<I>& input)
  {
    F f;
    return traverse(f, input);
  }

  template<template<class> class F,
	   class I> inline
  const F<Value(I)> traverse(const image<I>& input)
  {
    F<Value(I)> f;
    return traverse(f, input);
  }

  template<template<class,class> class F,
	   class I2, class I> inline
  const F<Value(I),I2> traverse(const image<I>& input)
  {
    F<Value(I),I2> f;
    return traverse(f, input);
  }

  template<template<class,class> class F,
           template<class> class F2,
           class I> inline
  const F< Value(I), F2<Value(I)> > traverse(const image<I>& input)
  {
    F< Value(I), F2<Value(I)> > f;
    traverse(f, input);
    return f;
  }


  // traverse2 (binary)


  template<class _I1, class _I2, class F>
  const F& traverse2(F& f,
		    const image<_I1>& _input1, const image<_I2>& _input2)
  {
    Exact_cref(I1, input1);
    Exact_cref(I2, input2);
    precondition(input1.size() == input2.size());
    Iter(I1) p(input1);
    for_all(p) f(input1[p], input2[p]);
    return f;
  }

  template<template<class> class F,
           class I> inline
  const F<Value(I)> traverse2(const image<I>& input1, const image<I>& input2)
  {
    F<Value(I)> f;
    return traverse2(f, input1, input2);
  }

  template<template<class,class> class F,
           class I1, class I2> inline
  const F<Value(I1),Value(I2)> traverse2(const image<I1>& input1, const image<I2>& input2)
  {
    F<Value(I1),Value(I2)> f;
    return traverse2(f, input1, input2);
  }

} // end of oln

#endif // ! OLENA_CORE_TRAVERSE_HH
