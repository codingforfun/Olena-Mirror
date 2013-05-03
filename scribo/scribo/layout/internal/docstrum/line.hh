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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_LINE_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_LINE_HH

# include <queue>

# include <mln/core/image/image2d.hh>

# include <scribo/layout/internal/docstrum/bbox2d.hh>
# include <scribo/layout/internal/docstrum/nearest_neighbors.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;


	class line
	{
	public:
	  line(const nnpoint* p, const unsigned id);

	  void add_component(const nnpoint* p);

	  void linear_regression();

	  const bbox2d& bbox() const;

	  float slope() const;

	  float y_intercept() const;

	  const point2d& line_begin() const;

	  const point2d& line_end() const;

	  unsigned id() const;

	private:
	  std::vector<const nnpoint*> comp_;
	  float slope_;
	  float y_intercept_;
	  point2d line_begin_;
	  point2d line_end_;
	  bbox2d bbox_;
	};


# ifndef MLN_INCLUDE_ONLY

	//-------------------------
	// Line
	//-------------------------

	inline
	line::line(const nnpoint* p,
		   const unsigned id)
	{
	  bbox_.pmin() = p->bbox->pmin();
	  bbox_.pmax() = p->bbox->pmax();
	  bbox_.set_bbox_number(id);

	  comp_.push_back(p);
	  slope_ = 0.0f;
	  y_intercept_ = 0.0f;
	}

	inline
	void
	line::add_component(const nnpoint* p)
	{
	  comp_.push_back(p);

	  const point2d& pmin = p->bbox->pmin();
	  const point2d& pmax = p->bbox->pmax();
	  point2d& bbox_pmin = bbox_.pmin();
	  point2d& bbox_pmax = bbox_.pmax();

	  if (bbox_pmin.col() > pmin.col())
	    bbox_pmin.col() = pmin.col();

	  if (bbox_pmin.row() > pmin.row())
	    bbox_pmin.row() = pmin.row();

	  if (bbox_pmax.col() < pmax.col())
	    bbox_pmax.col() = pmax.col();

	  if (bbox_pmax.row() < pmax.row())
	    bbox_pmax.row() = pmax.row();
	}

	inline
	void
	line::linear_regression()
	{
	  const int nelements = comp_.size();

	  if (nelements == 1)
	  {
	    const bbox2d* b = comp_[0]->bbox;
	    slope_ = 0.0f;
	    y_intercept_ = b->pmin().row() + 0.5f * (b->pmax().row() - b->pmin().row());
	    line_begin_ = point2d(y_intercept_, bbox_.pmin().col());
	    line_end_ = point2d(y_intercept_, bbox_.pmax().col());

	    return;
	  }

	  int sum_x = 0;
	  int sum_y = 0;
	  int sum_xy = 0;
	  int sum_x_square = 0;

	  std::vector<const nnpoint*>::iterator it_begin = comp_.begin();
	  std::vector<const nnpoint*>::iterator it_end = comp_.end();

	  for (; it_begin != it_end; ++it_begin)
	  {
	    const point2d center = (*it_begin)->bbox->pcenter();
	    const int x = center.col();
	    const int y = center.row();

	    sum_x += x;
	    sum_y += y;
	    sum_xy += x * y;
	    sum_x_square += x * x;
	  }

	  slope_ = (float) (nelements * sum_xy - sum_x * sum_y) /
	    (nelements * sum_x_square - sum_x * sum_x);
	  y_intercept_ = ((float) sum_y / nelements) -
	    slope_ * ((float) sum_x / nelements);

	  const int y_begin = slope_ * bbox_.pmin().col() + y_intercept_;
	  const int y_end = slope_ * bbox_.pmax().col() + y_intercept_;
	  line_begin_ = point2d(y_begin, bbox_.pmin().col());
	  line_end_ = point2d(y_end, bbox_.pmax().col());
	}

	inline
	const bbox2d&
	line::bbox() const
	{
	  return bbox_;
	}

	inline
	float
	line::slope() const
	{
	  return slope_;
	}

	inline
	float
	line::y_intercept() const
	{
	  return y_intercept_;
	}

	inline
	const point2d&
	line::line_begin() const
	{
	  return line_begin_;
	}

	inline
	const point2d&
	line::line_end() const
	{
	  return line_end_;
	}

	inline
	unsigned
	line::id() const
	{
	  return bbox_.bbox_number();
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_LINE_HH
