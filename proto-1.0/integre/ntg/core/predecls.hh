// Copyright (C) 2001, 2002, 2003, 2005  EPITA Research and Development Laboratory
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

#ifndef NTG_CORE_PREDECLS_HH
# define NTG_CORE_PREDECLS_HH

# include <mlc/bool.hh>
# include <mlc/is_a.hh>
# include <mlc/typedef.hh>
# include <mlc/ftraits.hh>

# include <ntg/core/type.hh>
# include <ntg/vect/cplx_representation.hh>




# define ntg_can_handle_types(T1, T2)						\
										\
  mlc::ors< mlc::and_< mlc_is_a(T1, ntg::value), mlc_is_a(T2, ntg::value) >,	\
	    mlc::and_< mlc_is_a(T1, ntg::value), mlc::is_builtin<T2>      >,	\
	    mlc::and_< mlc::is_builtin<T1>,      mlc_is_a(T2, ntg::value) >	\
          >



/*
  Forward declarations of every available types.
*/

namespace ntg
{

  template <class E>
  class value;

  /*----------.
  | behaviors |
  `----------*/

  class unsafe;
  class strict;
  class saturate;

  /*----------.
  | intervals |
  `----------*/

  template <class T, T i_min, T i_max> class		bounded;
  template <unsigned i_min, unsigned i_max> class	bounded_u;
  template <signed i_min, signed i_max> class		bounded_s;

  /*------.
  | reals |
  `------*/

  template <unsigned nbits, class behavior = strict>		class int_u;
  template <unsigned nbits, class behavior = strict>		class int_s;
  template <class T, class interval, class behavior = strict>	class range;
  template <class T, class interval>				class cycle;

  typedef float		float_s;
  typedef double	float_d;

  /*-----------.
  | enumerated |
  `-----------*/

  class bin;

  /*----------.
  | vectorial |
  `----------*/

  template <unsigned N, class T, class Self = ntg::final> class vec;

  template <cplx_representation R, class T> class cplx;

  /*-------.
  | colors |
  `-------*/

  // This helps swig to parse to file.
#ifndef SWIG
    template <unsigned ncomps,
	      unsigned qbits,
	      template <unsigned> class color_system>
    struct color;
#else
    struct color;
#endif

  template<int lval, int uval>
  struct interval;

  /*--------------------.
  | shortcuts for int_u |
  `--------------------*/

  typedef int_u<1, strict>	int_u1;
  template <> class int_u<1, unsafe>   {}; // type disabled
  template <> class int_u<1, saturate> {}; // type disabled

  typedef int_u<8, strict>	int_u8;
  typedef int_u<8, unsafe>	int_u8u;
  typedef int_u<8, saturate>	int_u8s;

  typedef int_u<16, strict>	int_u16;
  typedef int_u<16, unsafe>	int_u16u;
  typedef int_u<16, saturate>	int_u16s;

  typedef int_u<32, strict>	int_u32;
  typedef int_u<32, unsafe>	int_u32u;
  typedef int_u<32, saturate>	int_u32s;

  /*--------------------.
  | shortcuts for int_s |
  `--------------------*/

  typedef int_s<8, strict>	int_s8;
  typedef int_s<8, unsafe>	int_s8u;
  typedef int_s<8, saturate>	int_s8s;

  typedef int_s<16, strict>	int_s16;
  typedef int_s<16, unsafe>	int_s16u;
  typedef int_s<16, saturate>	int_s16s;

  typedef int_s<32, strict>	int_s32;
  typedef int_s<32, unsafe>	int_s32u;
  typedef int_s<32, saturate>	int_s32s;

  /*----------.
  | operators |
  `----------*/

  template <class, class> struct operator_plus_traits;
  template <class, class> struct operator_minus_traits;
  template <class, class> struct operator_times_traits;
  template <class, class> struct operator_div_traits;
  template <class, class> struct operator_mod_traits;
  template <class, class> struct operator_logical_traits;
  template <class, class> struct operator_cmp_traits;
  template <class, class> struct operator_min_traits;
  template <class, class> struct operator_max_traits;

  template <class T> struct signed_type_traits;

} // end of ntg.


/*
  Set information required by the mlc library.
*/


// Macros for mlc::decl_overloading common ops return type.

# define ntg_decl_binary_arith_traits(Name)				\
									\
  template <typename T1, typename T2>					\
  struct decl_overload <tag::Name##_, mlc::ntg_id, T1, T2>		\
    : public where< ntg_can_handle_types(T1, T2) >			\
  {									\
    typedef ntg::operator_##Name##_traits<T1, T2> type;			\
    typedef ntg_can_handle_types(T1, T2) cond;				\
    typedef mlc_typedef_onlyif_of(type, ret, cond) ret;			\
  };									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



# define ntg_decl_boolret_traits(Name)				\
								\
  template <typename T1, typename T2>				\
  struct decl_overload <tag::Name##_, mlc::ntg_id, T1, T2>	\
    : public where< ntg_can_handle_types(T1, T2) >		\
  {								\
    typedef bool ret;						\
  };								\
								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




namespace mlc
{
  mlc_decl_is( Boolean, ntg::bin );
  mlc_decl_is( Boolean, ntg::int_u1 );

  // Arith.

  ntg_decl_binary_arith_traits( plus  );
  ntg_decl_binary_arith_traits( minus );
  ntg_decl_binary_arith_traits( times );
  ntg_decl_binary_arith_traits( div   );
  ntg_decl_binary_arith_traits( mod   );

  template <typename T>
  struct decl_overload <tag::uminus_, mlc::ntg_id, T>
    : public where< mlc_is_a(T, ntg::value) >
  {
    typedef typename ntg::signed_type_traits<T> type;
    typedef mlc_is_a(T, ntg::value) cond;
    typedef mlc_typedef_onlyif_of(type, ret, cond) ret;
  };

  template <typename T>
  struct decl_overload <tag::inc_, mlc::ntg_id, T>
    : public where< mlc_is_a(T, ntg::value) >
  {
    typedef T ret;
  };

  template <typename T>
  struct decl_overload <tag::dec_, mlc::ntg_id, T>
    : public where< mlc_is_a(T, ntg::value) >
  {
    typedef T ret;
  };

  // Cmp.

  ntg_decl_boolret_traits(  eq );
  ntg_decl_boolret_traits( neq );
  ntg_decl_boolret_traits( less );
  ntg_decl_boolret_traits( leq );
  ntg_decl_boolret_traits( greater );
  ntg_decl_boolret_traits( geq );

  // Logic.

  ntg_decl_boolret_traits(  and );
  ntg_decl_boolret_traits( nand );
  ntg_decl_boolret_traits(   or );
  ntg_decl_boolret_traits(  nor );
  ntg_decl_boolret_traits(  xor );
  ntg_decl_boolret_traits( xnor );


  template <typename T>
  struct decl_overload <tag::not_, mlc::ntg_id, T>
    : public where< mlc_is_a(T, ntg::value) >
  {
    typedef bool ret;
  };


} // end of mlc.



#endif // !NTG_CORE_PREDECLS_HH
