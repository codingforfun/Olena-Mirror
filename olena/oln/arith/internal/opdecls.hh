// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_ARITH_INTERNAL_OPDECLS_HH
# define OLENA_ARITH_INTERNAL_OPDECLS_HH

/*-------------------.
| Binary functions.  |
`-------------------*/

/* For binary operators that takes rev_values<T> arguments
   and define ntg::internal::operator_##OPNAME##_traits.  */
# define oln_arith_declare_binrecval_functor_(OPNAME, OPCODE)		\
    template<class T1, class T2 = T1>					\
    struct f_##OPNAME : std::binary_function<const ntg::value<T1>&,	\
      const ntg::value<T2>&,						\
      ntg_return_type(OPNAME, T1, T2) >					\
    {									\
      typedef f_##OPNAME self_type;					\
      typename self_type::result_type					\
      operator()(typename self_type::first_argument_type val1,		\
                 typename self_type::second_argument_type val2) const	\
      {									\
	return OPCODE;							\
      }									\
    } /* no ; */

/* For binary operators that takes rev_values<T> arguments			\
   and define ntg::internal::operator_##OPNAME##_traits.  */			\
# define oln_arith_declare_binrecvalcst_functor_(OPNAME, OPCODE_CST)		\
    template<class T1, class T2 = T1>						\
    struct f_##OPNAME##_cst : std::unary_function<const ntg::value<T1>&,	\
      ntg_return_type(OPNAME, T1, T2) >						\
    {										\
      typedef f_##OPNAME##_cst self_type;						\
      f_##OPNAME##_cst(T2 cst) : cst_(cst) {}					\
										\
      typename self_type::result_type						\
      operator()(typename self_type::argument_type val) const			\
      {										\
	return OPCODE_CST;							\
      }										\
    private:									\
      T2 cst_;									\
    } /* no ; */

/* Both the above. */
# define oln_arith_declare_binrecval_functors_(OPNAME, OPCODE, OPCODE_CST) \
    oln_arith_declare_binrecval_functor_(OPNAME, OPCODE);	   \
    oln_arith_declare_binrecvalcst_functor_(OPNAME, OPCODE_CST)


/* For binary functions that work on a single known datatype.  */
# define oln_arith_declare_binfixedtype_functor_(OPNAME, OPCODE, TYPE)	      \
    struct f_##OPNAME : std::binary_function< const TYPE&, const TYPE&, TYPE> \
    {									      \
      const result_type							      \
      operator()(first_argument_type val1,				      \
		 second_argument_type val2) const			      \
      {									      \
	return OPCODE;							      \
      }									      \
    } /* no ; */

/* For binary functions that work on a single known datatype.  */
# define oln_arith_declare_binfixedtypecst_functor_(OPNAME, OPCODE_CST, TYPE) \
    struct f_##OPNAME##_cst: std::unary_function<const TYPE, TYPE>	     \
    {									     \
      f_##OPNAME##_cst(TYPE cst) : cst_(cst) {}				     \
									     \
      const result_type							     \
      operator()(argument_type val) const				     \
      {									     \
	return OPCODE_CST;						     \
      }									     \
    private:								     \
      TYPE cst_;							     \
    } /* no ; */

/* Both the above.  */
# define oln_arith_declare_binfixedtype_functors_(NAME, TYPE, CODE, CODE_CST) \
    oln_arith_declare_binfixedtype_functor_(NAME, CODE, TYPE);		     \
    oln_arith_declare_binfixedtypecst_functor_(NAME, CODE_CST, TYPE)

