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

#ifndef MLN_WORLD_KN_ACCU_MEDIAN_H_INTERVAL_HH
# define MLN_WORLD_KN_ACCU_MEDIAN_H_INTERVAL_HH

/// \file
///
/// \brief Define a median accumulator class based on a histogram and
/// working on a specific interval.

# include <vector>
# include <mln/accu/internal/base.hh>
# include <mln/value/interval.hh>
# include <mln/trait/routine/mean.hh>
# include <mln/math/mean.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {

      namespace accu
      {

	/// \brief Define a median accumulator class based on a histogram and
	/// working on a specific interval.
	template <typename T,
		  typename R = mln_trait_routine_mean(2,T) >
	struct median_h_interval
	  : public mln::accu::internal::base< R, median_h_interval<T,R> >
	{
	  typedef T argument;
	  typedef R result;

	  median_h_interval(const value::interval<argument>& inter);
	  median_h_interval(const argument& first, const argument& last);
	  template <typename U, typename V>
	  median_h_interval(const U& first, const V& last);

	  void init();

	  void take(const argument& v);
	  void take(const median_h_interval<T,R>& v);

	  // nota bene: no untake with this accumulator

	  result to_result() const;

	  bool is_valid() const;

	protected:

	  value::interval<argument> inter_;
	  std::vector<unsigned> h_;
	  unsigned nitems_;
	};

# ifndef MLN_INCLUDE_ONLY

	template <typename T, typename R>
	median_h_interval<T,R>::median_h_interval(const value::interval<argument>& inter)
	  : inter_(inter)
	{
	  h_.resize(inter_.nelements());
	  init();
	}

	template <typename T, typename R>
	median_h_interval<T,R>::median_h_interval(const argument& first,
						  const argument& last)
	  : inter_(first, last)
	{
	  h_.resize(inter_.nelements());
	  init();
	}

	template <typename T, typename R>
	template <typename U, typename V>
	median_h_interval<T,R>::median_h_interval(const U& first,
						  const V& last)
	{
	  mlc_converts_to(U,T)::check();
	  mlc_converts_to(V,T)::check();

	  inter_ = value::interval<T>(first, last);
	  h_.resize(inter_.nelements());
	  init();
	}

	template <typename T, typename R>
	void
	median_h_interval<T,R>::init()
	{
	  std::fill(h_.begin(), h_.end(), 0u);
	  nitems_ = 0;
	}

	template <typename T, typename R>
	void
	median_h_interval<T,R>::take(const argument& v)
	{
	  h_[inter_.index_of(v)] += 1;
	  nitems_ += 1;
	}

	template <typename T, typename R>
	void
	median_h_interval<T,R>::take(const median_h_interval<T,R>& other)
	{
	  mln_precondition(other.inter_ == inter_);
	  for (unsigned i = 0; i < other.h_.size(); ++i)
	    h_[i] += other.h_[i];
	  nitems_ += other.nitems_;
	}

	template <typename T, typename R>
	typename median_h_interval<T,R>::result
	median_h_interval<T,R>::to_result() const
	{
	  if (!nitems_)
	    return result(math::mean(inter_.first(), inter_.last()));

	  mln_precondition(is_valid());
	  unsigned i = 0, n = h_.size();
	  unsigned sum = h_[0], mid = (nitems_ + 1) / 2;
	  while (i < n && sum < mid)
	  {
	    ++i;
	    sum += h_[i];
	  }
	  if (nitems_ % 2) // nitems_ is odd.
	    return inter_.ith_element(i);

	  // nitems_ is even.
	  if (sum > mid)
	    return inter_.ith_element(i);
	  // looking for next non null value
	  unsigned i_memo = i;
	  do
	    ++i;
	  while (h_[i] == 0);
	  return (result(inter_.ith_element(i_memo))
		  + result(inter_.ith_element(i))) / 2;
	}

	template <typename T, typename R>
	bool
	median_h_interval<T,R>::is_valid() const
	{
	  return nitems_ != 0;
	}


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::world::kn::accu

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_ACCU_MEDIAN_H_INTERVAL_HH
