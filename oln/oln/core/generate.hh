// Copyright 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_GENERATE_HH
# define OLENA_CORE_GENERATE_HH

# include <oln/meta/type.hh>

# include <oln/core/contract.hh>
# include <oln/core/image.hh>
# include <oln/core/image_size.hh>
# include <oln/core/iter.hh>
# include <oln/core/macros.hh>

namespace oln {

  // generate (generator)

  template<class AdaptableGen, class I> inline
  typename image_for_dim<type::wrap<typename type::exact<I>::ret>::dim>::with_type<typename AdaptableGen::result_type>::ret
  generate(AdaptableGen f, const image_size<I>& size)
  {
    typename image_for_dim<(typename type::exact<I>::ret)::dim>::with_type<typename AdaptableGen::result_type>::ret output(to_exact(size));
    Iter(typename image_for_dim<(typename type::exact<I>::ret)::dim>::with_type<typename AdaptableGen::result_type>::ret) p(output);
    for_all(p) output[p] = f();
    return output;
  }

  template<class AdaptableGen, class _I> inline
  image<_I>
  generate(AdaptableGen f, image<_I>& _input)
  {
    Exact_ref (I, input);
    Iter(I) p(input);
    for_all(p) input[p] = f();
    return input;
  }

} // end of oln

#endif // OLENA_CORE_GENERATE_HH
