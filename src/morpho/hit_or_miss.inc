// -*- c++ -*-
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

//
//  hit-or-miss transform
//
///////////////////////////////////

// extended to every data types ;
// note that results depend on the type of image data when it is not bin.
// you MUST be aware of it.

template<class C, class _I, class _E1, class _E2> inline
Concrete(_I) hit_or_miss(const conversion<C>& c,
			 const image<_I>& _input,
			 const struct_elt<_E1>& _se1,
			 const struct_elt<_E2>& _se2)
{
  Exact_cref(I, input);
  Exact_cref(E1, se1);
  Exact_cref(E2, se2);

  precondition(inter(se1, se2).card() == 0);
  // FIXME: math::max(coord, coord) does not exist.
  border::adapt_copy(input,
		     se1.delta() > se2.delta() ?
		     se1.delta() : se2.delta());
  return morpho::min(c,
		     erosion(input, se1),
		     erosion(level::invert(input), se2));
}

template<class I, class E1, class E2> inline
Concrete(I) hit_or_miss(const image<I>& input,
			const struct_elt<E1>& se1,
			const struct_elt<E2>& se2)
{
  return hit_or_miss(convert::force<Value(I)>(), input, se1, se2);
}


//
//  hit-or-miss opening
//
///////////////////////////////////

template<class _I, class _E1, class _E2> inline
Concrete(_I) hit_or_miss_opening(const image<_I>& _input,
				 const struct_elt<_E1>& _se1,
				 const struct_elt<_E2>& _se2)
{
  Exact_cref(I, input);
  Exact_cref(E1, se1);
  Exact_cref(E2, se2);

  precondition(inter(se1, se2).card() == 0);
  return dilation(hit_or_miss(input, se1, se2), -se1);
}

template<class I, class E1, class E2> inline
Concrete(I) hit_or_miss_opening_bg(const image<I>& input,
				   const struct_elt<E1>& se1,
				   const struct_elt<E2>& se2)
{
  return hit_or_miss_opening(input, se2, se1);
}


//
//  hit-or-miss closing
//
///////////////////////////////////

template<class I, class _E1, class _E2> inline
Concrete(I) hit_or_miss_closing(const image<I>& input,
				const struct_elt<_E1>& _se1,
				const struct_elt<_E2>& _se2)
{
  Exact_cref(E1, se1);
  Exact_cref(E2, se2);

  precondition(inter(se1, se2).card() == 0);
  return level::invert(hit_or_miss_opening(level::invert(input),
					   se1, se2));
}

template<class I, class E1, class E2> inline
Concrete(I) hit_or_miss_closing_bg(const image<I>& input,
				   const struct_elt<E1>& se1,
				   const struct_elt<E2>& se2)
{
  return hit_or_miss_closing(input, se2, se1);
}
