// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CONVERT_CONVERSION_HH
# define OLENA_CONVERT_CONVERSION_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/ch_value_type.hh>
# include <oln/funobj/abstract/unary.hh>
# include <oln/funobj/abstract/binary.hh>
# include <oln/convert/abstract/conversion.hh>

namespace oln {
  /*!
  ** \brief Conversion implementation (for example cast, color, or
  ** neighborhood to window).
  */
  namespace convert {

    /// \brief Internal purpose only.
    namespace internal {
      template <typename C, typename UF> struct compconv1_;
      template <typename C, typename BF> struct compconv2_;
    } // end of namespace oln::convert::internal

  } // end of namespace oln::convert

  template <typename C, typename UF>
  struct set_super_type < convert::internal::compconv1_<C, UF> > { typedef f_::abstract::unary< convert::internal::compconv1_<C, UF> > ret; };

  template <typename C, typename BF>
  struct set_super_type < convert::internal::compconv2_<C, BF> > { typedef f_::abstract::binary< convert::internal::compconv2_<C, BF> > ret; };

  template <typename C, typename UF>
  struct set_props < category::fun1, convert::internal::compconv1_<C, UF> >
  {
    typedef oln_fun1_type_of(UF, arg) arg_type;
    typedef typename C::template output<oln_fun1_type_of(UF, res)>::ret
      res_type;
  };

  template <typename C, typename BF>
  struct set_props < category::fun2, convert::internal::compconv2_<C, BF> >
  {
    typedef oln_fun2_type_of(BF, left) left_type;
    typedef oln_fun2_type_of(BF, right) right_type;
    typedef typename C::template output<oln_fun2_type_of(BF, res)>::ret
      res_type;
  };

  namespace convert {

    /*! Trait that returns the output of a conversion.
    **
    ** convoutput queries the output type of conversion ConvType for
    ** an input of type InputType.  This comes handy when computing
    ** the return type of a function which takes a conversion function
    ** in argument.
    **
    ** \note convoutput is exported in the namespace oln for convenience.
    */
    template<class ConvType, class Base, class InputType>
    struct convoutput
    {
      typedef typename abstract::conversion<ConvType, Base>::template output<InputType>::ret ret;
    };

    namespace internal {

      /*! Compose a conversion C and an unary functor UF, producing an
      ** unary functor.
      */
      template <class C, class UF>
      struct compconv1_ : public oln_super2_of_(compconv1_<C, UF>)
      {
	typedef compconv1_<C, UF> self_type;
	typedef oln_fun1_type_of(self_type, arg) arg_type;
	typedef oln_fun1_type_of(self_type, res) res_type;

	compconv1_(const C& conv, const UF& func) :
	  conv_(conv.exact()), func_(func.exact())
	  {
	  }

	const res_type impl_unop(const arg_type& arg) const
	{
	  return conv_(func_(arg));
	}

      private:
	const C conv_;
	const UF func_;
      };

      /*! Compose a conversion C and a binary functor BF, producing
      ** a binary functor.
      */
      template <class C, class BF>
      struct compconv2_ : public oln_super2_of_(compconv2_<C, BF>)
      {
	typedef compconv2_<C, BF> self_type;
	typedef oln_fun2_type_of(self_type, left) left_type;
	typedef oln_fun2_type_of(self_type, right) right_type;
	typedef oln_fun2_type_of(self_type, res) res_type;

	compconv2_(const C& conv, const BF& func) :
	  conv_(conv.exact()), func_(func.exact())
	  {
	  }

	const res_type impl_binop(const left_type& left,
				  const right_type& right) const
	  {
	    return conv_(func_(left, right));
	  }

      private:
	const C conv_;
	const BF func_;
      };

    } // end of namespace oln::convert::internal

    /*! Friendly procedure that build an internal::compconv1_ with
    ** type deduction.
    */
    template <class C, class B, class UF>
    internal::compconv1_<C, UF>
    compconv1(const abstract::conversion<C, B>& conv, const UF &func)
    {
      return internal::compconv1_<C, UF>(conv.exact(), func);
    }

    /*! Likewise for compconv2_.  */
    template <class C, class B, class BF>
    internal::compconv2_<C, BF>
    compconv2(const abstract::conversion<C, B>& conv, const BF &func)
    {
      return internal::compconv2_<C, BF>(conv.exact(), func);
    }

  } // end of namespace oln::convert

  // Export conversion and convouput into oln:: to simplify the
  // writing of processings.
  using convert::convoutput;

} // end of namespace oln

#endif // OLENA_CONVERT_CONVERSION_HH
