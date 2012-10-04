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
      template <unsigned n>
      struct median_interval
	: public mln::accu::internal::base< const value::intsub<n>&, median_interval<n> >
      {
	typedef value::intsub<n> argument;

	median_interval(const value::interval<value::intsub<n> >& inter);
	median_interval(const value::intsub<n>& first, const value::intsub<n>& last);

	/// Manipulators.
	/// \{
	void   take(const argument& t);
	void   take(const median_interval& other);
	void untake(const argument& t);
	void init();
	/// \}

	/// Get the value of the accumulator.
	const value::intsub<n>& to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:

	mutable util::array<unsigned> h_;
	unsigned sum_;

	mutable unsigned sum_minus_, sum_plus_;

	mutable bool valid_;

	/// the median index
	mutable unsigned i_;
	/// the median argument
	mutable value::intsub<n> t_;

	value::interval<value::intsub<n> > inter_;

	// Auxiliary methods
	void go_minus_() const;
	void go_plus_() const;
	void update_() const;
      };

    } // end of mln::accu::stat


# ifndef MLN_INCLUDE_ONLY

    namespace stat
    {

      template <unsigned n>
      median_interval<n>::median_interval(const value::interval<value::intsub<n> >& inter)
	: inter_(inter)
      {
	init();
      }

      template <unsigned n>
      median_interval<n>::median_interval(const value::intsub<n>& first, const value::intsub<n>& last)
	: inter_(first, last)
      {
	init();
      }

      template <unsigned n>
      void
      median_interval<n>::take(const argument& t)
      {
	mln_precondition(inter_.has(t));

	unsigned index = inter_.index_of(t);

	// update h_
	++h_[index];
	++sum_;

	if (index < i_)
	  ++sum_minus_;
	else if (index > i_)
	  ++sum_plus_;

	if (valid_)
	  valid_ = false;
      }

      template <unsigned n>
      inline
      void
      median_interval<n>::take(const median_interval<n>& other)
      {
	for (unsigned i = 0; i < h_.size(); ++i)
	  h_[i] += other.h_[i];

	// sum_minus_
	for (unsigned i = 0; i < i_; ++i)
	  sum_minus_ += other.h_[i];

	// sum_plus_
	for (unsigned i = i_ + 1; i < h_.nelements(); ++i)
	  sum_plus_ += other.h_[i];

	if (valid_)
	  valid_ = false;
      }


      template <unsigned n>
      void
      median_interval<n>::untake(const argument& t)
      {
	mln_precondition(inter_.has(t));
	mln_precondition(h_[inter_.index_of(t)] != 0);

	unsigned index = inter_.index_of(t);

	// update h_
	--h_[index];
	--sum_;

	if (index < i_)
	  --sum_minus_;
	else if (index > i_)
	  --sum_plus_;

	if (valid_)
	  valid_ = false;
      }

      template <unsigned n>
      void
      median_interval<n>::init()
      {
	h_.resize(inter_.nvalues(), 0);
	sum_minus_ = 0;
	sum_plus_ = 0;
	i_ = inter_.nvalues() / 2;
	t_ = inter_.ith_element(i_);
      }

      template <unsigned n>
      const value::intsub<n>&
      median_interval<n>::to_result() const
      {
	if (! valid_)
	  update_();
	return t_;
      }

      template <unsigned n>
      bool
      median_interval<n>::is_valid() const
      {
	return true;
      }

      template <unsigned n>
      inline
      void
      median_interval<n>::update_() const
      {
	valid_ = true;

	if (sum_ == 0)
	  return;

	if (2 * sum_minus_ > sum_)
	  go_minus_();
	else
	  if (2 * sum_plus_ > sum_)
	    go_plus_();
	  else
	    if (h_[i_] == 0)
	      {
		// go to the heaviest side
		if (sum_plus_ > sum_minus_)
		  go_plus_();
		else
		  go_minus_(); // default when both sides are balanced
	      }
      }


      template <unsigned n>
      void
      median_interval<n>::go_minus_() const
      {
	do
	  {
	    sum_plus_ += h_[i_];
	    do
	      --i_;
	    while (h_[i_] == 0);
	    sum_minus_ -= h_[i_];
	  }
	while (2 * sum_minus_ > sum_);
	t_ = inter_.ith_element(i_);
      }


      template <unsigned n>
      void
      median_interval<n>::go_plus_() const
      {
	do
	  {
	    sum_minus_ += h_[i_];
	    do
	      ++i_;
	    while (h_[i_] == 0);
	    sum_plus_ -= h_[i_];
	  }
	while (2 * sum_plus_ > sum_);
	t_ = inter_.ith_element(i_);
      }

      template <unsigned n>
      std::ostream& operator<<(std::ostream& ostr, const median_interval<n>& m)
      {
	return ostr << m.to_result();
      }

    } // end of namespace mln::accu::stat

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_STAT_MEDIAN_INTERVAL_HH
