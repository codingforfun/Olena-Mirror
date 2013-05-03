// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_KD_NODE_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_KD_NODE_HH

#include <iostream>

# include <scribo/layout/internal/docstrum/bbox2d.hh>



namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;

	class kd_node
	{
	public:
	  kd_node(const bbox2d* bbox, const int father, const int dim);

	  bool is_leaf() const;

	  int dim() const;
	  const bbox2d* bbox() const;
	  int father() const;
	  int left_son() const;
	  int right_son() const;
	  int coord(const int dim) const;

	  int x() const;
	  int y() const;

	  void set_left_son(const int son_index);
	  void set_right_son(const int son_index);

	private:
	  int dim_;
	  const bbox2d* bbox_;
	  int father_;
	  int left_son_;
	  int right_son_;
	};


# ifndef MLN_INCLUDE_ONLY

	//---------------------------
	// KD node
	//---------------------------

	inline
	kd_node::kd_node(const bbox2d* bbox, const int father, const int dim)
	  : dim_(dim), bbox_(bbox), father_(father), left_son_(-1), right_son_(-1)
	{
	}

	inline
	bool
	kd_node::is_leaf() const
	{
	  return (left_son_ == -1 && right_son_ == -1);
	}

	inline
	int
	kd_node::dim() const
	{
	  return dim_;
	}

	inline
	const bbox2d*
	kd_node::bbox() const
	{
	  return bbox_;
	}

	inline
	int
	kd_node::father() const
	{
	  return father_;
	}

	inline
	int
	kd_node::left_son() const
	{
	  return left_son_;
	}

	inline
	int
	kd_node::right_son() const
	{
	  return right_son_;
	}

	inline
	int
	kd_node::coord(const int dim) const
	{
	  if (not dim)
	    return bbox_->pcenter().col();
	  else
	    return bbox_->pcenter().row();
	}

	inline
	int kd_node::x() const
	{
	  return bbox_->pcenter().col();
	}

	inline
	int
	kd_node::y() const
	{
	  return bbox_->pcenter().row();
	}

	inline
	void
	kd_node::set_left_son(const int son_index)
	{
	  left_son_ = son_index;
	}

	inline
	void
	kd_node::set_right_son(const int son_index)
	{
	  right_son_ = son_index;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_KD_NODE_HH
