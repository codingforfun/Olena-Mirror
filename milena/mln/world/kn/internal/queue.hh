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

#ifndef MLN_WORLD_KN_INTERNAL_QUEUE_HH
# define MLN_WORLD_KN_INTERNAL_QUEUE_HH

/// \file
/// \brief Class for hierarchical queues.


# include <vector>



namespace mln
{

  namespace world
  {

    namespace kn
    {

      namespace internal
      {

	/// \internal
	/// \brief Classical queue structure.
	template <typename T>
	struct queue_
	{
	  queue_();

	  void reserve(unsigned n);

	  unsigned nelements() const;
	  bool is_empty() const;

	  const T& operator[](unsigned i) const;

	  void push(const T& t);
	  T pop();

	  std::vector<T> v_;
	  unsigned head_;
	};


# ifndef MLN_INCLUDE_ONLY


	template <typename T>
	queue_<T>::queue_()
	{
	  head_ = 0;
	}

	template <typename T>
	void
	queue_<T>::reserve(unsigned n)
	{
	  v_.reserve(n);
	}

	template <typename T>
	unsigned
	queue_<T>::nelements() const
	{
	  mln_precondition(head_ >= v_.size());
	  return v_.size() - head_;
	}

	template <typename T>
	bool
	queue_<T>::is_empty() const
	{
	  return head_ == v_.size();
	}

	template <typename T>
	const T&
	queue_<T>::operator[](unsigned i) const
	{
	  mln_precondition(head_ + i < v_.size());
	  return v_[head_ + i];
	}

	template <typename T>
	void
	queue_<T>::push(const T& t)
	{
	  v_.push_back(t);
	}

	template <typename T>
	T
	queue_<T>::pop()
	{
	  mln_precondition(! is_empty());
	  mln_precondition(head_ < v_.size());
	  return v_[head_++];
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::world::kn::internal

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_INTERNAL_QUEUE_HH
