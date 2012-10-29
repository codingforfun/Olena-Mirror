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

#ifndef MLN_UTIL_HQUEUE_HH
# define MLN_UTIL_HQUEUE_HH

/// \file
/// \brief Class for hierarchical queues.


# include <cstdlib>
# include <iostream>
# include <vector>
# include <queue>


namespace mln
{

  namespace util
  {

    /// \brief Class for hierarchical queues.
    template <typename T, typename P>
    class hqueue
    {
    public:
      hqueue();
      hqueue(const value::interval<P>& inter);
      hqueue(const P& first, const P& last);

      unsigned size() const;

      bool is_empty(const P& bucket) const;
      bool is_not_empty(const P& bucket) const;

//      const std::queue<T>& operator[](const P& bucket) const;

      void push(const T& t, const P& bucket);
      T pop(const P& bucket);

    private:
      std::vector<std::queue<T> > v_;
      unsigned head_;
      value::interval<P> inter_;
    };


# ifndef MLN_INCLUDE_ONLY


    template <typename T, typename P>
    hqueue<T,P>::hqueue()
    {
      head_ = 0;
    }

    template <typename T, typename P>
    hqueue<T,P>::hqueue(const value::interval<P>& inter)
    {
      inter_ = inter;
      v_.resize(inter.nelements());
      head_ = 0;
    }

    template <typename T, typename P>
    hqueue<T,P>::hqueue(const P& first, const P& last)
    {
      inter_ = value::interval<P>(first, last);
      v_.resize(inter_.nelements());
      head_ = 0;
    }

    template <typename T, typename P>
    unsigned
    hqueue<T,P>::size() const
    {
      mln_precondition(head_ <= v_.size());
      return v_.size() - head_;
    }

    template <typename T, typename P>
    bool
    hqueue<T,P>::is_empty(const P& bucket) const
    {
      return v_[inter_.index_of(bucket)].empty() == 0;
    }

    template <typename T, typename P>
    bool
    hqueue<T,P>::is_not_empty(const P& bucket) const
    {
      return ! is_empty(bucket);
    }

    // template <typename T, typename P>
    // const std::queue<T>&
    // hqueue<T,P>::operator[](const P& i) const
    // {
    //   mln_precondition(i < this->size());
    //   return v_[head_ + i];
    // }

    template <typename T, typename P>
    void
    hqueue<T,P>::push(const T& t, const P& bucket)
    {
      v_[inter_.index_of(bucket)].push(t);
    }

    template <typename T, typename P>
    T
    hqueue<T,P>::pop(const P& bucket)
    {
      mln_precondition(!is_empty(bucket));
      mln_precondition(bucket < v_.size());
      T front = v_[bucket].front();
      v_[bucket].pop();
      return front;
    }


    template <typename T, typename P>
    std::ostream&
    operator<<(std::ostream& ostr, const hqueue<T,P>& q)
    {
      unsigned n = q.size();
      ostr << '(';
      for (unsigned i = 0; i < n; ++i)
	ostr << q.v_[i] << (i + 1 == n ? "" : ", ");
      return ostr << ')';
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_HQUEUE_HH
