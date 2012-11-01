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

#ifndef MLN_WORLD_KN_ACCU_MEDIAN_FEW_HH
# define MLN_WORLD_KN_ACCU_MEDIAN_FEW_HH

/// \file
///
/// \brief Define a median accumulator class to be used with few
/// values.

# include <iostream>
# include <vector>
# include <algorithm>

# include <mln/trait/routine/mean.hh>
# include <mln/accu/internal/base.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {

      namespace accu
      {

	/// \brief Define a median accumulator class to be used with few
	/// values.
	template <typename T,
		  typename R = mln_trait_routine_mean(2,T) >
	struct median_few
	  : public mln::accu::internal::base< R, median_few<T,R> >
	{
	  typedef T argument;
	  typedef R result;

	  median_few();

	  void reserve(unsigned n);
	  void init();

	  void take(const argument& v);
	  void take(const median_few<T,R>& other);
	  // nota bene: no possible untake with this accumulator

	  result to_result() const;

	  bool is_valid() const;

	protected:

	  mutable std::vector<argument> v_;
	};


# ifndef MLN_INCLUDE_ONLY

	template <typename T, typename R>
	median_few<T,R>::median_few()
	{
	  init();
	}

	template <typename T, typename R>
	void
	median_few<T,R>::reserve(unsigned n)
	{
	  v_.reserve(n);
	}

	template <typename T, typename R>
	void
	median_few<T,R>::init()
	{
	  v_.clear();
	}

	template <typename T, typename R>
	void
	median_few<T,R>::take(const argument& v)
	{
	  v_.push_back(v);
	}

	template <typename T, typename R>
	void
	median_few<T,R>::take(const median_few<T,R>& other)
	{
	  v_.insert(v_.end(), other.v_.begin(), other.v_.end());
	}


	template <typename T, typename R>
	typename median_few<T,R>::result
	median_few<T,R>::to_result() const
	{
	  mln_precondition(is_valid());
	  std::sort(v_.begin(), v_.end());
	  // indices 0 1 2 => size = 3 => mid = 1
	  //         0 1   => size = 2 => mid = 0
	  unsigned mid = (v_.size() - 1) / 2;
	  if (v_.size() % 2)
	    return v_[mid];
	  else
	    return (R(v_[mid]) + R(v_[mid + 1])) / 2;
	}

	template <typename T, typename R>
	bool
	median_few<T,R>::is_valid() const
	{
	  return v_.size() != 0;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::world::kn::accu

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_ACCU_MEDIAN_FEW_HH
