// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_INVERSE_VIDEO_HANDLE_COLLISIONS_HH
# define SCRIBO_INVERSE_VIDEO_HANDLE_COLLISIONS_HH

# include <set>
# include <vector>
# include <iostream>

# include <mln/data/fill.hh>
# include <mln/core/image/image2d.hh>
# include <mln/util/array.hh>
# include <scribo/core/line_set.hh>
# include <mln/draw/box.hh>
# include <mln/literal/colors.hh>

namespace scribo
{

  namespace inverse_video
  {

    template <typename I,
	      typename L>
    void
    handle_collisions(image2d<I>& input,
		      scribo::line_set<L>& ls1,
		      scribo::line_set<L>& ls2,
		      void (*choose)(image2d<I>&,
				     scribo::line_info<L>&,
				     scribo::line_info<L>&));
# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {
      template <typename L>
      struct order_2_lines_id
      {
	order_2_lines_id(const scribo::line_set<L>& ls1,
			 const scribo::line_set<L>& ls2)
	  : ls1_(ls1),
	    ls2_(ls2)
	{
	  ls1_size_ = ls1_.nelements();
	}

	bool operator()(const scribo::line_id_t& a,
			const scribo::line_id_t& b)
	{
	  const scribo::line_info<L>& la =
	    (a > ls1_size_ ? ls2_(a - ls1_size_ + 1) : ls1_(a));
	  const scribo::line_info<L>& lb =
	    (b > ls1_size_ ? ls2_(b - ls1_size_ + 1) : ls1_(b));

	  return la.bbox().nsites() < lb.bbox().nsites();
	}

	unsigned ls1_size_;
	const scribo::line_set<L>& ls1_;
	const scribo::line_set<L>& ls2_;
      };
    } // end of namespace scribo::text::internal

    // Facade
    template <typename I, typename L>
    void
    handle_collisions(image2d<I>& input,
		      scribo::line_set<L>& ls1,
		      scribo::line_set<L>& ls2,
		      void (*choose)(image2d<I>&,
				     scribo::line_info<L>&,
				     scribo::line_info<L>&))
    {
      mln::trace::entering("scribo::inverse_video::handle_collisions");

      mln::image2d<unsigned> billboard(input.domain());
      mln::data::fill(billboard, 0);

      internal::order_2_lines_id<L> func(ls1, ls2);

      std::vector<scribo::line_id_t> v;

      unsigned ls1_size = ls1.nelements();
      unsigned n = ls1_size + ls2.nelements();
      v.reserve(n);
      
      for (unsigned i = 1; i < n; ++i)
	v.push_back(i);

      std::sort(v.begin(), v.end(), func);

      for (int i = v.size() - 1; i > 0; --i)
	{
	  bool inverse_video = (v[i] > ls1_size);
	  
	  scribo::line_set<L>& ls = (inverse_video) ? ls2 : ls1;
	  scribo::line_id_t l = (inverse_video) ?
	    (scribo::line_id_t) (v[i] - ls1_size + 1)
	    : v[i];

	  if (!scribo::text::internal::looks_like_a_text_line(ls(l))
	      || !ls(l).is_valid() || ls(l).is_hidden())
	    continue;

	  unsigned tl, tr, bl, br;

	  box2d b = ls(l).bbox();

	  tl = billboard(b.pmin());
	  tr = billboard.at_(b.pmin().row(), b.pmax().col());
	  bl = billboard.at_(b.pmax().row(), b.pmin().col());
	  br = billboard(b.pmax());

	  std::set<unsigned> labels;
	  labels.insert(tl);
	  labels.insert(tr);
	  labels.insert(bl);
	  labels.insert(br);

	  for (std::set<unsigned>::const_iterator it = labels.begin();
	       it != labels.end();
	       ++it)
	  {
	    if (*it == 0)
	      continue;

	    bool inverse_video_tested = (*it > ls1_size);

	    if (inverse_video_tested == inverse_video)
	      break;

	    scribo::line_info<L>& l_tested =
	      (inverse_video_tested ? ls2(*it - ls1_size + 1) : ls1(*it));

	    if (!scribo::text::internal::looks_like_a_text_line(l_tested)
		|| !l_tested.is_valid() || l_tested.is_hidden())
	      continue;

	    choose(input, ls(l), l_tested);

	    if (ls(l).is_hidden())
	      break;
	  }
	  
	  if (!ls(l).is_hidden())
	    scribo::text::internal::draw_box(billboard, b, v[i]);
	}

      mln::trace::exiting("scribo::inverse_video::handle_collisions");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::inverse_video

} // end of namespace scribo

#endif // ! SCRIBO_INVERSE_VIDEO_HANDLE_COLLISIONS_HH
