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
  template <typename I> struct value_box;

  namespace abstract
  {
    template <typename I> struct image;

  } // end of namespace oln::abstract

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

  template <typename what_is_overloaded, typename where_clause_1, typename where_clause_2>
  struct set_overload_2
  {
    typedef mlc::undefined_type ret;
  };



  namespace internal {


    /// Function f_where: T -> type.

    template <typename T>
    struct f_where
    {
      typedef typename
      mlc::if_< mlc_is_a(T, ntg::value),
		where<T, ntg::value>, typename 
		mlc::if_< mlc_is_a(T, abstract::image),
			  where<T, abstract::image>, typename 
			  mlc::if_< mlc_is_a(T, value_box),
				    where<T, value_box>,
				    where<T>
                                   >::ret
                         >::ret
              >::ret
      ret;
    // FIXME: to be continued...
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


#endif // ! OLN_UTILS_OVERLOAD_HH
