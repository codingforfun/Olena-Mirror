// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLN_UTILS_OVERLOAD_HH
# define OLN_UTILS_OVERLOAD_HH

# include <mlc/types.hh>
# include <mlc/is_a.hh>
# include <mlc/if.hh>




// FIXME: most code here should move to metalic!

// FIXME: some piece of code should move to oln/core?




//   // Sample use:

//   struct for_doit;

//   template <typename I>
//   struct set_overload_1 < for_doit, where<I, abstract::image> >
//   {
//     typedef int ret;
//     static ret exec(const abstract::image<I>& ima)
//     {
//       std::cout << "image" << std::endl;
//       return 51;
//     }
//   };

//   template <typename T>
//   struct set_overload_1 < for_doit, where<T, whatever> >
//   {
//     typedef T ret;
//     static T exec(const T& t)
//     {
//       std::cout << "NOT image" << std::endl;
//       return t;
//     }
//   };

//   template <typename T>
//   typename overload_1<for_doit, T>::ret
//   doit(const T& arg)
//   {
//     std::cout << "doit..." << std::endl;
//     return overload_1<for_doit, T>::exec(arg);
//   }




// fwd decls

namespace ntg
{
  template <typename E> struct value;

} // end of namespace ntg

namespace oln
{

  namespace internal
  {
    template <typename I> struct value_box;

  } // end of namespace oln::internal

  namespace abstract
  {
    template <typename I> struct image;

  } // end of namespace oln::abstract

  namespace pw
  {
    namespace abstract
    {
      template <typename F> struct function;

    } // end of namespace oln::pw::abstract

  } // end of namespace oln::pw

} // end of namespace oln


// end of fwd decls





namespace oln
{

  /// whatever

  template <typename> struct whatever;

  /// when

  template <typename T,
	    template <class> class abstraction = whatever >
  struct where;





  /// Structures to be specialized by the client.

  template <typename what_is_overloaded, typename where_clause>
  struct set_overload_1
  {
    typedef mlc::undefined_type ret;
  };

  template <typename what_is_overloaded,
	    typename where_clause_1, typename where_clause_2>
  struct set_overload_2
  {
    typedef mlc::undefined_type ret;
  };


  /// Make 'whatever' be the 'default' overload.

  template <typename W, typename T1, typename T2>
  struct set_overload_2 < W, where<T1,whatever>, where<T2,whatever> >
  {
    typedef mlc::undefined_type ret;
  };

  template <typename W,
	    typename T1, template <class> class A1,
	    typename T2, template <class> class A2>
  struct set_overload_2     < W, where<T1,A1>,       where<T2,A2>       >
    : public set_overload_2 < W, where<T1,whatever>, where<T2,whatever> >
  {
    typedef set_overload_2<W, where<T1,whatever>, where<T2,whatever> > super_type;
    using super_type::ret;
    using super_type::exec;
  };



  namespace internal {


    /// Function f_where: T -> type.

    template <typename T>
    struct f_where
    {
      typedef typename
      mlc::if_< mlc_is_a(T, ntg::value),
		where<T, ntg::value>, typename 
		mlc::if_< mlc_is_a(T, oln::abstract::image),
			  where<T, oln::abstract::image>, typename 
			  mlc::if_< mlc_is_a(T, oln::internal::value_box),
				    where<T, oln::internal::value_box>, typename 
				    mlc::if_< mlc_is_a(T, oln::pw::abstract::function),
					      where<T, oln::pw::abstract::function>,
					      where<T>
                                             >::ret
                                   >::ret
                         >::ret
              >::ret
      ret;
    };



  } // end of namespace oln::internal




  /// Facades.

  template <typename what_is_overloaded, typename T>
  struct overload_1
  {
    typedef typename internal::f_where<T>::ret where_type;
    typedef set_overload_1<what_is_overloaded, where_type> overload_type;
    typedef typename overload_type::ret ret;

    static ret exec(const T& arg)
    {
      return overload_type::exec(arg);
    }
  };

  template <typename what_is_overloaded, typename T1, typename T2>
  struct overload_2
  {
    typedef typename internal::f_where<T1>::ret where1_type;
    typedef typename internal::f_where<T2>::ret where2_type;
    typedef set_overload_2<what_is_overloaded, where1_type, where2_type> overload_type;
    typedef typename overload_type::ret ret;

    static ret exec(const T1& arg1, const T2& arg2)
    {
      return overload_type::exec(arg1, arg2);
    }
    // FIXME: insert a static check that exec is defined in set_overload
  };

} // end of namespace oln




# include <ntg/core/internal/global_ops.hh>



