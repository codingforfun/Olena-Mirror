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

#ifndef OLENA_CORE_FUNOBJ_DECL_ATTR_HH
# define OLENA_CORE_FUNOBJ_DECL_ATTR_HH

# include <mlc/is_a.hh>
# include <oln/funobj/abstract/accessor.hh>
# include <oln/funobj/abstract/unary.hh>
# include <oln/core/gen/attr.hh>


/// Macro oln_decl_attr. 

# define oln_decl_attr(ATTR)						\
									\
namespace oln								\
{									\
									\
  namespace a_ {							\
									\
    struct ATTR : public f_::abstract::accessor< ATTR >			\
    {									\
      template <typename T>						\
      struct type							\
      {									\
	typedef typename T::ATTR##_type ret;				\
      };								\
      template <typename T>						\
      typename T::ATTR##_type& operator()(T& t) const			\
      {									\
	return t.ATTR;							\
      }									\
      template <typename T>						\
      const typename T::ATTR##_type& operator()(const T& t) const	\
      {									\
	return t.ATTR;							\
      }									\
    };									\
									\
  }									\
									\
  template <typename I>							\
  attr<a_::ATTR, I> a_##ATTR(abstract::image<I>& input)			\
  {									\
    mlc_is_a(I, abstract::raw_image)::ensure();				\
    attr<a_::ATTR, I> tmp(input);					\
    return tmp;								\
  }									\
									\
  template <typename I>							\
  attr<a_::ATTR, const I> a_##ATTR(const abstract::image<I>& input)	\
  {									\
    mlc_is_a(I, abstract::raw_image)::ensure();				\
    attr<a_::ATTR, const I> tmp(input);					\
    return tmp;								\
  }									\
									\
}									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




// Dummy code for testing purpose.

namespace oln
{

  namespace a_ {

    struct dummy : public f_::abstract::accessor< dummy >
    {
      template <typename T>
      struct type
      {
	typedef typename T::dummy_type ret;
      };
      template <typename T>
      typename T::dummy_type& operator()(T& t) const
      {
	return t.dummy;
      }
      template <typename T>
      const typename T::dummy_type& operator()(const T& t) const
      {
	return t.dummy;
      }
    };

  } // end of namespace oln::a_


  template <typename I>
  attr<a_::dummy, I> a_dummy(abstract::image<I>& input)
  {
    mlc_is_a(I, abstract::raw_image)::ensure();
    attr<a_::dummy, I> tmp(input);
    return tmp;
  }

  template <typename I>
  attr<a_::dummy, const I> a_dummy(const abstract::image<I>& input)
  {
    mlc_is_a(I, abstract::raw_image)::ensure();
    attr<a_::dummy, const I> tmp(input);
    return tmp;
  }


  // f_::a_dummy_<T> as a pure function

  namespace f_ {
    template <typename T> struct a_dummy_;
  }

  template <typename T>
  struct set_super_type < f_::a_dummy_<T> > { typedef f_::abstract::unary< f_::a_dummy_<T> > ret; };

  template <typename T>
  struct set_props < category::fun1, f_::a_dummy_<T> >
  {
    typedef typename T::dummy_type res_type;
    typedef T arg_type;
  };

  namespace f_ {

    template <typename T>
    struct a_dummy_ : public oln_super_of_(f_::a_dummy_<T>)
    {
      const typename T::dummy_type impl_unop(const T& t) const
      {
	return t.dummy;
      }
    };

  } // end of namespace oln::f_

  // f_dummy as a pure meta function

  typedef f_::unary_meta<f_::a_dummy_> fa_dummy_type;
  static fa_dummy_type fa_dummy;


} // end of namespace oln



#endif // ! OLENA_CORE_FUNOBJ_DECL_ATTR_HH
