// Copyright 2001, 2002  EPITA Research and Development Laboratory
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
   and define internal::operator_##OPNAME##_traits.  */
# define _OLN_ARITH_DECLARE_BINRECVAL_FUNCTOR(OPNAME, OPCODE)		\
    template<class T1, class T2 = T1>					\
    struct f_##OPNAME : std::binary_function<const rec_value<T1>&,	\
      const rec_value<T2>&,						\
      typename internal::deduced_from_traits<				\
        internal::operator_##OPNAME##_traits, T1, T2 >::ret_t >		\
    {									\
      typedef f_##OPNAME self;						\
      typename self::result_type					\
      operator()(typename self::first_argument_type val1,		\
                 typename self::second_argument_type val2) const	\
      {									\
	return OPCODE;							\
      }									\
    } /* no ; */

/* For binary operators that takes rev_values<T> arguments
   and define internal::operator_##OPNAME##_traits.  */
# define _OLN_ARITH_DECLARE_BINRECVALCST_FUNCTOR(OPNAME, OPCODE_CST)	\
    template<class T1, class T2 = T1>					\
    struct f_##OPNAME##_cst : std::unary_function<const rec_value<T1>&,	\
      typename internal::deduced_from_traits<				\
        internal::operator_##OPNAME##_traits, T1, T2>::ret_t >		\
    {									\
      typedef f_##OPNAME##_cst self;					\
      f_##OPNAME##_cst(T2 cst) : _cst(cst) {}				\
									\
      typename self::result_type					\
      operator()(typename self::argument_type val) const		\
      {									\
	return OPCODE_CST;						\
      }									\
    private:								\
      T2 _cst;								\
    } /* no ; */

/* Both the above. */
# define _OLN_ARITH_DECLARE_BINRECVAL_FUNCTORS(OPNAME, OPCODE, OPCODE_CST) \
    _OLN_ARITH_DECLARE_BINRECVAL_FUNCTOR(OPNAME, OPCODE);	   \
    _OLN_ARITH_DECLARE_BINRECVALCST_FUNCTOR(OPNAME, OPCODE_CST)


/* For binary functions that work on a single known datatype.  */
#define _OLN_ARITH_DECLARE_BINFIXEDTYPE_FUNCTOR(OPNAME, OPCODE, TYPE)	      \
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
#define _OLN_ARITH_DECLARE_BINFIXEDTYPECST_FUNCTOR(OPNAME, OPCODE_CST, TYPE) \
    struct f_##OPNAME##_cst: std::unary_function<const TYPE, TYPE>	     \
    {									     \
      f_##OPNAME##_cst(TYPE cst) : _cst(cst) {}				     \
									     \
      const result_type							     \
      operator()(argument_type val) const				     \
      {									     \
	return OPCODE_CST;						     \
      }									     \
    private:								     \
      TYPE _cst;							     \
    } /* no ; */

/* Both the above.  */
#define _OLN_ARITH_DECLARE_BINFIXEDTYPE_FUNCTORS(NAME, TYPE, CODE, CODE_CST) \
    _OLN_ARITH_DECLARE_BINFIXEDTYPE_FUNCTOR(NAME, CODE, TYPE);		     \
    _OLN_ARITH_DECLARE_BINFIXEDTYPECST_FUNCTOR(NAME, CODE_CST, TYPE)

