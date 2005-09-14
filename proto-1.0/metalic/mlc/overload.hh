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

#ifndef METALIC_OVERLOAD_HH
# define METALIC_OVERLOAD_HH

# include <mlc/types.hh>
# include <mlc/bool.hh>
# include <mlc/is_a.hh>
# include <mlc/if.hh>
# include <mlc/cmp.hh>
# include <mlc/typedef.hh>
# include <mlc/properties.hh>


namespace mlc
{

  mlc_equip_namespace_with_properties();


  /// where (for use as where clause).

  template <typename B>
  struct where : public B
  {
    static const bool check_that_B_is_a_Boolean = mlc_bool(B);
  };



  /// decl_overload (structure to be specialized by the client).

  template <typename what_is_overloaded,
 	    unsigned i,
	    typename T1,
	    typename T2 = no_type,
	    typename T3 = no_type>
  struct decl_overload : public undefined_type // FIXME: undefined_type is obsolete here
  {
    // no 'ret' inside
  };

  // we know that the structure above is specialized by a client when
  // it does not derive from mlc::undefined_type



  // Internal stuff.

  namespace internal
  {

    template <typename what, unsigned i,
	      typename T1,
	      typename T2 = no_type,
	      typename T3 = no_type>
    struct f_overload;

    // f_overload(i)  ---here, other params are omitted
    // {
    //   for i = 1...
    //     if (decl_overload(i) has not "typedef ret")
    //       ---default case
    //       return decl_overload(0)
    //     else if (decl_overload(i) derives from true_type)
    //       ---the proper overload version is found
    //       return decl_overload(i)
    //     else
    //       ---a proper overload version is not found yet
    //       ---so searching continues with i+1
    //       return f_overload(i+1)
    // }

    template <typename base,
	      typename what, unsigned i,
	      typename T1, typename T2, typename T3>
    struct helper_f_overload;

    // default case

    template <typename what, unsigned i,
	      typename T1, typename T2, typename T3>
    struct helper_f_overload <undefined_type,
			      what, i, T1,T2,T3>
    {
      typedef decl_overload<what, 0, T1,T2,T3> ret;
    };

    // case 'found'

    template <typename what, unsigned i,
	      typename T1, typename T2, typename T3>
    struct helper_f_overload <true_type,
			      what, i, T1,T2,T3>
    {
      typedef decl_overload<what, i, T1,T2,T3> ret;
    };

    // case 'search again'

    template <typename what, unsigned i,
	      typename T1, typename T2, typename T3>
    struct helper_f_overload <false_type,
			      what, i, T1,T2,T3>
    {
      typedef f_overload<what, i+1, T1,T2,T3> next_overload;
      typedef typename next_overload::ret ret;
    };

    // f_overload

    template <typename what, unsigned i,
	      typename T1, typename T2, typename T3>
    struct f_overload
    {
      typedef decl_overload<what, i, T1,T2,T3> decl;

//       first version of meta-code:
//       ---------------------------
//       typedef typename
//       if_< mlc_is_a(decl, undefined_type),
// 	   undefined_type, typename
// 	   if_< mlc_is_a(decl, true_type),
// 		true_type,
// 		false_type >::ret
//       >::ret base;

//       new version relying on mlc_typedef_of:
//       --------------------------------------
      typedef mlc_typedef_of(decl, ret) decl_ret;

//       typedef eq<decl_ret, mlc::internal::not_found> eq_res;
//       typedef false_type base;
      typedef typename
      if_< eq<decl_ret, mlc::internal::not_found>,
	   undefined_type, typename
	   if_< mlc_is_a(decl, true_type),
		true_type,
		false_type >::ret
      >::ret base;

      typedef helper_f_overload<base, what, i, T1,T2,T3> helper;
      typedef typename helper::ret ret;
    };

  } // end of namespace mlc::internal


  /// overload (ternary version).

  template <typename what,
	    typename T1,
	    typename T2 = no_type,
	    typename T3 = no_type>
  struct overload
  {
    static const unsigned start = 1;
    typedef typename internal::f_overload<what, start, T1,T2,T3>::ret overload_type;

    // ret
    typedef mlc_typedef_of(overload_type, ret) ret;

    // exec
    static ret exec(const T1& arg1, const T2& arg2, const T3& arg3)
    {
      return overload_type::exec(arg1, arg2, arg3);
    }
  };


  /// overload (binary version).

  template <typename what,
	    typename T1, typename T2>
  struct overload <what,
		   T1, T2,
		   no_type>
  {
    typedef no_type T3;

    static const unsigned start = 1;
    typedef typename internal::f_overload<what, start, T1,T2,T3>::ret overload_type;

    // ret
    typedef mlc_typedef_of(overload_type, ret) ret;

    // exec
    static ret exec(const T1& arg1, const T2& arg2)
    {
      return overload_type::exec(arg1, arg2);
    }
  };


  /// overload (unary version).

  template <typename what,
	    typename T1>
  struct overload <what,
		   T1,
		   no_type, no_type>
  {
    typedef no_type T2;
    typedef no_type T3;

    static const unsigned start = 1;
    typedef typename internal::f_overload<what, start, T1,T2,T3>::ret overload_type;

    // ret
    typedef mlc_typedef_of(overload_type, ret) ret;

    // exec
    static ret exec(const T1& arg1)
    {
      return overload_type::exec(arg1);
    }
  };


} // end of namespace mlc



// sample use

/*

namespace tag
{
  struct foo;
}

template <class T>
struct decl_overload <tag::foo, 1, T> : public where< eq<T, int> >
{
  typedef int ret;
  static ret exec(const int& lhs)
  {
    std::cout << "foo(int)" << std::endl;
    return 1;
  }
};

template <typename I>
struct decl_overload <tag::foo, 2, I> : public where< mlc_is_a(I, oln::abstract::image) >
{
  typedef int ret;
  static ret exec(const I& lhs)
    {
      std::cout << "foo(image)" << std::endl;
      lhs.m();
      return 2;
    }
};

template <typename T>
struct decl_overload <tag::foo, 0, T>
{
  typedef int ret;
  static ret exec(const T& lhs)
  {
    std::cout << "foo(T)" << std::endl;
    return 0;
  }
};

template <typename T>
typename overload<tag::foo, T>::ret
foo(const T& arg)
{
  return overload<tag::foo, T>::exec(arg);
}

int main()
{
  int i;
  float f;
  oln::image2d<int> ima;

  test::foo(i);
  test::foo(f);
  test::foo(ima);
}

*/


#endif // ! METALIC_OVERLOAD_HH

