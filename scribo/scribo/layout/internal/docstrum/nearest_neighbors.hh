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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_NEAREST_NEIGHBORS_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_NEAREST_NEIGHBORS_HH

# include <vector>
# include <climits>

# include <mln/core/image/image2d.hh>
# include <mln/math/pi.hh>

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


	struct nnpoint
	{
	  nnpoint();
	  nnpoint(const bbox2d* b, const unsigned d);

	  void compute_angle(const point2d& origin);

	  void root_distance();

	  bool operator< (const nnpoint& p) const;

	  unsigned id() const;

	  const bbox2d* bbox;
	  int distance;
	  float angle_h;
	  float angle_v;
	};


	class nearestn
	{
	public:
	  nearestn();
	  nearestn(const int n, const bbox2d* bbox);

	  void add_point(const nnpoint& p);
	  void compute_angles(const point2d& origin);

	  void root_distances();
	  int get_highest_distance() const;

	  const std::vector<nnpoint>& nearest_points() const;

	  int ninserted() const;

	  const nnpoint* nnp() const;

	private:
	  std::vector<nnpoint> neighbors_;
	  int inserted_;
	  int n_;
	  int highest_distance_;
	  nnpoint nnpoint_;
	};


# ifndef MLN_INCLUDE_ONLY

	//---------------------------
	// Nearest neighbor point
	//---------------------------

	inline
	nnpoint::nnpoint()
	  : bbox(0), distance(0), angle_h(0.0f), angle_v(0.0f)
	{
	}

	inline
	nnpoint::nnpoint(const bbox2d* b, const unsigned d)
	    : bbox(b), distance(d), angle_h(0.0f), angle_v(0.0f)
	{
	}

	inline
	void
	nnpoint::compute_angle(const point2d& origin)
	{
	  const point2d center = bbox->pcenter();
	  const float delta_x = center.col() - origin.col();
	  const float delta_y = center.row() - origin.row();

	  if (delta_x == 0 && delta_y > 0)
	    angle_h = math::pi / 2;
	  else if (delta_x == 0 && delta_y < 0)
	    angle_h = -math::pi / 2;
	  else
	  {
	    angle_h = atan(delta_y / delta_x);
	    if (angle_h > 0)
	      angle_v = angle_h - math::pi / 2;
	    else
	      angle_v = angle_h + math::pi / 2;
	  }
	}

	inline
	void
	nnpoint::root_distance()
	{
	  distance = sqrt(distance);
	}

	inline
	bool
	nnpoint::operator< (const nnpoint& p) const
	{
	  return (distance < p.distance);
	}

	inline
	unsigned
	nnpoint::id() const
	{
	  return bbox->bbox_number();
	}


	//---------------------------
	// Nearest neighbors
	//---------------------------


	inline
	nearestn::nearestn()
	  : inserted_(0), n_(0), highest_distance_(INT_MAX)
	{
	}

	inline
	nearestn::nearestn(const int n,
			   const bbox2d* bbox)
	  : inserted_(0), n_(n), highest_distance_(INT_MAX)
	{
	  for (int i = 0; i < n_; ++i)
	    neighbors_.push_back(nnpoint());

	  nnpoint_ = nnpoint(bbox, 0);
	}

	inline
	void
	nearestn::add_point(const nnpoint& p)
	{
	  int i = 0;

	  while (i < n_ && i < inserted_ && neighbors_[i] < p)
	    ++i;

	  if (i < n_)
	  {
	    for (int j = inserted_; j > i; --j)
	      neighbors_[j] = neighbors_[j - 1];

	    neighbors_[i] = p;

	    if (inserted_ < n_)
	      ++inserted_;

	    if (inserted_ >= n_)
	      highest_distance_ = neighbors_[n_ - 1].distance;
	  }
	}

	inline
	void
	nearestn::compute_angles(const point2d& origin)
	{
	  for (int i = 0; i < inserted_; ++i)
	    neighbors_[i].compute_angle(origin);
	}

	inline
	void
	nearestn::root_distances()
	{
	  for (int i = 0; i < inserted_; ++i)
	    neighbors_[i].root_distance();
	}

	inline
	int
	nearestn::get_highest_distance() const
	{
	  return highest_distance_;
	}

	inline
	const std::vector<nnpoint>&
	nearestn::nearest_points() const
	{
	  return neighbors_;
	}

	inline
	int
	nearestn::ninserted() const
	{
	  return inserted_;
	}

	inline
	const nnpoint*
	nearestn::nnp() const
	{
	  return &nnpoint_;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_NEAREST_NEIGHBORS_HH
