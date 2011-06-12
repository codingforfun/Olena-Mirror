// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_INVERSE_VIDEO_CHOOSE_HH
# define SCRIBO_INVERSE_VIDEO_CHOOSE_HH

/// \file
///
/// \brief	Choose beetwen two lines.

# include <scribo/core/line_set.hh>
# include <scribo/inverse_video/nb_comps.hh>
# include <scribo/inverse_video/ratio_area.hh>
# include <iostream>

namespace scribo
{

  namespace inverse_video
  {
    /// Choose the most meaningful line between two.
    /*!
    ** \param[in]     input		The input image.
    ** \param[in]     line		The first line.
    ** \param[in]     line_inverse	The second line
    **
    ** \return		void
    */
    template <typename L, typename I>
    static inline
    void
    choose (mln::image2d<I>& input,
	    scribo::line_info<L>& line,
	    scribo::line_info<L>& line_inverse);

# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {
      template <typename I, typename L>
      static inline
      void
      handle_score (double score,
		    I& image,
		    scribo::line_info<L>& line,
		    scribo::line_info<L>& line_inverse)
      {
	if (score > 0.5)
	  {
	    mln::draw::box(image, line.bbox(), mln::literal::green);
	    mln::draw::box(image, line_inverse.bbox(), mln::literal::red);
	    line_inverse.update_tag(scribo::line::Ignored);
	    line_inverse.set_hidden(true);
	    std::cout << "normal choosed" << std::endl;
	  }
	else
	  {
	    mln::draw::box(image, line_inverse.bbox(), mln::literal::green);
	    mln::draw::box(image, line.bbox(), mln::literal::red);
	    line.update_tag(scribo::line::Ignored);
	    line.set_hidden(true);
	    std::cout << "inverse choosed" << std::endl;
	  }
      }
    } // end of namespace scribo::text::internal

    template <typename L, typename I>
    static inline
    void
    choose (mln::image2d<I>& input,
	    scribo::line_info<L>& line,
	    scribo::line_info<L>& line_inverse)
    {
      mln::trace::entering("scribo::inverse_video::choose");

      double score = 0;

      double score_nb_comps = scribo::inverse_video::nb_comps(line, line_inverse);
      double score_ratio_area = scribo::inverse_video::ratio_area(line, line_inverse);

      score =
	score_nb_comps * 0.4 +
	score_ratio_area * 0.6;

      scribo::inverse_video::internal::handle_score (score, input, line, line_inverse);

      mln::trace::exiting("scribo::inverse_video::choose");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::inverse_video

} // end of namespace scribo

#endif // ! SCRIBO_INVERSE_VIDEO_CHOOSE_HH
