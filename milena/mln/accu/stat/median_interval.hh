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

#ifndef MLN_ACCU_STAT_MEDIAN_ALT_HH
# define MLN_ACCU_STAT_MEDIAN_INTERVAL_HH

/// \file
///
/// Define a median accumulator class accepting values restricted to a
/// specific interval of values.

# include <mln/accu/internal/base.hh>
# include <mln/value/interval.hh>
# include <mln/value/intsub.hh>
# include <mln/util/array.hh>
# include <mln/value/dec.hh>
# include <mln/value/inc.hh>
# include <mln/value/prev.hh>
# include <mln/value/succ.hh>

namespace mln
{

  namespace accu
  {

    namespace stat
    {


      /// \brief Generic median_interval function based on histogram over a
      /// value set with type \c S.
      ///
      /// \ingroup modaccuvalues
      //
      template <unsigned n, unsigned m=2*n>
      struct median_interval
	: public mln::accu::internal::base< const value::intsub<2*n>&, median_interval<n> >
      {
	typedef value::intsub<n> argument;
	typedef value::intsub<1> index_t;

	median_interval(const value::interval<argument>& inter);
	median_interval(const argument& first, const argument& last);

	/// Manipulators.
	/// \{
	void   take(const argument& t);
	void   take(const median_interval& other);
	void untake(const argument& t);
	void init();
	/// \}

	/// Get the value of the accumulator.
	const value::intsub<2*n>& to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:

	mutable util::array<unsigned> h_;
	unsigned sum_;

	mutable unsigned sum_minus_, sum_plus_;

	mutable bool result_ready;

	/// the median value
	mutable value::intsub<2*n> t_;

	/// The median index.
	mutable index_t n_;

	value::interval<argument > inter_;

	// Auxiliary methods
	void go_minus_() const;
	void go_plus_() const;
	void update_() const;

	index_t find_n1_() const;

      };

    } // end of mln::accu::stat


# ifndef MLN_INCLUDE_ONLY

    namespace stat
    {

      template <unsigned n, unsigned m>
      median_interval<n,m>::median_interval(const value::interval<argument >& inter)
	: inter_(inter)
      {
	init();
      }

      template <unsigned n, unsigned m>
      median_interval<n,m>::median_interval(const argument& first, const argument& last)
	: inter_(first, last)
      {
	init();
      }

      template <unsigned n, unsigned m>
      void
      median_interval<n,m>::take(const argument& t)
      {
	mln_precondition(inter_.has(t));

	unsigned index = inter_.index_of(t);

	// update h_
	++h_[index];
	++sum_;

	if (index < n_)
	  ++sum_minus_;
	else if (index > n_)
	  ++sum_plus_;

	if (result_ready)
	  result_ready = false;
      }

      template <unsigned n, unsigned m>
      inline
      void
      median_interval<n,m>::take(const median_interval<n,m>& other)
      {
	mln_precondition(inter_ == other.inter_);

	for (unsigned i = 0; i < h_.size(); ++i)
	  h_[i] += other.h_[i];

	// sum_minus_
	for (unsigned i = 0; i < n_; ++i)
	  sum_minus_ += other.h_[i];

	// sum_plus_
	for (unsigned i = n_ + 1; i < h_.nelements(); ++i)
	  sum_plus_ += other.h_[i];

	if (result_ready)
	  result_ready = false;
      }

      template <unsigned n, unsigned m>
      void
      median_interval<n,m>::untake(const argument& t)
      {
	mln_precondition(inter_.has(t));
	mln_precondition(h_[inter_.index_of(t)] != 0);

	unsigned index = inter_.index_of(t);

	// update h_
	--h_[index];
	--sum_;

	if (index < n_)
	  --sum_minus_;
	else if (index > n_)
	  --sum_plus_;

	if (result_ready)
	  result_ready = false;
      }