# define oln_arith_declare_binop_procs_(OPNAME)				   \
    /* Standard application of OPNAME */				   \
    template<class I1, class I2> inline					   \
    typename mute<I1,							   \
      typename f_##OPNAME<oln_value_type(I1),oln_value_type(I2)>::result_type>::ret	   \
    OPNAME(const abstract::image<I1>& input1, const abstract::image<I2>& input2)   \
    {									   \
      /* KLUGE: Build the functor, don't pass it as a parameter as in	   \
	   apply2<f_##OPNAME>(input1, input2)				   \
	 otherwise GCC 2.95.x will ICE.  */				   \
      return apply2(f_##OPNAME<oln_value_type(I1),oln_value_type(I2)>(), input1, input2);   \
    }									   \
									   \
    /* Same as above, plus conversion.  */				   \
    template<class C, class B, class I1, class I2> inline		   \
    typename mute<I1,							   \
      typename convoutput<C, B,						   \
        typename f_##OPNAME<oln_value_type(I1),oln_value_type(I2)>::result_type>::ret>::ret \
    OPNAME(const convert::abstract::conversion<C, B>& conv,			   \
	 const abstract::image<I1>& input1, const abstract::image<I2>& input2)	   \
    {									   \
      return apply2(convert::compconv2(conv,				   \
				    f_##OPNAME<oln_value_type(I1),oln_value_type(I2)>()),   \
		    input1, input2);					   \
    }

# define oln_arith_declare_binopcst_procs_(OPNAME)			  \
    /* Apply OPNAME with a constant as second operand.  */		  \
    template<class I, class T> inline					  \
    typename mute<I,							  \
      typename f_##OPNAME##_cst<oln_value_type(I), T>::result_type>::ret		  \
    OPNAME##_cst(const abstract::image<I>& input, T val)			  \
    {									  \
      return apply(f_##OPNAME##_cst<oln_value_type(I),T>(val), input);		  \
    }									  \
									  \
    /* Same as above, plus conversion.  */				  \
    template<class C, class B, class I, class T> inline				  \
    typename mute<I,							  \
      typename convoutput<C, B,						  \
        typename f_##OPNAME##_cst<oln_value_type(I), T>::result_type>::ret>::ret	  \
    OPNAME##_cst(const convert::abstract::conversion<C, B>& conv, const abstract::image<I>& input, T val) \
    {									  \
      return apply(convert::compconv1(conv,				  \
				   f_##OPNAME##_cst<oln_value_type(I),T>(val)),	  \
		   input);						  \
    }

# define oln_arith_declare_all_binop_procs_(OPNAME)	\
    oln_arith_declare_binop_procs_(OPNAME)		\
    oln_arith_declare_binopcst_procs_(OPNAME)


/* Same as oln_arith_declare_binop_procs_, for functor parametred by
   a single type.  */
# define oln_arith_declare_semigeneric_binop_procs_(OPNAME)		\
    /* Standard application of OPNAME */				\
    template<class I1, class I2> inline					\
    typename mute<I1,							\
      typename f_##OPNAME<oln_value_type(I1)>::result_type>::ret			\
    OPNAME(const abstract::image<I1>& input1, const abstract::image<I2>& input2) \
    {									\
      return apply2<f_##OPNAME>(input1, input2);			\
    }									\
									\
    /* Same as above, plus conversion.  */				\
    template<class C, class B, class I1, class I2> inline			\
    typename mute<I1,							\
      typename convoutput<C, B,						\
        typename f_##OPNAME<oln_value_type(I1)>::result_type>::ret>::ret		\
    OPNAME(const convert::abstract::conversion<C, B>& conv,				\
	 const abstract::image<I1>& input1, const abstract::image<I2>& input2)		\
    {									\
      return apply2(convert::compconv2(conv, f_##OPNAME<oln_value_type(I1)>()),	\
		    input1, input2);					\
    }

/* Same as oln_arith_declare_nongenericbinop_procs_ but for non template
   functors.  */
# define oln_arith_declare_nongenericbinop_procs_(OPNAME)		  \
    /* Standard application of OPNAME */				  \
    template<class I1, class I2> inline					  \
    typename mute<I1, typename f_##OPNAME::result_type>::ret		  \
    OPNAME(const abstract::image<I1>& input1, const abstract::image<I2>& input2)  \
    {									  \
      return apply2<f_##OPNAME>(input1, input2);			  \
    }									  \
									  \
    /* Same as above, plus conversion.  */				  \
    template<class C, class B, class I1, class I2> inline			  \
    typename mute<I1,							  \
      typename convoutput<C, B, typename f_##OPNAME::result_type>::ret>::ret \
    OPNAME(const convert::abstract::conversion<C, B>& conv,			  \
	 const abstract::image<I1>& input1, const abstract::image<I2>& input2)	  \
    {									  \
      return apply2(convert::compconv2(conv, f_##OPNAME()),		  \
		    input1, input2);					  \
    }

/* Same as oln_arith_declare_nongenericbinopcst_procs_ but for non
   template functors.  */
# define oln_arith_declare_nongenericbinopcst_procs_(OPNAME)		    \
    /* Apply OPNAME with a constant as second operand.  */		    \
    template<class I, class T> inline					    \
    typename mute<I, typename f_##OPNAME##_cst::result_type>::ret	    \
    OPNAME##_cst(const abstract::image<I>& input, T val)		    \
    {									    \
      return apply(f_##OPNAME##_cst(val), input);			    \
    }									    \
									    \
    /* Same as above, plus conversion.  */				    \
    template<class C, class B, class I, class T> inline				    \
    typename mute<I,							    \
      typename convoutput<C, B,						    \
        typename f_##OPNAME##_cst::result_type>::ret>::ret		    \
    OPNAME##_cst(const convert::abstract::conversion<C, B>& conv, const abstract::image<I>& input, T val)   \
    {									    \
      return apply(convert::compconv1(conv, f_##OPNAME##_cst(val)),	    \
		   input);						    \
    }

/* Same as oln_arith_declare_all_nongenericbinop_procs_ but for non
   template functors.  */
# define oln_arith_declare_all_nongenericbinop_procs_(OPNAME)	\
    oln_arith_declare_nongenericbinop_procs_(OPNAME)		\
    oln_arith_declare_nongenericbinopcst_procs_(OPNAME)


/*------------------.
| Unary functions.  |
`------------------*/

/* For binary functions that work on a single known datatype.  */
# define oln_arith_declare_unfixedtype_functor_(OPNAME, TYPE, OPCODE)	\
    struct f_##OPNAME : std::unary_function< const TYPE&, TYPE>		\
    {									\
      const result_type operator()(argument_type val) const		\
      {									\
	return OPCODE;							\
      }									\
    } /* no ; */

# define oln_arith_declare_nongenericunop_procs_(OPNAME)		  \
    /* Standard application of OPNAME */				  \
    template<class I> inline						  \
    typename mute<I, typename f_##OPNAME::result_type>::ret		  \
    OPNAME(const abstract::image<I>& input1)					  \
    {									  \
      return apply<f_##OPNAME>(input1);					  \
    }									  \
									  \
    /* Same as above, plus conversion.  */				  \
    template<class C, class B, class I> inline				  \
    typename mute<I,							  \
      typename convoutput<C, B, typename f_##OPNAME::result_type>::ret>::ret \
    OPNAME(const convert::abstract::conversion<C, B>& conv, const abstract::image<I>& input1)  \
    {									  \
      return apply(convert::compconv2(conv, f_##OPNAME()), input1);	  \
    }

# define oln_arith_declare_unop_procs_(OPNAME)				      \
    /* Standard application of OPNAME */				      \
    template<class I> inline						      \
    typename mute<I, typename f_##OPNAME<oln_value_type(I)>::result_type>::ret	      \
    OPNAME(const abstract::image<I>& input1)					      \
    {									      \
      return apply(f_##OPNAME<oln_value_type(I)>(), input1);			      \
    }									      \
									      \
    /* Same as above, plus conversion.  */				      \
    template<class C, class B, class I> inline					      \
    typename mute<I,							      \
      typename convoutput<C, B,						      \
        typename f_##OPNAME<oln_value_type(I)>::result_type>::ret>::ret		      \
    OPNAME(const convert::abstract::conversion<C>& conv, const abstract::image<I>& input1)      \
    {									      \
      return apply(convert::compconv2(conv, f_##OPNAME<oln_value_type(I)>()), input1); \
    }


#endif // OLENA_ARITH_INTERNAL_OPDECLS_HH
