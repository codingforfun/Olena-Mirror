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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_KD_HYPERRECT_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_KD_HYPERRECT_HH

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



	class kd_hyperrect
	{
	public:
	  kd_hyperrect();

	  int min_coord(const int dim) const;
	  int max_coord(const int dim) const;
	  void set_min_coord(const int dim, const int value);
	  void set_max_coord(const int dim, const int value);

	  void init(const bbox2d* bbox);

	  void extend(const bbox2d* bbox);

	  int distance(const bbox2d* bbox);

	  int vertical_distance(const bbox2d* bbox);

	private:
	  int min_[2];
	  int max_[2];
	};



# ifndef MLN_INCLUDE_ONLY


	//---------------------------
	// KD hyperrectangle
	//---------------------------

	inline
	kd_hyperrect::kd_hyperrect()
	{
	}

	inline
	int
	kd_hyperrect::min_coord(const int dim) const
	{
	  return min_[dim];
	}

	inline
	int
	kd_hyperrect::max_coord(const int dim) const
	{
	  return max_[dim];
	}

	inline
	void
	kd_hyperrect::set_min_coord(const int dim, const int value)
	{
	  min_[dim] = value;
	}

	inline
	void
	kd_hyperrect::set_max_coord(const int dim, const int value)
	{
	  max_[dim] = value;
	}

	inline
	void
	kd_hyperrect::init(const bbox2d* bbox)
	{
	  const point2d& center = bbox->pcenter();
	  min_[0] = max_[0] = center.col();
	  min_[1] = max_[1] = center.row();
	}

	inline
	void
	kd_hyperrect::extend(const bbox2d* bbox)
	{
	  const point2d& center = bbox->pcenter();
	  const int x = center.col();
	  const int y = center.row();

	  if (x < min_[0])
	    min_[0] = x;
	  if (y < min_[1])
	    min_[1] = y;

	  if (x > max_[0])
	    max_[0] = x;
	  if (y > max_[1])
	    max_[1] = y;
	}

	inline
	int
	kd_hyperrect::distance(const bbox2d* bbox)
	{
	  int distance = 0;
	  const point2d& center = bbox->pcenter();
	  const int x = center.col();
	  const int y = center.row();

	  if (x < min_[0])
	  {
	    const int tmp = min_[0] - x;
	    distance += (tmp * tmp);
	  }
	  else if (x > max_[0])
	  {
	    const int tmp = max_[0] - x;
	    distance += (tmp * tmp);
	  }

	  if (y < min_[1])
	  {
	    const int tmp = min_[1] - y;
	    distance += (tmp * tmp);
	  }
	  else if (y > max_[1])
	  {
	    const int tmp = max_[1] - y;
	    distance += (tmp * tmp);
	  }

	  return distance;
	}

	inline
	int
	kd_hyperrect::vertical_distance(const bbox2d* bbox)
	{
	  int distance = 0;
	  const point2d& center = bbox->pcenter();
	  const int y = center.row();

	  if (y < min_[1])
	  {
	    const int tmp = min_[1] - y;
	    distance += (tmp * tmp);
	  }
	  else if (y > max_[1])
	  {
	    const int tmp = max_[1] - y;
	    distance += (tmp * tmp);
	  }

	  return distance;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_KD_HYPERRECT_HH
