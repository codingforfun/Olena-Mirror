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

# include <mlc/cmp.hh>
# include <mlc/fun.hh>

# include <oln/core/abstract/image.hh>
# include <oln/funobj/abstract/unary.hh>
# include <oln/funobj/abstract/binary.hh>
# include <oln/core/ch_value_type.hh>


namespace oln {


  /*--------.
  | Unary.  |
  `--------*/

  /*! \brief Standard unary \a apply procedure.
  **
  ** Apply a function \a f to each element of \a input, the function
  ** is passed as a type and is instantiated.
  */
  template <typename F, typename I>
  typename ch_value_type<I, typename F::result_type>::ret
  apply(const mlc::abstract::unary_function<F>& f,
	const abstract::image<I>& input)
  {
    entering("apply");
    registering(input, "input");

    typedef typename ch_value_type<I, typename F::result_type>::ret
      output_type;
    output_type output(input.size(), "output");

    oln_type_of(I, fwd_piter) p(input.size());
    for_all_p (p)
      output[p] = f(input[p]);

    exiting("apply");
    return output;
  }

  // version with  oln::f_::abstract::unary<F>

  template <typename F, typename I>
  typename ch_value_type<I, oln_fun1_type_of(F, res)>::ret
  apply(const oln::f_::abstract::unary<F>& f,
	const abstract::image<I>& input)
  {
    entering("apply");
    registering(input, "input");

    typedef typename ch_value_type<I, oln_fun1_type_of(F, res)>::ret
      output_type;
    output_type output(input.size(), "output");

    oln_type_of(I, fwd_piter) p(input.size());
    for_all_p (p)
      output[p] = f(input[p]);

    exiting("apply");
    return output;
  }


  /*---------.
  | Binary.  |
  `---------*/

  /*! \brief Standard binary \a apply procedure.
  **
  ** Apply a function \a f to each pair of elements of
  ** \a input1 x \a input2.
  */
  template <typename F, typename I1, typename I2>
  typename ch_value_type<I1, typename F::result_type>::ret
  apply2(const mlc::abstract::binary_function<F>& f,
	 const abstract::image<I1>& input1,
	 const abstract::image<I2>& input2)
  {
    mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();

    entering("apply2");
    registering(input1, "input1");
    registering(input2, "input2");

    precondition(input1.size() == input2.size());

    typedef typename ch_value_type<I1, typename F::result_type>::ret
      output_type;
    output_type output(input1.size(), "output");

    oln_type_of(I1, fwd_piter) p(input1.size());
    for_all_p (p)
      output[p] = f(input1[p], input2[p]);

    exiting("apply2");
    return output;
  }


  // version with  oln::f_::abstract::binary<F>

  template <typename F, typename I1, typename I2>
  typename ch_value_type<I1, oln_fun2_type_of(F, res)>::ret
  apply2(const oln::f_::abstract::binary<F>& f,
	 const abstract::image<I1>& input1,
	 const abstract::image<I2>& input2)
  {
    mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();

    entering("apply2");
    registering(input1, "input1");
    registering(input2, "input2");

    precondition(input1.size() == input2.size());

    typedef typename ch_value_type<I1, oln_fun2_type_of(F, res)>::ret
      output_type;
    output_type output(input1.size(), "output");

    oln_type_of(I1, fwd_piter) p(input1.size());
    for_all_p (p)
      output[p] = f(input1[p], input2[p]);

    exiting("apply2");
    return output;
  }


  // version with  oln::f_::abstract::mbinary<F>

  template <typename F, typename I1, typename I2>
  typename ch_value_type<I1, typename f_::mbinary_result<F,
							 oln_type_of(I1, value),
							 oln_type_of(I2, value)>::ret
    >::ret
  apply2(const oln::f_::abstract::mbinary<F>& f,
	 const abstract::image<I1>& input1,
	 const abstract::image<I2>& input2)
  {
    mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();

    entering("apply2");
    registering(input1, "input1");
    registering(input2, "input2");

    precondition(input1.size() == input2.size());

    typedef
      typename ch_value_type<I1, typename f_::mbinary_result<F,
                                                    oln_type_of(I1, value),
						    oln_type_of(I2, value)>::ret
      >::ret output_type;
    output_type output(input1.size(), "output");

    oln_type_of(I1, fwd_piter) p(input1.size());
    for_all_p (p)
      output[p] = f(input1[p], input2[p]);

    exiting("apply2");
    return output;
  }


} // end of namespace oln

#endif // ! OLENA_CORE_APPLY_HH
