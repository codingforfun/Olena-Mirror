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

#ifndef MLN_WORLD_KN_HQUEUE_HH
# define MLN_WORLD_KN_HQUEUE_HH

/// \file
/// \brief Class for hierarchical queues.


# include <iostream>
# include <vector>
# include <mln/world/kn/internal/queue.hh>
# include <mln/value/interval.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {

      /// \brief Class for hierarchical queues.
      template <typename E, typename P>
      class hqueue
      {
      public:

	typedef E element;
	typedef P priority;

	hqueue();
	hqueue(const value::interval<P>& inter);
	hqueue(const P& first, const P& last);

	const value::interval<P>& inter() const;
	unsigned nelements() const;

	bool is_empty() const;
	bool is_empty_at(const P& bucket) const;
	bool is_empty_at_index(unsigned i) const;

	void push_at(const E& elt, const P& bucket);
	E pop_at(const P& bucket);

	// FIXME: add some reserve strategies...

      private:
	std::vector< internal::queue_<E> > v_;
	value::interval<P> inter_;
	unsigned n_;
      };


# ifndef MLN_INCLUDE_ONLY


      template <typename E, typename P>
      hqueue<E,P>::hqueue()
      {
	n_ = 0;
      }

      template <typename E, typename P>
      hqueue<E,P>::hqueue(const value::interval<P>& inter)
      {
	v_.resize(inter.nelements());
	inter_ = inter;
	n_ = 0;
      }

      template <typename E, typename P>
      hqueue<E,P>::hqueue(const P& first, const P& last)
      {
	v_.resize(inter_.nelements());
	inter_ = value::interval<P>(first, last);
	n_ = 0;
      }

      template <typename E, typename P>
      const value::interval<P>&
      hqueue<E,P>::inter() const
      {
	return inter_;
      }

      template <typename E, typename P>
      unsigned
      hqueue<E,P>::nelements() const
      {
	return n_;
      }

      template <typename E, typename P>
      bool
      hqueue<E,P>::is_empty_at(const P& bucket) const
      {
	unsigned i = inter_.index_of(bucket);
	mln_precondition(i < v_.size());
	return v_[i].is_empty();
      }

      template <typename E, typename P>
      bool
      hqueue<E,P>::is_empty_at_index(unsigned i) const
      {
	mln_precondition(i < v_.size());
	return v_[i].is_empty();
      }

      template <typename E, typename P>
      bool
      hqueue<E,P>::is_empty() const
      {
	return n_ == 0;
      }

      template <typename E, typename P>
      void
      hqueue<E,P>::push_at(const E& elt, const P& bucket)
      {
	unsigned i = inter_.index_of(bucket);
	mln_precondition(i < v_.size());
	v_[i].push(elt);
	n_ += 1;
      }

      template <typename E, typename P>
      E
      hqueue<E,P>::pop_at(const P& bucket)
      {
	mln_precondition(inter_.has(bucket));
	unsigned i = inter_.index_of(bucket);
	mln_precondition(i < v_.size());
	mln_precondition(! is_empty_at_index(i));
	mln_precondition(n_ > 0);
	n_ -= 1;
	return v_[i].pop();
      }

      // template <typename E, typename P>
      // std::ostream&
      // operator<<(std::ostream& ostr, const hqueue<E,P>& q)
      // {
      //   unsigned n = q.size();
      //   ostr << '(';
      //   for (unsigned i = 0; i < n; ++i)
      // 	ostr << q.v_[i] << (i + 1 == n ? "" : ", ");
      //   return ostr << ')';
      // }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_HQUEUE_HH
