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

#ifndef MLN_VALUE_INTERVAL_HH
# define MLN_VALUE_INTERVAL_HH

/// \file
///
/// Define an interval.

# include <cstdlib>
# include <iostream>
# include <mln/value/inc.hh>
# include <mln/value/concept/interval.hh>


namespace mln
{

  namespace value
  {


    /// \brief Interval of values.
    template <typename T>
    class interval : public value::Interval<interval<T> >
    {
    public:
      typedef T enc;
      typedef T equiv;

      interval();
      interval(T single);
      interval(T first, T last);

      interval& operator=(const interval& rhs);

      /// Return True if a value is within this interval.
      bool has(const T& v) const;

      /// Return the distance between the first and the last value.
      T length() const;

      /// Return the ith value in this interval.
      T ith_element(unsigned i) const;

      /// Return the index of value \p v in this interval.
      unsigned index_of(const T& v) const;

      /// Return the number of values in this interval.
      unsigned nelements() const;

      /// Return True if this interval contains only one value.
      bool is_degenerated() const;

      /// The first value included in this interval.
      const T& first() const;
      /// The last value included in this interval.
      const T& last() const;

      /// Open this interval and exclude first and last values from
      /// it. The interval is then defined on [first + iota, last -
      /// iota].
      void self_open();

    private:
      T first_;
      T last_;
      unsigned nelements_;
    };

    //  comparison

    template <typename T>
    bool
    operator==(const interval<T>& lhs, const interval<T>& rhs);

    template <typename T>
    bool
    operator!=(const interval<T>& lhs, const interval<T>& rhs);

    // deactivation of ordering related operators

    template <typename T>
    void operator<(const interval<T>&, const interval<T>&);

    template <typename T>
    void operator<=(const interval<T>&, const interval<T>&);

    template <typename T>
    void operator>(const interval<T>&, const interval<T>&);

    template <typename T>
    void operator>=(const interval<T>&, const interval<T>&);

    //  set ops

    /*! \p r1 and \p r2 intersection is empty and the number of
      elements in the span is equal to the sum of the number of
      elements in \p r1 and \p r2.

      \verbatim
          span(r1,r2)
      <-------------------->
          r1         r2
      [x--x--x] [x--x--x--x]
      <------->-<---------->
          ^    ^     ^
	  |    |     |
	  |  iota    |
	  |          |
      length(r1)  length(r2)
      \endverbatim

    */
    template <typename T>
    bool
    are_adjacent(const interval<T>& r1, const interval<T>& r2);

    /* \brief Return true if \p r1 and \p r2 intersect.
     */
    template <typename T>
    bool
    do_intersect(const interval<T>& r1, const interval<T>& r2);

    /* \brief Perform the intersection of \p r1 and \p r2.
     */
    template <typename T>
    interval<T>
    inter(const interval<T>& r1, const interval<T>& r2);

    //  min / max

    /*! \brief Re-implementation of the min function.
      \sa math::min
    */
    template <typename T>
    interval<T>
    min_(const interval<T>& r1, const interval<T>& r2);

    /*! \brief Re-implementation of the max function.
      \sa math::max
    */
    template <typename T>
    interval<T>
    max_(const interval<T>& r1, const interval<T>& r2);

    /// \brief Compute the span of \p r1 and \p r2.
    template <typename T>
    interval<T>
    span(const interval<T>& r1, const interval<T>& r2);

    /// \brief Compute the span of \p r1, \p r2, \p r3 and \p r4.
    template <typename T>
    interval<T>
    span(const interval<T>& r1, const interval<T>& r2,
	 const interval<T>& r3, const interval<T>& r4);

    //  op<<

    template <typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const interval<T>& i);


  } // end of namespace mln::value

} // end of namespace mln


// for sorting purpose

namespace std
{

  template <typename T>
  struct less< mln::value::interval<T> >
  {
    bool operator()(const mln::value::interval<T>& l,
		    const mln::value::interval<T>& r) const;
  };

} // std



# ifndef MLN_INCLUDE_ONLY


namespace mln
{

  namespace value
  {


    template <typename T>
    interval<T>::interval()
    {
    }

    template <typename T>
    interval<T>::interval(T single)
    {
      first_ = single;
      last_ = single;

      nelements_ = 1;
    }