# define oln_decl_binary_op(OperatorName, OperatorSymbol)		\
									\
  template <typename T1, typename T2>					\
  struct set_overload_2 < tag::op_##OperatorName,			\
			  where<T1, whatever>,				\
			  where<T2, whatever> >				\
  {									\
    typedef bool ret;							\
    static ret exec(const T1& lhs, const T2& rhs)			\
    {									\
      return oln::internal::operator_##OperatorName(lhs, rhs);		\
    }									\
  };									\
									\
  template <typename T1, typename T2>					\
  struct set_overload_2 < tag::op_##OperatorName,			\
			  where<T1, ntg::value>,			\
			  where<T2, whatever> >				\
  {									\
    typedef bool ret;							\
    static ret exec(const T1& lhs, const T2& rhs)			\
    {									\
      return ntg::internal::operator OperatorSymbol (lhs, rhs);		\
    }									\
  };									\
									\
  template <typename T1, typename T2>					\
  struct set_overload_2 < tag::op_##OperatorName,			\
			  where<T1, whatever>,				\
			  where<T2, ntg::value> >			\
  {									\
    typedef bool ret;							\
    static ret exec(const T1& lhs, const T2& rhs)			\
    {									\
      return ntg::internal::operator OperatorSymbol (lhs, rhs);		\
    }									\
  };									\
									\
  template <typename T1, typename T2>					\
  struct set_overload_2 < tag::op_##OperatorName,			\
			  where<T1, ntg::value>,			\
			  where<T2, ntg::value> >			\
  {									\
    typedef bool ret;							\
    static ret exec(const T1& lhs, const T2& rhs)			\
    {									\
      return ntg::internal::operator OperatorSymbol (lhs, rhs);		\
    }									\
  };									\
									\
  template <typename T1, typename T2>					\
  typename overload_2<tag::op_##OperatorName, T1, T2>::ret		\
  operator OperatorSymbol (const T1& lhs, const T2& rhs)		\
  {									\
    return overload_2<tag::op_##OperatorName, T1, T2>::exec(lhs, rhs);	\
  }									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




namespace oln
{

  /// Tags for operators.

  namespace tag
  {

    // Cmp tags.

    struct op_eq;
    struct op_neq;
    struct op_less;
    struct op_leq;
    struct op_greater;
    struct op_geq;

    // Logic tags.

    struct op_not;
    struct op_and;
    struct op_nand;
    struct op_or;
    struct op_nor;
    struct op_xor;
    struct op_xnor;

    // Arith tags.

    struct op_plus;
    struct op_minus;
    struct op_uminus;
    struct op_times;
    struct op_div;
    struct op_mod;
  
  } // end of namespace tag



  // Cmp operators.


  namespace internal
  {

    // Empty fwd decl for canonical cmp operators:
    // operator_eq and operator_less.
    // Both operands should exactly have the same type (T).

    template <typename T>
    bool operator_eq(const T&, const T&);

    template <typename T>
    bool operator_less(const T&, const T&);


    // Default impl for oln::internal::operator_*'s.

    // Warning: an important assumption of default impls is that
    // the return type is bool.

    // FIXME: should they call operator_* or the facade?
    // e.g., should we replace "not operator_eq(lhs, rhs)" by
    // "not (lhs == rhs)"?

    template <typename T1, typename T2>
    bool operator_neq(const T1& lhs, const T2& rhs)
    {
      // "lhs != rhs"  is  "not (lhs == rhs)"
      return not oln::internal::operator_eq(lhs, rhs);
    }

    template <typename T1, typename T2>
    bool operator_leq(const T1& lhs, const T2& rhs)
    {
      // "lhs <= rhs"  is  "not (rhs < lhs)"
      return not oln::internal::operator_less(rhs, lhs);
    }

    template <typename T1, typename T2>
    bool operator_greater(const T1& lhs, const T2& rhs)
    {
      // "lhs > rhs"  is  "rhs < lhs"
      return oln::internal::operator_less(rhs, lhs);
    }

    template <typename T1, typename T2>
    bool operator_geq(const T1& lhs, const T2& rhs)
    {
      // "lhs >= rhs"  is  "not (lhs < rhs)"
      return not oln::internal::operator_less(lhs, rhs);
    }

  } // end of namespace oln::internal



  // Section dedicated to ntg types
  // ------------------------------
  // When such a type is involved in an "oln::operator something(lhs, rhs)"
  // call, the default behavior is to turn to ntg to solve this call.
  // One could think that since we presently *are* in oln, FIXME
  // 


  oln_decl_binary_op(      eq, == );
  oln_decl_binary_op(     neq, != );
  oln_decl_binary_op(    less, <  );
  oln_decl_binary_op(     leq, <= );
  oln_decl_binary_op( greater, >  );
  oln_decl_binary_op(     geq, >= );



} // end of namespace oln


#endif // ! OLN_UTILS_OVERLOAD_HH
