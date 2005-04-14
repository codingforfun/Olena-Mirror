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

# include <mlc/fun.hh>

# include <oln/basics.hh>
# include <oln/core/abstract/image_operator.hh>
# include <oln/core/ch_value_type.hh>

namespace oln {

  /*--------.
  | Unary.  |
  `--------*/

  // Fwd decl.
  namespace impl {
    template <typename F, typename I>
    struct apply1_type;
  }

  // Super type.
  template <typename F, typename I>
  struct set_super_type< impl::apply1_type<F, I> >
  {
    typedef typename ch_value_type<I,
				   typename F::result_type>::ret output_type;
    typedef typename abstract::image_operator<output_type, impl::apply1_type<F, I> > ret;
  };

  namespace impl {

    template <typename F, typename I>
    struct apply1_type :
      public abstract::image_operator<
        typename ch_value_type<I, typename F::result_type>::ret,
        apply1_type<F, I> >
    {
      typedef typename ch_value_type<I, typename F::result_type>::ret
        output_type;

      F f_;
      box<const I> input_;

      apply1_type(const mlc::abstract::unary_function<F>& f,
		  const abstract::image<I>& input) :
	f_(f.exact ()),
	input_(input.exact())
      {
      }

      void impl_run()
      {
	output_type tmp(input_.size());
	oln_type_of(I, fwd_piter) p(input_.size());
	for_all_p (p)
	  tmp[p] = f_(input_[p]);
	this->output = tmp;
      }
    };

  } // end of namespace impl


  /*! \brief Standard unary \a apply procedure.
  **
  ** Apply a function \a f to each element of \a input, the function
  ** is passed as a type and is instantiated.
  */
  template <typename F, typename I>
  impl::apply1_type<F, I>
  apply(const mlc::abstract::unary_function<F>& f,
	const abstract::image<I>& input)
  {
    impl::apply1_type<F, I> tmp(f, input);
    tmp.run();
    return tmp;
  }


  /*---------.
  | Binary.  |
  `---------*/

  // Fwd decl.
  namespace impl {
    template <typename F, typename I1, typename I2>
    struct apply2_type;
  }

  // Super type.
  template <typename F, typename I1, typename I2>
  struct set_super_type< impl::apply2_type<F, I1, I2> >
  {
    typedef typename ch_value_type<I1,
				   typename F::result_type>::ret output_type;
    typedef typename abstract::image_operator<output_type,
				  impl::apply2_type<F, I1, I2> > ret;
  };

  namespace impl {

    template <typename F, typename I1, typename I2>
    struct apply2_type :
      public abstract::image_operator<
        typename ch_value_type<I1, typename F::result_type>::ret,
        apply2_type<F, I1, I2> >
    {
      typedef typename ch_value_type<I1, typename F::result_type>::ret
        output_type;

      F f_;
      box<const I1> input1_;
      box<const I2> input2_;

      apply2_type(const mlc::abstract::binary_function<F>& f,
		  const abstract::image<I1>& input1,
		  const abstract::image<I2>& input2) :
	f_(f.exact ()),
	input1_(input1.exact()),
	input2_(input2.exact())
      {
	assertion (input1_.size() == input2_.size());
      }

      void impl_run()
      {
	output_type tmp(input1_.size());
	oln_type_of(I1, fwd_piter) p(input1_.size());
	for_all_p (p)
	  tmp[p] = f_(input1_[p], input2_[p]);
	this->output = tmp;
      }
    };

  } // end of namespace impl


  /*! \brief Standard binary \a apply procedure.
  **
  ** Apply a function \a f to each pair of elements of
  ** \a input1 x \a input2.
  */
  template <typename F, typename I1, typename I2>
  impl::apply2_type<F, I1, I2>
  apply(const mlc::abstract::binary_function<F>& f,
	const abstract::image<I1>& input1,
	const abstract::image<I2>& input2)
  {
    assertion (input1.size() == input2.size());
    impl::apply2_type<F, I1, I2> tmp(f, input1, input2);
    tmp.run();
    return tmp;
  }

} // end of namespace oln

#endif // ! OLENA_CORE_APPLY_HH
