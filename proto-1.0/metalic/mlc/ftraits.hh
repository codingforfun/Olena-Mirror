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

#ifndef METALIC_FTRAITS_HH
# define METALIC_FTRAITS_HH

# include <mlc/types.hh>
# include <mlc/bool.hh>
# include <mlc/cmp.hh>
# include <mlc/typedef.hh>
# include <mlc/overload.hh>


// FIXME: libraries should be id-ed in a different way...

namespace mlc
{
  enum {
    // FIXME: add builtin_id = 1 and update code below...
    ntg_id = 2,
    oln_id = 3
  };
} // end of namespace mlc





// Macro mlc_decl_unary_traits.

# define mlc_decl_unary_traits(Name)				\
								\
  namespace tag { struct Name##_; }				\
								\
  template <typename T>						\
  struct unary_traits <tag::Name##_, T>				\
  {								\
    typedef typename overload<tag::Name##_, T>::ret ret;	\
  };								\
								\
  template <typename T>						\
  struct traits_##Name : public unary_traits<tag::Name##_, T>	\
  {								\
  };								\
								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



# define mlc_decl_unary_arith_builtin_traits(Name)	\
							\
  mlc_decl_unary_traits(Name);				\
							\
  template <typename T>					\
  struct decl_overload <tag::Name##_, 1, T>		\
    : public where< is_builtin<T> >			\
  {							\
    typedef T ret;					\
  };							\
							\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



// Macro mlc_decl_binary_traits.

# define mlc_decl_binary_traits(Name)					\
									\
  namespace tag { struct Name##_; }					\
									\
  template <typename T1, typename T2>					\
  struct binary_traits <tag::Name##_, T1, T2>				\
  {									\
    typedef typename overload<tag::Name##_, T1, T2>::ret ret;		\
  };									\
									\
  template <typename T1, typename T2>					\
  struct traits_##Name : public binary_traits<tag::Name##_, T1, T2>	\
  {									\
  };									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


// FIXME: what about binary traits with a single type; e.g., for min<T,T>?
// FIXME: is the code below ok?

// Macro mlc_decl_mono_binary_traits.

# define mlc_decl_mono_binary_traits(Name)				\
									\
  namespace tag { struct Name##_; }					\
									\
  template <typename T1, typename T2>					\
  struct binary_traits <tag::Name##_, T1, T2>				\
  {									\
    typedef typename overload<tag::Name##_, T1, T2>::ret ret;		\
  };									\
									\
  template <typename T1, typename T2>					\
  struct traits_##Name : public binary_traits<tag::Name##_, T1, T2>	\
  {									\
  };									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




// Macro mlc_decl_binary_arith_builtin_traits.

# define mlc_decl_binary_arith_builtin_traits(Name)		\
								\
  mlc_decl_binary_traits(Name);					\
								\
  template <typename T1, typename T2>				\
  struct decl_overload <tag::Name##_, 1, T1, T2>		\
    : public where< and_< is_builtin<T1>, is_builtin<T2> > >	\
  {								\
    typedef overload<internal::tag_UAC, T1, T2> type;		\
    typedef and_< is_builtin<T1>, is_builtin<T2> > cond;	\
    typedef mlc_typedef_onlyif_of(type, ret, cond) ret;		\
  };								\
								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



// Macro mlc_decl_mono_binary_builtin_traits.
// FIXME: simplistic and wrong code... change!

# define mlc_decl_mono_binary_builtin_traits(Name)		\
								\
  mlc_decl_mono_binary_traits(Name);				\
								\
  template <typename T1, typename T2>				\
  struct decl_overload <tag::Name##_, 1, T1, T2>		\
    : public where< and_< is_builtin<T1>, is_builtin<T2> > >	\
  {								\
    typedef T1 ret;						\
  };								\
								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n






// Macro mlc_decl_unary_logic_builtin_traits.

# define mlc_decl_unary_logic_builtin_traits(Name)	\
							\
  mlc_decl_unary_traits(Name);				\
							\
  template <typename T>					\
  struct decl_overload <tag::Name##_, 1, T>		\
    : public where< is_builtin<T> >			\
  {							\
    typedef bool ret;					\
  };							\
							\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


// Macro mlc_decl_binary_logic_builtin_traits.

// FIXME: rule 2 should return either T1 or T2
// FIXME: rule 2 should ensure that both types are Boolean

# define mlc_decl_binary_logic_builtin_traits(Name)					\
											\
  mlc_decl_binary_traits(Name);								\
											\
  template <typename T1, typename T2>							\
  struct decl_overload <tag::Name##_, 1, T1, T2>					\
    : public where< and_< is_builtin<T1>, is_builtin<T2> > >				\
  {											\
    typedef bool ret;									\
  };											\
											\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




// Macro mlc_decl_cmp_builtin_traits.

# define mlc_decl_cmp_builtin_traits mlc_decl_binary_logic_builtin_traits





# define mlc_intprom(TYPE) typename mlc::internal::integral_promote<TYPE>::ret




# define mlc_decl_is(QUALIFIER, TYPE)				\
								\
  template <>							\
  struct is_##QUALIFIER <TYPE> : public value<bool, true>	\
  {								\
  protected:							\
    is_##QUALIFIER() {}						\
  };								\
								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace mlc
{

  template <typename Tag, typename T>
  struct unary_traits
  {
  };

  template <typename Tag, typename T1, typename T2>
  struct binary_traits
  {
  };


  // identification of built-in types

  template <typename T>
  struct is_builtin : public value<bool, false>
  {
  protected:
    is_builtin() {}
  };

  mlc_decl_is( builtin,          bool   );

  mlc_decl_is( builtin,          float  );
  mlc_decl_is( builtin,          double );
  mlc_decl_is( builtin,     long double );

  mlc_decl_is( builtin,          char   );
  mlc_decl_is( builtin, unsigned char   );
  mlc_decl_is( builtin,   signed char   );

  mlc_decl_is( builtin, unsigned short );
  mlc_decl_is( builtin,   signed short );
  mlc_decl_is( builtin, unsigned int   );
  mlc_decl_is( builtin,   signed int   );
  mlc_decl_is( builtin, unsigned long  );
  mlc_decl_is( builtin,   signed long  );


  // identification of arrays

  template <typename T>
  struct is_array : public value<bool, false>
  {
  protected:
    is_array() {}
  };

  template <typename T, unsigned n>
  struct is_array <T[n]> : public value<bool, true>
  {
  protected:
    is_array() {}
  };


  // identification of Boolean value types

  template <typename T>
  struct is_Boolean : public value<bool, false>
  {
  protected:
    is_Boolean() {}
  };

  mlc_decl_is( Boolean, bool );



  // traits

  template <typename Tag, typename T1, typename T2 = no_type>
  struct traits
  {
    typedef typename overload<Tag, T1, T2>::ret ret;
  };


  // built-in traits and usual arithmetic conversions

  namespace internal
  {

    // FIXME:... Cf. page 90 du pdf de la norme...


    struct tag_UAC; // for "usual arithmetic conversions"

    // Integral promotion such as defined in FIXME: std.

    template <class T>
    struct integral_promote
    {
      typedef T ret;
    };

    // FIXME: approx?

    template <> struct integral_promote <char> { typedef int ret; };
    template <> struct integral_promote <signed char> { typedef int ret; };
    template <> struct integral_promote <unsigned char> { typedef int ret; };
    template <> struct integral_promote <unsigned short int> { typedef int ret; };

    template <> struct integral_promote <bool> { typedef int ret; }; // FIXME: right?

  } // end of namespace mlc::internal


  // 1) Prevent bool to be involved in arithmetics.
  template <typename T1, typename T2>
  struct decl_overload <internal::tag_UAC, 1, T1, T2>
    : where< or_<eq<T1, bool>, eq<T2, bool> > >
  {
    typedef undefined_type ret;
  };

  // 2) If either operand is of type long double, the other shall be
  // 2) converted to long double.
  template <typename T1, typename T2>
  struct decl_overload <internal::tag_UAC, 2, T1, T2>
    : where< or_<eq<T1, long double>, eq<T2, long double> > >
  {
    typedef long double ret;
  };

  // 3) Otherwise, if either operand is double, the other shall be
  // 3) converted to double.
  template <typename T1, typename T2>
  struct decl_overload <internal::tag_UAC, 3, T1, T2>
    : where< or_<eq<T1, double>, eq<T2, double> > >
  {
    typedef double ret;
  };

  // 4) Otherwise, if either operand is float, the other shall be
  // 4) converted to float.
  template <typename T1, typename T2>
  struct decl_overload <internal::tag_UAC, 4, T1, T2>
    : where< or_<eq<T1, float>, eq<T2, float> > >
  {
    typedef float ret;
  };
  
  // 5) If either operand is unsigned long the other shall be
  // 5) converted to unsigned long.
  template <typename T1, typename T2>
  struct decl_overload <internal::tag_UAC, 5, T1, T2>
    : where< or_<eq<T1, unsigned long>, eq<T2, unsigned long> > >
  {
    typedef unsigned long ret;
  };
  
  // 6) Otherwise, if one operand is a long int and the other unsigned
  // 6) int, then if a long int can represent all the values of an
  // 6) unsigned int, the unsigned int shall be converted to a long
  // 6) int; otherwise both operands shall be converted to unsigned
  // 6) long int.
  template <typename T1, typename T2>
  struct decl_overload <internal::tag_UAC, 6, T1, T2>
    : where< or_< and_< eq<mlc_intprom(T1), long int>,
			eq<mlc_intprom(T2), unsigned int> >,
		  and_< eq<mlc_intprom(T2), long int>,
			eq<mlc_intprom(T1), unsigned int> > > >
  {
    typedef long int ret; // FIXME: approx...
  };

  // 7) Otherwise, if either operand is long, the other shall be
  // 7) converted to long.
  template <typename T1, typename T2>
  struct decl_overload <internal::tag_UAC, 7, T1, T2>
    : where< or_<eq<mlc_intprom(T1), long>, eq<mlc_intprom(T2), long> > >
  {
    typedef long ret;
  };

  // 8) Otherwise, if either operand is unsigned, the other shall be
  // 8) converted to unsigned.
  template <typename T1, typename T2>
  struct decl_overload <internal::tag_UAC, 8, T1, T2>
    : where< or_<eq<mlc_intprom(T1), unsigned>, eq<mlc_intprom(T2), unsigned> > >
  {
    typedef unsigned ret;
  };

  // 0) Note: otherwise, the only remaining case is that both operands
  // 0) are int.
  template <typename T1, typename T2>
  struct decl_overload <internal::tag_UAC, 0, T1, T2>
  {
    // FIXME: test that:
    // or_<eq<mlc_intprom(T1), int>, eq<mlc_intprom(T2), int> >
    typedef int ret;
  };


  // Arith.

  mlc_decl_unary_traits( uminus );

  template <typename T>
  struct decl_overload <tag::uminus_, 1, T>
    : public where< is_builtin<T> >
  {
    typedef mlc_intprom(T) ret; // FIXME: correct?
    // FIXME: is it relevant to check that the result is traits_minus<int,T>::ret?
  };

  mlc_decl_unary_arith_builtin_traits( inc );
  mlc_decl_unary_arith_builtin_traits( dec );

  mlc_decl_binary_arith_builtin_traits( plus  );
  mlc_decl_binary_arith_builtin_traits( minus );
  mlc_decl_binary_arith_builtin_traits( times );
  mlc_decl_binary_arith_builtin_traits( div   );
  mlc_decl_binary_arith_builtin_traits( mod   );
  

  // Logic.

  mlc_decl_unary_logic_builtin_traits(   not );
  
  mlc_decl_binary_logic_builtin_traits(  and );
  mlc_decl_binary_logic_builtin_traits( nand );
  mlc_decl_binary_logic_builtin_traits(   or );
  mlc_decl_binary_logic_builtin_traits(  nor );
  mlc_decl_binary_logic_builtin_traits(  xor );
  mlc_decl_binary_logic_builtin_traits( xnor );


  // Cmp.

  mlc_decl_cmp_builtin_traits(  eq );
  mlc_decl_cmp_builtin_traits( neq );
  mlc_decl_cmp_builtin_traits( less );
  mlc_decl_cmp_builtin_traits( leq );
  mlc_decl_cmp_builtin_traits( greater );
  mlc_decl_cmp_builtin_traits( geq );


  // Mono binary.

  mlc_decl_mono_binary_builtin_traits(min);
  mlc_decl_mono_binary_builtin_traits(max);
  mlc_decl_mono_binary_builtin_traits(inf);
  mlc_decl_mono_binary_builtin_traits(sup);


} // end of namespace mlc


# define mlc_binary_ret_(OperatorName, T1, T2) mlc::traits_##OperatorName <T1,T2>::ret
# define mlc_binary_ret(OperatorName, T1, T2) typename mlc_binary_ret_(OperatorName, T1, T2)

# define mlc_unary_ret_(OperatorName, T) mlc::traits_##OperatorName <T>::ret
# define mlc_unary_ret(OperatorName, T) typename mlc_unary_ret_(OperatorName, T)



#endif // ! METALIC_FTRAITS_HH
