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
/// \brief Routines used in Voronoi algorithm.


#ifndef SCRIBO_LAYOUT_INTERNAL_VORONOI_SORT_HH
# define SCRIBO_LAYOUT_INTERNAL_VORONOI_SORT_HH

# include <mln/core/image/image2d.hh>
# include <mln/value/int_u16.hh>
# include <scribo/layout/internal/voronoi/flooding.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace voronoi
      {
	using namespace mln;

	void reorder_bbox_list(std::vector<outline_bbox>& bbox_list,
			       const image2d<value::int_u16>& ws);

# ifndef MLN_INCLUDE_ONLY

	inline
	void reorder_bbox_list(std::vector<outline_bbox>& bbox_list,
			       const image2d<value::int_u16>& ws)
	{
	  const unsigned nelements = bbox_list.size();
	  std::vector<outline_bbox> list(bbox_list.size());

	  for (unsigned i = 0; i < nelements; ++i)
	  {
	    // outline_bbox tmp = bbox_list[i];
	    point2d center = bbox_list[i].bbox.pcenter();
	    int index = ws(center) - 1;

	    if (index < 0)
	      continue;

	    list[i] = bbox_list[index];

	    // bbox_list[i] = bbox_list[index];
	    // bbox_list[index] = tmp;
	  }

	  bbox_list.clear();
	  bbox_list = list;
	}


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::voronoi

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_VORONOI_SORT_HH





