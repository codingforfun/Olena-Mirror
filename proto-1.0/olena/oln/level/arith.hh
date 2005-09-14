// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_LEVEL_ARITH_HH
# define OLENA_LEVEL_ARITH_HH

# include <mlc/cmp.hh>
# include <oln/core/abstract/size.hh>
# include <oln/core/ch_value_type.hh>

# include <oln/funobj/arith.hh>
# include <oln/core/apply.hh>

# include <oln/ops/arith.hh>
# include <oln/convert/conversion.hh>
# include <oln/utils/clone.hh>


# define oln_arith_output(Op, I1, I2) \
  typename arith_output<f_::Op##_,I1,I2>::ret



namespace oln
{

  namespace level
  {

    // min

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    min (const abstract::image<I1>& input1,
	 const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      mlc::eq<oln_type_of(I1, value), oln_type_of(I2, value)>::ensure();

      entering("level::min");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      oln_type_of(I1, concrete) output("output");
      output = clone( mlc::f_min(input1, input2) );

      exiting("level::min");
      return output;
    }


    // max

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    max (const abstract::image<I1>& input1,
	 const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      mlc::eq<oln_type_of(I1, value), oln_type_of(I2, value)>::ensure();

      entering("level::max");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      oln_type_of(I1, concrete) output("output");
      output = clone( mlc::f_max(input1, input2) );

      exiting("level::max");
      return output;
    }


    // inf

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    inf (const abstract::image<I1>& input1,
	 const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      mlc::eq<oln_type_of(I1, value), oln_type_of(I2, value)>::ensure();

      entering("level::inf");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      oln_type_of(I1, concrete) output("output");
      output = clone( mlc::f_inf(input1, input2) );

      exiting("level::inf");
      return output;
    }


    // sup

    template <typename I1, typename I2>
    oln_type_of(I1, concrete)
    sup (const abstract::image<I1>& input1,
	 const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
      mlc::eq<oln_type_of(I1, value), oln_type_of(I2, value)>::ensure();

      entering("level::sup");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      oln_type_of(I1, concrete) output("output");
      output = clone( mlc::f_sup(input1, input2) );

      exiting("level::sup");
      return output;
    }


    // arith output

    template <template <typename, typename> class F,
	      typename I1, typename I2>
    struct arith_output
    {
      typedef oln_type_of(I1, value) T1;
      typedef oln_type_of(I2, value) T2;
      typedef oln_fun2_type_2_of(F<T1,T2>, res) T;
      typedef oln_ch_concrete_type(I1, T) ret;
    };


//     template <typename I1, typename I2>
//     oln_arith_output(plus, I1, I2)
//     plus (const abstract::image<I1>& input1,
// 	  const abstract::image<I2>& input2)
//     {
//       // FIXME: recording(?)
//       mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();
//       typedef oln::f_::plus_<oln_type_of(I1, value), oln_type_of(I2, value)> fun_type;
//       return apply2(fun_type(), input1, input2);
//       // FIXME: does not work yet!
// //       return apply2(oln::f_plus, input1, input2);
//     }


    // plus

    template <typename I1, typename I2>
    oln_arith_output(plus,I1,I2)
    plus (const abstract::image<I1>& input1,
	  const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();

      entering("level::plus");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      oln_arith_output(plus,I1,I2) output("output");
      output = clone( input1 + input2 );

      exiting("level::plus");
      return output;
    }


    // minus

    template <typename I1, typename I2>
    oln_arith_output(minus,I1,I2)
    minus (const abstract::image<I1>& input1,
	   const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();

      entering("level::minus");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      oln_arith_output(minus,I1,I2) output("output");
      output = clone( input1 - input2 );

      exiting("level::minus");
      return output;
    }

    // FIXME: Should we keep this?
    /// Variant taking a conversion function in parameter.
    template <typename C, typename B, typename I1, typename I2>
    typename ch_value_type<I1, typename convoutput<C, B, typename arith_output<f_::minus_, I1, I2>::T>::ret>::ret
    minus (const convert::abstract::conversion<C, B>& conv,
	   const abstract::image<I1>& input1,
	   const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();

      entering("level::minus");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      typedef oln_type_of(I1, value) T1;
      typedef oln_type_of(I2, value) T2;

      // The return type, if there were no conversion.
      typedef typename arith_output<f_::minus_, I1, I2>::T oper_output_type;
      // The actual return type.
      typedef typename
	ch_value_type<I1,
	              typename convoutput<C, B, oper_output_type>::ret>::ret
	output_type;
      output_type output("output");
      output = apply2(convert::compconv2(conv, f_::minus_<T1, T2>()),
		      input1, input2);

      exiting("level::minus");
      return output;
    }


    // times

    template <typename I1, typename I2>
    oln_arith_output(times,I1,I2)
    times (const abstract::image<I1>& input1,
	   const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();

      entering("level::times");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      oln_arith_output(times,I1,I2) output("output");
      output = clone( input1 * input2 );

      exiting("level::times");
      return output;
    }


    // div

    template <typename I1, typename I2>
    oln_arith_output(div,I1,I2)
    div (const abstract::image<I1>& input1,
	 const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();

      entering("level::div");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      oln_arith_output(div,I1,I2) output("output");
      output = clone( input1 / input2 );

      exiting("level::div");
      return output;
    }


    // mod

    template <typename I1, typename I2>
    oln_arith_output(mod,I1,I2)
    mod (const abstract::image<I1>& input1,
	 const abstract::image<I2>& input2)
    {
      mlc::eq<oln_type_of(I1, grid),  oln_type_of(I2, grid)>::ensure();

      entering("level::mod");
      registering(input1, "input1");
      registering(input2, "input2");

      precondition(input1.size() == input2.size());

      oln_arith_output(mod,I1,I2) output("output");
      output = clone( input1 % input2 );

      exiting("level::mod");
      return output;
    }


  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLENA_LEVEL_ARITH_HH
