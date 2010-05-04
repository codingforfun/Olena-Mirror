// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef PARALLEL_APPLY_HH
# define PARALLEL_APPLY_HH

///
/// Milena concept check for TBB wrapper.
/// FIXME: it's currently only for 2D images.
///


#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <tbb/partitioner.h>

#include <mln/core/alias/box2d.hh>
#include <mln/metal/is.hh>
#include <mln/debug/println.hh>

namespace mln
{

  template < typename E>
  struct Parallel_app : Object< E >
  {
    Parallel_app();
  };

# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Parallel_app<E>::Parallel_app()
  {
    typedef typename E::domain_t domain_t;
    mlc_is(domain_t, box2d)::check();

    // // Constructor check
    // E c(exact(*this), tbb::split ());
    // (void) c;

    void (E::*m1)(const tbb::blocked_range<int>&) = &E::operator();
    m1 = 0;

    void (E::*m2)(E&) = &E::join;
    m2 = 0;
  }

  template <typename T, typename F>
  void parallel_apply(const image2d<T>& ima, Parallel_app<F>& fun_)
  {
    point2d pmin = ima.domain().pmin();
    point2d pmax = ima.domain().pmax();
    F& fun = exact(fun_);
    tbb::blocked_range<int> r(pmin[0], pmax[0] + 1, 3);
    std::cout << "Entree" << std::endl;

    tbb::auto_partitioner partitioner;
    tbb::parallel_reduce(r, fun, partitioner);
    std::cout << "Sortie" << std::endl;
  }

# endif // ! MLN_INCLUDE_ONLY
}

#endif // !PARALLEL_APPLY_HH
