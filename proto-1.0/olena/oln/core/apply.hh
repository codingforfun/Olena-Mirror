// Copyright (C)  2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

# include <oln/basics.hh>
# include <oln/core/abstract/op.hh>
# include <oln/core/ch_value_type.hh>

namespace oln {

  // fwd decl
  namespace impl {
    template <typename AdaptableUnaryFun, typename I> struct apply_type;
  }

  // category
  template <typename AdaptableUnaryFun, typename I>
  struct set_category< impl::apply_type<AdaptableUnaryFun, I> >
  {
    typedef category::image ret;
  };

  // super_type
  template <typename AdaptableUnaryFun, typename I>
  struct set_super_type< impl::apply_type<AdaptableUnaryFun, I> >
  {
    typedef typename abstract::op<I, impl::apply_type<AdaptableUnaryFun, I> >
    ret;
  };

  namespace impl {

    template <typename AdaptableUnaryFun, typename I>
    struct apply_type :
      public abstract::op<
        typename ch_value_type<
          I, typename AdaptableUnaryFun::result_type>::ret,
        apply_type<AdaptableUnaryFun, I> >
    {
      typedef typename
        ch_value_type<I, typename AdaptableUnaryFun::result_type>::ret
        output_type;

      const AdaptableUnaryFun& f_;
      box<const I> input_;

      apply_type(const AdaptableUnaryFun& f, const abstract::image<I>& input) :
	f_ (f),
	input_ (input.exact ())
      {
      }

      void impl_run()
      {
	output_type output(input_.size());
	oln_type_of(I, fwd_piter) p(input_.size());
	for_all(p)
	  output[p] = f_(input_[p]);
	this->image_ = output;
      }
    };

  } // end of namespace impl

  template <class AdaptableUnaryFun, typename I>
  impl::apply_type<AdaptableUnaryFun, I>
  apply (const AdaptableUnaryFun& f, const abstract::image<I>& input)
  {
    impl::apply_type<AdaptableUnaryFun, I> tmp (f, input);
    tmp.run ();
    return tmp;
  }
  
} // end of namespace oln

#endif // ! OLENA_CORE_APPLY_HH
