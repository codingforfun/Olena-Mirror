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

#ifndef BENCH_HH
# define BENCH_HH

# ifndef WALL_CLOCK

#  include <ctime>

#  define START_BENCH(N_ITERATION)		\
  {						\
  clock_t start__ = std::clock();		\
  const int n_iteration__ = N_ITERATION;	\
  for (int i__ = 0; i__ < n_iteration__; ++i__) {


#  define END_BENCH(MSG)				\
  }							\
    std::cout << MSG					\
    << (float)(std::clock() - start__) * 1000.0 /	\
    (CLOCKS_PER_SEC * n_iteration__)			\
    << " ms." << std::endl;				\
    }

# else // !WALL_CLOCK

#  include <tbb/tick_count.h>

#  define START_BENCH(N_ITERATION)		\
  {						\
  tbb::tick_count t0__ = tbb::tick_count::now();	\
  const int n_iteration__ = N_ITERATION;	\
  for (int i__ = 0; i__ < n_iteration__; ++i__) {

#  define END_BENCH(MSG)				\
  }							\
    tbb::tick_count t1__ = tbb::tick_count::now();	\
    std::cout << MSG					\
    << (t1__ - t0__).seconds() * 1000 / n_iteration__	\
    << " ms." << std::endl;				\
    }

# endif

#endif // !BENCH_HH
