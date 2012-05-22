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
#include <mln/debug/println.hh>


/*----------------.
| Lambda traits.  |
`----------------*/

// Metaprogramming traits for the deduction of return and argument
// types of lambda asbtractions.  Inspired from
// http://stackoverflow.com/questions/6512019/can-we-get-the-type-of-a-lambda-argument

/// Deduce the type of \p F's first argument.
template <typename F>
struct first_arg_of
{
  template <typename Ret, typename A, typename... Rest>
  static A
  helper(Ret (F::*)(A, Rest...));

  template <typename Ret, typename A, typename... Rest>
  static A
  helper(Ret (F::*)(A, Rest...) const);

  typedef decltype( helper(&F::operator()) ) type;
};

/// Deduce the type of \p F's first argument.
///
/// This traits works with lambdas, whereas std::result_of does not.
template <typename F>
struct return_of
{
  template <typename Ret, typename A, typename... Rest>
  static Ret
  helper(Ret (F::*)(A, Rest...));

  template <typename Ret, typename A, typename... Rest>
  static Ret
  helper(Ret (F::*)(A, Rest...) const);

  typedef decltype( helper(&F::operator()) ) type;
};


/*---------------------.
| Conversion routine.  |
`---------------------*/

/// Convert a lambda abstraction into a Milena function (functor).

template <typename F>
auto
to_fun (F f) -> mln::fun::C< typename return_of<F>::type (*) (typename first_arg_of<F>::type) >
{
  return mln::fun::C< typename return_of<F>::type (*) (typename first_arg_of<F>::type) > (f);
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

  auto sqr = [](V x) { return x * x; };

  I output = data::transform(input, to_fun([](V x) { return x * x; }));
  debug::println(output);

  // FIXME: Or use mln::test instead?
  mln_piter_(I) p(output.domain());
  for_all(p)
    mln_assertion(output(p) = math::sqr(input(p)));
}
