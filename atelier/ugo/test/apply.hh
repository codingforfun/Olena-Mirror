// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef OLN_LEVEL_APPLY_HH
# define OLN_LEVEL_APPLY_HH

# include <oln/core/concept/image.hh>
// FIXME: # include <oln/core/abstract/functions.hh>



# define  oln_ch_value(I, U)  typename oln::internal::f_ch_value<I,U>::ret


namespace oln
{

  // FIXME: Temporary code.

  namespace internal
  {

    template <typename I, typename U>
    struct f_ch_value;
    

    template <typename T, typename U>
    struct f_ch_value < image2d_b<T>, U >
    {
      typedef image2d_b<U> ret;
    };
    

  } // end of namespace oln::internal



  // FIXME: Move!!!

  mlc_decl_typedef(argument);
  mlc_decl_typedef(result);

  template <typename Exact>
  struct Function : public virtual Any<Exact>
  {
  };



  namespace level
  {

    /// Fwd decl.

    template <typename R, typename A, typename I>
    oln_ch_value(I, R) // FIXME
      apply(R (*fun)(A), const Image<I>& input);


    template <typename F, typename I>
    oln_ch_value(I, typename F::result)
      apply(const /*FIXME: Unary_*/Function<F>& fun, const Image<I>& input);


    template <typename F, typename I>
    oln_ch_value(I,
		 typename F:: template result<typename I::value> ::ret)
      apply(const /*Unary_Meta_*/Function<F>& fun, const Image<I>& input)
    {
	oln_ch_value(I, typename F:: template result<typename I::value> ::ret) output(input.points());
	oln_piter(I) p(input.points());
	for_all(p)
	  output(p) = exact(fun)(input(p));
	return output;
    }

    /*

    /// Fwd decl.
    template <typename I, typename F>
    oln_plain_value(I, typename F::result_value)
    apply(const abstract::image<I>& input, const abstract::fun_v2v<F>& fun);

    /// Fwd decl.
    template <typename I, typename V>
    void apply_inplace(abstract::mutable_image<I>& input, V (*fun)(const oln_value(I)&));

    /// Fwd decl.
    template <typename I, typename F>
    void apply_inplace(abstract::mutable_image<I>& input, const abstract::fun_v2v<F>& fun);

    */


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      /// Generic version.

      template <typename R, typename A, typename I>
      oln_ch_value(I, R) // FIXME: oln_plain_value(I, V)
	apply(R (*fun)(A), const Image<I>& input)
      {
	oln_ch_value(I, R) output(input.points());
	oln_piter(I) p(input.points());
	for_all(p)
	  output(p) = fun( static_cast<A>(input(p)) );
	return output;
      }


      template <typename F, typename I>
      oln_ch_value(I, typename F::result)
	apply_function(const F& fun, const Image<I>& input)
      {
	typedef typename F::argument A;
	typedef typename F::result   R;
	oln_ch_value(I, R) output(input.points());
	oln_piter(I) p(input.points());
	for_all(p)
	  output(p) = fun( static_cast<A>(input(p)) );
	return output;
      }
      

      /*

      /// Generic version.
      template <typename I, typename F>
      oln_plain_value(I, typename F::result_value)
      apply(const abstract::image<I>& input, const abstract::fun_v2v<F>& fun)
      {
      oln_plain_value(I, typename F::result_value) output(input.topo());
      oln_piter(I) p(input.topo());
      for_all(p)
      output(p) = fun.exact()(input(p));
      return output;
      }


      /// Generic version.
      template <typename I, typename V>
      void
      apply_inplace(abstract::mutable_image<I>& input, V (*fun)(const oln_value(I)&))
      {
      oln_piter(I) p(input.topo());
      for_all(p)
      input(p) = fun(input(p));
      }

      /// Generic version.
      template <typename I, typename F>
      void
      apply_inplace(abstract::mutable_image<I>& input, const abstract::fun_v2v<F>& fun)
      {
      oln_piter(I) p(input.topo());
      for_all(p)
      input(p) = fun.exact()(input(p));
      }

      */

    } // end of namespace oln::level::impl



    /// Facade.


    template <typename R, typename A, typename I>
    oln_ch_value(I, R) // FIXME: oln_plain_value(I, V)
      apply(R (*fun)(A), const Image<I>& input)
    {
      return impl::apply(fun, exact(input));
    }

    
    struct ERROR_bla_bla;


    template <typename F, typename I>
    oln_ch_value(I, typename F::result)
      apply(const Function<F>& fun, const Image<I>& input)
    {
      mlc::assert_found_< mlc_typedef(F, result), ERROR_bla_bla >::check();
      mlc::assert_found_< mlc_typedef(F, argument), ERROR_bla_bla >::check();
      return impl::apply_function(exact(fun), exact(input));
    }


    /*

    /// Facade.
    template <typename I, typename F>
    oln_plain_value(I, typename F::result_value)
    apply(const abstract::image<I>& input, const abstract::fun_v2v<F>& fun)
    {
    return impl::apply(input, fun);
    }


    /// Facade.
    template <typename I, typename V>
    void
    apply_inplace(abstract::mutable_image<I>& input, V (*fun)(const oln_value(I)&))
    {
    return impl::apply_inplace(input, fun);
    }

    /// Facade.
    template <typename I, typename F>
    void
    apply_inplace(abstract::mutable_image<I>& input, const abstract::fun_v2v<F>& fun)
    {
    return impl::apply_inplace(input, fun);
    }

    */

# endif

  } // end of namespace oln::level

} // end of namespace oln


#endif // ! OLN_LEVEL_APPLY_HH
