// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_UTILS_STAT_HH
# define OLENA_UTILS_STAT_HH

# include <oln/basics.hh>
# include <ntg/basics.hh>

namespace oln {

  namespace utils {

    /// Unary function that stores the min and the max.
    template< class T >
    struct f_minmax : std::unary_function< const T&, void >
    {
      f_minmax()
      {
        reset();
      }

      void
      operator()(const T& val)
      {
        if (! valued())
	  min_ = max_ = val;
	else if (val < min_) // FIXME: use a ftor instead of > and <
          min_ = val;
        else if (val > max_)
	  max_ = val;
	++count_;
      }

      void
      reset()
      {
        count_ = 0;
      }

      /// True if a value has been tested.
      bool
      valued() const
      {
        return count_;
      }

      /// Number of value has been tested.
      size_t
      count() const
      {
        return count_;
      }

      /// Minimum found.
      const T
      min() const
      {
        assertion(valued());
        return min_;
      }
      /// Maximum found.
      const T
      max() const
      {
        assertion(valued());
        return max_;
      }

    protected:
      size_t count_;
      T min_;
      T max_;
    };

    /// Computes the mean, the variance and store the min, the max.
    template< class T, class C = ntg::float_s >
    struct f_moments : f_minmax<T>
    {
      typedef f_minmax<T> super;

      void
      operator()(const T& val)
      {
	if (! this->valued())
	  {
	    sum1_ = ntg_zero_val(C);
	    sum2_ = ntg_zero_val(C);
	  }
	else
	  {
	    sum1_ += val;
	    sum2_ += C(val) * val;
	  }
	super::operator()(val);
      }

      /// Return the mean value.
      const C
      mean() const
      {
        assertion(this->valued());
        return sum1_ / C(this->count());
      }
      /// Return the variance.
      const C
      variance() const
      {
        assertion(this->valued());
        return sum2_ / C(this->count()) - mean() * mean();
      }

    protected:
      C sum1_;
      C sum2_;
    };

  } // end of namespace utils

} // end of namespace oln

#endif // ! OLENA_STAT_HISTOGRAM_HH