    template <typename T>
    interval<T>::interval(T first, T last)
    {
      mln_precondition(last >= first);
      first_ = first;
      last_ = last;

      nelements_ = 0;
      for (T v = first_; v <= last_; value::inc(v))
	++nelements_;
    }

    template <typename T>
    interval<T>&
    interval<T>::operator=(const interval& rhs)
    {
      first_ = rhs.first_;
      last_ = rhs.last_;
      return *this;
    }

    template <typename T>
    bool
    interval<T>::has(const T& v) const
    {
      return first_ <= v && v <= last_;
    }

    template <typename T>
    T
    interval<T>::length() const
    {
      return last_ - first_;
    }

    template <typename T>
    T
    interval<T>::ith_element(unsigned i) const
    {
      mln_precondition(i < nelements());
      return first_ + i * iota<T>::value();
    }

    template <typename T>
    unsigned
    interval<T>::index_of(const T& v) const
    {
      mln_precondition(has(v));
      return (v - first_) / iota<T>::value();
    }

    template <typename T>
    unsigned
    interval<T>::nelements() const
    {
      return nelements_;
    }

    template <typename T>
    bool
    interval<T>::is_degenerated() const
    {
      return last_ == first_;
    }

    template <typename T>
    const T&
    interval<T>::first() const
    {
      return first_;
    }

    template <typename T>
    const T&
    interval<T>::last() const
    {
      return last_;
    }

    template <typename T>
    void
    interval<T>::self_open()
    {
      if (is_degenerated())
	abort();
      mln_precondition(nelements_ > 2);

      first += iota<T>::value();
      last_ -= iota<T>::value();
      nelements_ -= 2;
    }


    //  comparison

    template <typename T>
    bool
    operator==(const interval<T>& lhs, const interval<T>& rhs)
    {
      return lhs.first() == rhs.first() && lhs.last() == rhs.last();
    }

    template <typename T>
    bool
    operator!=(const interval<T>& lhs, const interval<T>& rhs)
    {
      return ! (lhs == rhs);
    }


    // set ops

    template <typename T>
    bool
    are_adjacent(const interval<T>& r1, const interval<T>& r2)
    {
      return span(r1, r2).length() == r1.length() + r2.length()
	+ value::iota<T>::value();
    }

    template <typename T>
    bool
    do_intersect(const interval<T>& r1, const interval<T>& r2)
    {
      return span(r1, r2).length() <= r1.length() + r2.length();
    }

    template <typename T>
    interval<T>
    inter(const interval<T>& r1, const interval<T>& r2)
    {
      mln_precondition(do_intersect(r1, r2));
      return interval<T>(std::max(r1.first(), r2.first()),
			 std::min(r1.last(), r2.last()));
    }


    template <typename T>
    interval<T>
    min_(const interval<T>& r1, const interval<T>& r2)
    {
      return interval<T>(std::min(r1.first(), r2.first()),
			 std::min(r1.last(), r2.last()));
    }

    template <typename T>
    interval<T>
    max_(const interval<T>& r1, const interval<T>& r2)
    {
      return interval<T>(std::max(r1.first(), r2.first()),
			 std::max(r1.last(), r2.last()));
    }


    //  span

    template <typename T>
    interval<T>
    span(const interval<T>& r1, const interval<T>& r2)
    {
      return interval<T>(std::min(r1.first(), r2.first()),
			 std::max(r1.last(), r2.last()));
    }

    template <typename T>
    interval<T>
    span(const interval<T>& r1, const interval<T>& r2,
	 const interval<T>& r3, const interval<T>& r4)
    {
      return span(span(r1, r2), span(r3, r4));
    }



    //  op<<

    template <typename T>
    std::ostream&
    operator<<(std::ostream& ostr, const interval<T>& i)
    {
      if (i.is_degenerated())
	return ostr << '{' << i.first() << '}';
      else
	return ostr << '[' << i.first() << ',' << i.last() << ']';
    }


  } // end of namespace mln::value

} // end of namespace mln


namespace std
{

  template <typename T>
  bool less< mln::value::interval<T> >::operator()(
    const mln::value::interval<T>& l,
    const mln::value::interval<T>& r) const
  {
    mln_precondition(l.is_degenerated() && r.is_degenerated());
    return l.first() < r.first();
  }

} // std


# endif // ! MLN_INCLUDE_ONLY

#endif // ndef MLN_VALUE_INTERVAL_HH