# define _OLN_ARITH_DECLARE_BINOP_PROCS(OPNAME)				   \
    /* Standard application of OPNAME */				   \
    template<class I1, class I2> inline					   \
    typename mute<I1,							   \
      typename f_##OPNAME<Value(I1), Value(I2)>::result_type>::ret	   \
    OPNAME(const image<I1>& input1, const image<I2>& input2)		   \
    {									   \
      /* KLUGE: Build the functor, don't pass it as a parameter as in	   \
	   apply2<f_##OPNAME>(input1, input2)				   \
	 otherwise GCC 2.95.x will ICE.  */				   \
      return apply2(f_##OPNAME<Value(I1), Value(I2)>(), input1, input2);   \
    }									   \
									   \
    /* Same as above, plus conversion.  */				   \
    template<class C, class I1, class I2> inline			   \
    typename mute<I1,							   \
      typename convoutput<C,						   \
        typename f_##OPNAME<Value(I1), Value(I2)>::result_type>::ret>::ret \
    OPNAME(const conversion<C>& conv,					   \
	 const image<I1>& input1, const image<I2>& input2)		   \
    {									   \
      return apply2(convert::compconv2(conv,				   \
				    f_##OPNAME<Value(I1), Value(I2)>()),   \
		    input1, input2);					   \
    }

# define _OLN_ARITH_DECLARE_BINOPCST_PROCS(OPNAME)			  \
    /* Apply OPNAME with a constant as second operand.  */		  \
    template<class I, class T> inline					  \
    typename mute<I,							  \
      typename f_##OPNAME##_cst<Value(I), T>::result_type>::ret		  \
    OPNAME##_cst(const image<I>& input, T val)				  \
    {									  \
      return apply(f_##OPNAME##_cst<Value(I),T>(val), input);		  \
    }									  \
									  \
    /* Same as above, plus conversion.  */				  \
    template<class C, class I, class T> inline				  \
    typename mute<I,							  \
      typename convoutput<C,						  \
        typename f_##OPNAME##_cst<Value(I), T>::result_type>::ret>::ret	  \
    OPNAME##_cst(const conversion<C>& conv, const image<I>& input, T val) \
    {									  \
      return apply(convert::compconv1(conv,				  \
				   f_##OPNAME##_cst<Value(I),T>(val)),	  \
		   input);						  \
    }

# define _OLN_ARITH_DECLARE_ALL_BINOP_PROCS(OPNAME)	\
    _OLN_ARITH_DECLARE_BINOP_PROCS(OPNAME)		\
    _OLN_ARITH_DECLARE_BINOPCST_PROCS(OPNAME)


/* Same as _OLN_ARITH_DECLARE_BINOP_PROCS, for functor parametred by
   a single type.  */
# define _OLN_ARITH_DECLARE_SEMIGENERIC_BINOP_PROCS(OPNAME)		\
    /* Standard application of OPNAME */				\
    template<class I1, class I2> inline					\
    typename mute<I1,							\
      typename f_##OPNAME<Value(I1)>::result_type>::ret			\
    OPNAME(const image<I1>& input1, const image<I2>& input2)		\
    {									\
      return apply2<f_##OPNAME>(input1, input2);			\
    }									\
									\
    /* Same as above, plus conversion.  */				\
    template<class C, class I1, class I2> inline			\
    typename mute<I1,							\
      typename convoutput<C,						\
        typename f_##OPNAME<Value(I1)>::result_type>::ret>::ret		\
    OPNAME(const conversion<C>& conv,					\
	 const image<I1>& input1, const image<I2>& input2)		\
    {									\
      return apply2(convert::compconv2(conv, f_##OPNAME<Value(I1)>()),	\
		    input1, input2);					\
    }

/* Same as _OLN_ARITH_DECLARE_NONGENERICBINOP_PROCS but for non tempalte
   functors.  */
# define _OLN_ARITH_DECLARE_NONGENERICBINOP_PROCS(OPNAME)		  \
    /* Standard application of OPNAME */				  \
    template<class I1, class I2> inline					  \
    typename mute<I1, typename f_##OPNAME::result_type>::ret		  \
    OPNAME(const image<I1>& input1, const image<I2>& input2)		  \
    {									  \
      return apply2<f_##OPNAME>(input1, input2);			  \
    }									  \
									  \
    /* Same as above, plus conversion.  */				  \
    template<class C, class I1, class I2> inline			  \
    typename mute<I1,							  \
      typename convoutput<C, typename f_##OPNAME::result_type>::ret>::ret \
    OPNAME(const conversion<C>& conv,					  \
	 const image<I1>& input1, const image<I2>& input2)		  \
    {									  \
      return apply2(convert::compconv2(conv, f_##OPNAME()),		  \
		    input1, input2);					  \
    }

/* Same as _OLN_ARITH_DECLARE_NONGENERICBINOPCST_PROCS but for non
   tempalte functors.  */
# define _OLN_ARITH_DECLARE_NONGENERICBINOPCST_PROCS(OPNAME)		    \
    /* Apply OPNAME with a constant as second operand.  */		    \
    template<class I, class T> inline					    \
    typename mute<I, typename f_##OPNAME##_cst::result_type>::ret	    \
    OPNAME##_cst(const image<I>& input, T val)				    \
    {									    \
      return apply(f_##OPNAME##_cst(val), input);			    \
    }									    \
									    \
    /* Same as above, plus conversion.  */				    \
    template<class C, class I, class T> inline				    \
    typename mute<I,							    \
      typename convoutput<C,						    \
        typename f_##OPNAME##_cst::result_type>::ret>::ret		    \
    OPNAME##_cst(const conversion<C>& conv, const image<I>& input, T val)   \
    {									    \
      return apply(convert::compconv1(conv, f_##OPNAME##_cst(val)),	    \
		   input);						    \
    }

/* Same as _OLN_ARITH_DECLARE_ALL_NONGENERICBINOP_PROCS but for non
   tempalte functors.  */
# define _OLN_ARITH_DECLARE_ALL_NONGENERICBINOP_PROCS(OPNAME)	\
    _OLN_ARITH_DECLARE_NONGENERICBINOP_PROCS(OPNAME)		\
    _OLN_ARITH_DECLARE_NONGENERICBINOPCST_PROCS(OPNAME)


/*------------------.
| Unary functions.  |
`------------------*/

/* For binary functions that work on a single known datatype.  */
#define _OLN_ARITH_DECLARE_UNFIXEDTYPE_FUNCTOR(OPNAME, TYPE, OPCODE)	\
    struct f_##OPNAME : std::unary_function< const TYPE&, TYPE>		\
    {									\
      const result_type operator()(argument_type val) const		\
      {									\
	return OPCODE;							\
      }									\
    } /* no ; */

# define _OLN_ARITH_DECLARE_NONGENERICUNOP_PROCS(OPNAME)		  \
    /* Standard application of OPNAME */				  \
    template<class I> inline						  \
    typename mute<I, typename f_##OPNAME::result_type>::ret		  \
    OPNAME(const image<I>& input1)					  \
    {									  \
      return apply<f_##OPNAME>(input1);					  \
    }									  \
									  \
    /* Same as above, plus conversion.  */				  \
    template<class C, class I> inline					  \
    typename mute<I,							  \
      typename convoutput<C, typename f_##OPNAME::result_type>::ret>::ret \
    OPNAME(const conversion<C>& conv, const image<I>& input1)		  \
    {									  \
      return apply(convert::compconv2(conv, f_##OPNAME()), input1);	  \
    }

# define _OLN_ARITH_DECLARE_UNOP_PROCS(OPNAME)				      \
    /* Standard application of OPNAME */				      \
    template<class I> inline						      \
    typename mute<I, typename f_##OPNAME<Value(I)>::result_type>::ret	      \
    OPNAME(const image<I>& input1)					      \
    {									      \
      return apply(f_##OPNAME<Value(I)>(), input1);			      \
    }									      \
									      \
    /* Same as above, plus conversion.  */				      \
    template<class C, class I> inline					      \
    typename mute<I,							      \
      typename convoutput<C,						      \
        typename f_##OPNAME<Value(I)>::result_type>::ret>::ret		      \
    OPNAME(const conversion<C>& conv, const image<I>& input1)		      \
    {									      \
      return apply(convert::compconv2(conv, f_##OPNAME<Value(I)>()), input1); \
    }


#endif // OLENA_ARITH_INTERNAL_OPDECLS_HH