      template <unsigned n, unsigned m>
      void
      median_interval<n,m>::init()
      {
	h_.resize(inter_.nvalues(), 0);
	h_.fill(0);
	sum_minus_ = 0;
	sum_plus_ = 0;
	sum_ = 0;
	n_ = int(inter_.nvalues() / 2);
	t_ = inter_.ith_element(n_);
	result_ready = true;
      }

      template <unsigned n, unsigned m>
      const value::intsub<2*n>&
      median_interval<n,m>::to_result() const
      {
	mln_precondition(is_valid());

	if (! result_ready)
	  update_();
	return t_;
      }

      template <unsigned n, unsigned m>
      bool
      median_interval<n,m>::is_valid() const
      {
	return sum_ > 1;
      }

      template <unsigned n, unsigned m>
      inline
      void
      median_interval<n,m>::update_() const
      {
	result_ready = true;

	if (sum_ < 1)
	  return;

	if (2 * sum_minus_ >= sum_)
	  go_minus_();
	else
	  if (2 * sum_plus_ >= sum_)
	    go_plus_();
	  else // Both sides are balanced
	    if (h_[n_] == 0 || sum_ == 2)
	      go_minus_();
      }

      template <unsigned n, unsigned m>
      void
      median_interval<n,m>::go_minus_() const
      {
	do
	  {
	    sum_plus_ += h_[n_];
	    do
	      dec(n_);
	    while (h_[n_] == 0);
	    sum_minus_ -= h_[n_];
	  }
	while (2 * sum_minus_ > sum_);

	// The number of values is odd, the current index corresponds
	// to the exact median.
	// OR
	// We need to take the mean of the (n/2)-th and (n/2+1)-th value, so
	// if this is the same value, we just return it.
	if (sum_ % 2 || (sum_minus_ + h_[n_] > sum_ / 2))
	{
	  t_ = inter_.ith_element(n_);
	  return;
	}

	// The number of values is even, we need to effectively take
	// the mean of the (n/2)-th and (n/2+1)-th values which are
	// different.
	index_t n1 = find_n1_();
	if (n1 == h_.nelements())
	  n1 = n_;

	value::intsub<2> index = mean(n_, n1);

	t_ = inter_.ith_element(index)
	  + (index.is_integer() ? literal::zero
	     : value::iota<value::intsub<2*n> >::value());
      }

      template <unsigned n, unsigned m>
      void
      median_interval<n,m>::go_plus_() const
      {
	do
	  {
	    sum_minus_ += h_[n_];
	    do
	      inc(n_);
	    while (h_[n_] == 0);
	    sum_plus_ -= h_[n_];
	  }
	while (2 * sum_plus_ > sum_);


	// The number of values is odd, the current index corresponds
	// to the exact median.
	// OR
	// We need to take the mean of the (n/2)-th and (n/2+1)-th value, so
	// if this is the same value, we just return it.
	if (sum_ % 2 || (sum_minus_ + h_[n_] > sum_ / 2))
	{
	  t_ = inter_.ith_element(n_);
	  return;
	}

	// The number of values is even, we need to effectively take
	// the mean of the (n/2)-th and (n/2+1)-th values which are
	// different.
	index_t n1 = find_n1_();
	if (n1 == -1)
	  n1 = n_;

	value::intsub<2> index = mean(n_, n1);

	t_ = inter_.ith_element(index)
	  + (index.is_integer() ? literal::zero
	     : value::iota<value::intsub<2*n> >::value());
      }

      template <unsigned n, unsigned m>
      typename median_interval<n,m>::index_t
      median_interval<n,m>::find_n1_() const
      {
	index_t id = value::succ(n_);
	while (id < h_.nelements() && h_[id] == 0)
	  inc(id);

	return id;
      }


      template <unsigned n, unsigned m>
      std::ostream& operator<<(std::ostream& ostr, const median_interval<n,m>& med)
      {
	return ostr << med.to_result();
      }

    } // end of namespace mln::accu::stat

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_STAT_MEDIAN_INTERVAL_HH
