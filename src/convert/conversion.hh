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

#ifndef OLENA_CONVERT_CONVERSION_HH
# define OLENA_CONVERT_CONVERSION_HH

# include <functional>
# include "core/type.hh"
# include "value/debug.hh"

namespace oln {
  namespace convert {

    /* Conversions are unary functors which can be passed to
       processings to perform result conversions.

       Conversions are organized as an Inferior-parametred hierarchy,
       like the image hierarchy.  conversion is the top type here,
       therefore the processings which accept a conversion usually
       looks as follow.

       template< class _C, ...>
       ... fubar(const converstion<_C>& _conv, ...)
       {
          Exact_cref(C, conv);
	  ...
       }
    */

    template<class Inferior = type::bottom>
    struct conversion : public type::any< conversion<Inferior> >
    {
      typedef Inferior inferior;

      static std::string name() {
	return std::string("conversion<") + Inferior::name() + ">";
      }

    protected:
      conversion() {}
    };

    /*  Conversion's children should feature two things:

	1. a unary operator<T>() function, which perform the conversion.
	2. a output<T>::ret typedef, which is set to the type
	   returned by operator() for an input of type T.

       Note that a conversion is generic w.r.t. the input type of
       operator().  Different input types can lead to different
       output types, hence the inner output<T>::ret typedef.
    */


    /* However, some conversions do always return the same type.  In
       this case, output<T>::ret has the same definition for any T.
       Such conversions should inherit from the conversion_to_type<>
       class.  */
    template<class Result_Type, class Inferior = type::bottom>
    struct conversion_to_type :
      public conversion< conversion_to_type< Result_Type, Inferior > >
    {
      typedef Inferior inferior;

      template< class Input >
      struct output {
	/* We always convert to type Output, whatever is the Input
	   type.  */
	typedef Result_Type ret;
      };

      /* Additionally define result_type.  This is not required
         in a conversion class (generally not possible to define).
	 But it's useful when it's available (like here)
	 because it make the conversion appear almost as Adaptable
	 Unary Function (it will just lack the argument_type, but
	 this typedef is not used very much.)  */
      typedef Result_Type result_type;

      static std::string name() {
	return std::string("conversion_to_type<")
	  + typename_of<Result_Type>() + ", "
	  + typename_of<Inferior>() + ">";
      }
    };

    /* If both input and output types of the conversion are fixed.
       Inherit from conversion_from_type_to_type<>.  */
    template<class Argument_Type, class Result_Type,
	     class Inferior = type::bottom>
    struct conversion_from_type_to_type :
      public conversion_to_type< Result_Type,
        conversion_from_type_to_type< Argument_Type, Result_Type, Inferior> >
    {
      typedef Inferior inferior;

      /* By defining argument_type, and inheriting from result_type,
	 we comply to the STL concept of Adaptable Unary Function.  */
      typedef Argument_Type argument_type;

      static std::string name() {
	return std::string("conversion_from_type_to_type<")
	  + typename_of<Argument_Type>() + ", "
	  + typename_of<Result_Type>() + ", "
	  + typename_of<Inferior>() + ">";
      }
    };


    /* ConvOutput queries the output type of conversion ConvType for
       an input of type InputType.  This comes handy when computing
       the return type of a function which takes a conversion function
       in argument.  */
    template<class ConvType, class InputType>
    struct convoutput
    {
      typedef Exact(ConvType)::output<InputType>::ret ret;
    };


    /* Because a conversion is generic w.r.t. its input type, it can't
       _always_ be seen as an Adaptable Unary Function.  (Althought, It
       can always be seen as a Unary Function.)

       However, if C is a conversion function and A an Adaptable Unary
       Function, then we can define (C o A) as an Adaptable Unary
       Function.  Indeed, we know the i/o types of (C o A): it takes
       A::argument_type and returns C::output<A::result_type>::ret.

       Similary, if B is an Adaptable Binary Function, we can define
       (C o B) as an Adaptable Binary Function.

       The tools below create such compound functions.  */

    namespace internal {
      /* Compose a conversion C and an adaptable unary function UF,
	 producing an adaptable unary function.  */
      template <class C, class UF>
      struct _compconv1 :
	public std::unary_function <typename UF::argument_type,
	  typename C::output<typename UF::argument_type>::ret>
      {
	typedef _compconv1 self;

	typename self::result_type
	operator()(typename self::argument_type arg) const {
	  return _conv(_func(arg));
	}

	_compconv1(const C& conv, const UF& func)
	  : _conv(conv), _func(func)
	{}
      private:
	const C _conv;
	const UF _func;
      };

      /* Compose a conversion C and an adaptable binary function BF,
	 producing an adaptable binary function.  */
      template <class C, class BF>
      struct _compconv2 :
	public std::binary_function <typename BF::first_argument_type,
          typename BF::second_argument_type,
          typename C::output<typename BF::result_type>::ret>
      {
	typedef _compconv2 self;

	typename self::result_type
	operator()(typename self::first_argument_type arg1,
		   typename self::second_argument_type arg2) const {
	  return _conv(_func(arg1, arg2));
	}

	_compconv2(const C &conv, const BF &func)
	  : _conv(conv), _func(func)
	{}
      private:
	const C _conv;
	const BF _func;
      };

    } // end of internal

    /* Friendly procedure that build an internal::_compconv1 with
       type deduction.  */
    template <class _C, class UF>
    internal::_compconv1<Exact(_C), UF>
    compconv1(const conversion<_C>& _conv, const UF &func) {
      Exact_cref(C, conv);
      return internal::_compconv1<C, UF>(conv, func);
    }

    /* Likewise for _compconv2.  */
    template <class _C, class BF>
    internal::_compconv2<Exact(_C), BF>
    compconv2(const conversion<_C>& _conv, const BF &func) {
      Exact_cref(C, conv);
      return internal::_compconv2<C, BF>(conv, func);
    }


    /* The core oln::apply function, cannot apply all conversion function,
       because they do not all define result_type.  So we define another
       apply function here, to apply conversions.  */
    template<class C, class I> inline
    typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
    apply(const conversion<C>& conv, const image<I>& input)
    {
      /* CONV can now be wrapped as an Adaptable Unary Function
	 because we know the input type.  Composing CONV with the
	 identity for the input type will cause such wrapping to
	 happen.  */
      return apply(compconv1(conv, f_identity<Value(I)>()), input);
    }

  } // convert

  /* Export conversion and convouput into oln:: to simplify the
     writing of processings.  */
  using convert::conversion;
  using convert::convoutput;

} // oln


#endif // OLENA_CONVERT_CONVERSION_HH
