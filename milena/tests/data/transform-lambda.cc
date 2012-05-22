// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
/// Exercise mln::data::transform with a lambda abstraction.

#include <type_traits>

#include <mln/core/concept/function.hh>
#include <mln/fun/c.hh>

#include <mln/core/image/image2d.hh>

#include <mln/data/transform.hh>
#include <mln/debug/iota.hh>

#include <mln/test/predicate.hh>


/*---------------------------------------------------.
| Extension of mln::fun::C to 2-argument functions.  |
`---------------------------------------------------*/

// FIXME: To be completed, revamped and moved into Milena.
namespace mln
{

  /// Category declaration for a unary C function.
  template <typename R, typename A1, typename A2>
  struct category< R (*)(A1, A2) >
  {
    typedef C_Function<void> ret;
  };

  namespace fun
  {

    /// Wrapper of a pointer to a 2-argument function into a Milena
    /// function.
    template <typename R, typename A1, typename A2>
    struct C< R (*)(A1, A2) >
      :
      // FIXME: Hard-coded base class, so as to make this part of the
      // code as small as possible for this test.
      //
      // This functor should inherit from either `Function_vv2b' or
      // `Function_vv2b', depending on the result type.  See how it is
      // implemented in `C< R (*)(A) >' (located in mln/fun/c.hh).
      Function_vv2b< C< R (*)(A1, A2) > >
    {
      C() {}
      C(R (*f)(A1, A2)) : f_(f) {}
      typedef R result;
      R operator()(const mlc_unqualif(A1)& a, const mlc_unqualif(A2)& b) const
      {
	return f_(a, b);
      }
    protected:
      R (*f_)(A1, A2);
    };

  } // end of namespace mln::fun

} // end of namespace mln::fun


/*----------------.
| Lambda traits.  |
`----------------*/

// Metaprogramming trait for the deduction of the pointer-to-function
// type equivalent to the type of a lambda asbtraction.  Inspired from
// http://stackoverflow.com/questions/6512019/can-we-get-the-type-of-a-lambda-argument

template <typename F>
struct fun_ptr
{
  template <typename Ret, typename... Rest>
  static auto
  helper(Ret (F::*)(Rest...)) -> Ret(*)(Rest...);

  template <typename Ret, typename... Rest>
  static auto
  helper(Ret (F::*)(Rest...) const) -> Ret(*)(Rest...);

  typedef decltype( helper(&F::operator()) ) type;
};


/*---------------------.
| Conversion routine.  |
`---------------------*/

template <typename F>
auto
to_fun (F f) -> mln::fun::C< typename fun_ptr<F>::type >
{
  return mln::fun::C< typename fun_ptr<F>::type > (f);
}


/*-------.
| Test.  |
`-------*/

int
main()
{
  using namespace mln;

  // FIXME: Replacing unsigned by int_u8 would trigger a bug here, as
  // the (low quantization) implementation of data::transform would
  // create values that would not fit in int_u8 (e.g. sqr(16) = 256).
  typedef unsigned V;
  typedef image2d<V> I;

  I input(3, 3);
  debug::iota(input);

  I output = data::transform(input, to_fun([](V x) { return x * x; }));

  mln_assertion(test::predicate(output, input,
				to_fun([](V x, V y) { return x == y * y; })));
}
