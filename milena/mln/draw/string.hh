// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_DRAW_STRING_HH
# define MLN_DRAW_STRING_HH

/*! \file
 *
 * \brief Draw a string in an image.
 *
 *
 */

# include <mln/core/concept/image.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/data/paste.hh>
# include <mln/draw/line.hh>
# include <mln/draw/box.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/core/var.hh>

namespace mln
{

  namespace draw
  {
    /*! Draw a char at value \p v in image \p ima
     *
     * \param[in,out] ima The image to be drawn.
     * \param[in] c the char draw.
     * \param[in] _b the bounding box of the character.
     * \param[in] v The value to assign to all drawn pixels.
     *
     * \pre \p ima has to be initialized.
     * \pre \p ima has \p beg.
     * \pre \p ima has \p end.
     *
     */
    template <typename I, typename B>
    void character(Image<I>& ima,
	     const char c,
	     const Box<B>& b,
	     const mln_value(I)& v);
	     
	     
    /*! Draw a string at value \p v in image \p ima
     *
     * \param[in,out] ima The image to be drawn.
     * \param[in] s the string draw.
     * \param[in] _b the bounding box of one character.
     * \param[in] v The value to assign to all drawn pixels.
     *
     * \pre \p ima has to be initialized.
     * \pre \p ima has \p beg.
     * \pre \p ima has \p end.
     *
     */
    template <typename I, typename B>
    void string(Image<I>& ima,
	     const char* s,
	     const Box<B>& b,
	     const mln_value(I)& v);


# ifndef MLN_INCLUDE_ONLY

 

    namespace internal
    {
      template <typename I, typename B>
      inline
      void string_dot(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pcenter());
	      mln_VAR(cp_max, b.pcenter());
	      cp_min[1] -= 1;
	      cp_min[0] -= 1;
	       B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
      template <typename I, typename B>
      inline
      void string_box(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	       B b = exact(_b);
	      mln_VAR(cp_min, b.pmin());
	      mln_VAR(cp_max, b.pmax());
	      cp_min[0] -= 1; cp_min[1] -= 1;
	      cp_max[0] -= 1; cp_max[1] -= 1;
	       B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	       draw::box(ima, b, v);
	     }

      template <typename I, typename B>
      inline
      void string_pipe(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pcenter());
	      mln_VAR(cp_max, b.pcenter());
	      cp_min[1] -= 1; cp_min[0] = b.pmin()[0];
	      cp_min[0] -= 1; cp_max[0] = b.pmax()[0];
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
	     
      template <typename I, typename B>
      inline
      void string_pipe_down(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pcenter());
	      mln_VAR(cp_max, b.pcenter());
	      cp_min[1] -= 1; cp_min[0] = b.pmin()[0];
	      cp_min[0] -= 1; cp_max[0] = b.pmax()[0];
	      cp_min[0] = cp_max[0] - b.len(0) / 2;
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
      template <typename I, typename B>
      inline
      void string_pipe_top(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pcenter());
	      mln_VAR(cp_max, b.pcenter());
	      cp_min[1] -= 1; cp_min[0] = b.pmin()[0];
	      cp_min[0] -= 1; cp_max[0] = b.pmax()[0];
	      cp_max[0] = cp_min[0] + b.len(0) / 2;
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
      template <typename I, typename B>
      inline
      void string_right_pipe(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmax());
	      mln_VAR(cp_max, b.pmax());
	      cp_min[1] -= 1; cp_min[0] = b.pmin()[0];
	      cp_min[0] -= 1; cp_max[0] = b.pmax()[0];
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
	     
	     
      template <typename I, typename B>
      inline
      void string_right_pipe_top(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmax());
	      mln_VAR(cp_max, b.pmax());
	      cp_min[1] -= 1; cp_min[0] = b.pmin()[0];
	      cp_min[0] -= 1; cp_max[0] = b.pmax()[0];
	      cp_max[0] = cp_min[0] + b.len(0) / 2;
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
	     
      template <typename I, typename B>
      inline
      void string_right_pipe_down(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmax());
	      mln_VAR(cp_max, b.pmax());
	      cp_min[1] -= 1; cp_min[0] = b.pmin()[0];
	      cp_min[0] -= 1; cp_max[0] = b.pmax()[0];
	      cp_min[0] = cp_max[0] - b.len(0) / 2;
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
	     
      template <typename I, typename B>
      inline
      void string_left_pipe(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmin());
	      mln_VAR(cp_max, b.pmin());
	      cp_min[1] -= 1; cp_min[0] = b.pmin()[0];
	      cp_min[0] -= 1; cp_max[0] = b.pmax()[0];
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }

      template <typename I, typename B>
      inline
      void string_left_pipe_top(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmin());
	      mln_VAR(cp_max, b.pmin());
	      cp_min[1] -= 1; cp_min[0] = b.pmin()[0];
	      cp_min[0] -= 1; cp_max[0] = b.pmax()[0];
	      cp_max[0] = cp_min[0] + b.len(0) / 2;
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }

      template <typename I, typename B>
      inline
      void string_down_slash(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pcenter());
	      mln_VAR(cp_max, b.pmax());
	      cp_min[1] = b.pmin()[1];
	      draw::line(ima,cp_min, cp_max, v );
	      cp_min[0] -= 1; cp_max[0] -= 1;
	      draw::line(ima,cp_min, cp_max, v );
	     }
	     
	     
      template <typename I, typename B>
      inline
      void string_top_slash(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pcenter());
	      mln_VAR(cp_max, b.pmin());
	      cp_min[1] = b.pmin()[1]; cp_max[1] = b.pmax()[1];
	      draw::line(ima,cp_min, cp_max, v );
	      cp_min[0] -= 1; cp_max[0] -= 1;
	      draw::line(ima,cp_min, cp_max, v );
	     }
	     
	     
      template <typename I, typename B>
      inline
      void string_left_slash(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmax());
	      mln_VAR(cp_max, b.pmin());
	      draw::line(ima,cp_min, cp_max, v );
	      cp_min[0] -= 1; cp_max[0] -= 1;
	      draw::line(ima,cp_min, cp_max, v );
	     }
	     
      template <typename I, typename B>
      inline
      void string_right_slash(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmin());
	      mln_VAR(cp_max, b.pmax());
	      cp_min[1] = b.pmax()[1]; cp_max[1] = b.pmin()[1];
	      draw::line(ima,cp_min, cp_max, v );
	      cp_min[0] -= 1; cp_max[0] -= 1;
	      draw::line(ima,cp_min, cp_max, v );
	     }
	     
      template <typename I, typename B>
      inline
      void string_left_pipe_down(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmin());
	      mln_VAR(cp_max, b.pmin());
	      cp_min[1] -= 1; cp_min[0] = b.pmin()[0];
	      cp_min[0] -= 1; cp_max[0] = b.pmax()[0];
	      cp_min[0] = cp_max[0] - b.len(0) / 2;
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }

      template <typename I, typename B>
      inline
      void string_minus(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pcenter());
	      mln_VAR(cp_max, b.pcenter());
	      cp_min[1] -= 1; cp_min[1] = b.pmin()[1];
	      cp_min[0] -= 1; cp_max[1] = b.pmax()[1];
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
	     
      template <typename I, typename B>
      inline
      void string_underscore(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmax());
	      mln_VAR(cp_max, b.pmax());
	      cp_min[1] -= 1; cp_min[1] = b.pmin()[1];
	      cp_min[0] -= 1; cp_max[1] = b.pmax()[1];
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
	     
      template <typename I, typename B>
      inline
      void string_topscore(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	      B b = exact(_b);
	      mln_VAR(cp_min, b.pmin());
	      mln_VAR(cp_max, b.pmin());
	      cp_min[1] -= 1; cp_min[1] = b.pmin()[1];
	      cp_min[0] -= 1; cp_max[1] = b.pmax()[1];
	      B cb(cp_min, cp_max);
	       draw::box(ima, cb, v);
	     }
	
      template <typename I, typename B>
      inline
      void string_topbox(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	       string_topscore(ima, _b, v);
	       string_minus(ima, _b, v);
	       string_left_pipe_top(ima, _b, v);
	       string_right_pipe_top(ima, _b, v);
	     }
	     
      template <typename I, typename B>
      inline
      void string_downbox(Image<I>& ima,
	     const Box<B>& _b,
	     const mln_value(I)& v)
	     {
	       string_underscore(ima, _b, v);
	       string_minus(ima, _b, v);
	       string_left_pipe_down(ima, _b, v);
	       string_right_pipe_down(ima, _b, v);
	     }
    } // end of namespace mln::draw::internal



    template <typename I, typename B>
    inline
    void character(Image<I>& ima,
	     const char c,
	     const Box<B>& _b,
	     const mln_value(I)& v)
    {
	   B b = exact(_b);
	  switch(c)
	  {
	    case 'A':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      mln::draw::internal::string_topbox(ima, b, v);
	      break;
	    case 'a':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      mln::draw::internal::string_topbox(ima, b, v);
	      break;
	      
	    case 'B':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_downbox(ima, b, v);
	      break;
	    case 'b':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_downbox(ima, b, v);
	      break;
	    case 'C':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      break;
	    case 'c':
	      mln::draw::internal::string_left_pipe_down(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      break;
	    case 'D':
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      mln::draw::internal::string_downbox(ima, b, v);
	      break;
	    case 'd':
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      mln::draw::internal::string_downbox(ima, b, v);
	      break;
	    
	    case 'E':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;
	    case 'e':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;

	    case 'F':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;
	    case 'f':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;

	      
	    case 'G':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_downbox(ima, b, v);
	      mln::draw::internal::string_left_pipe_top(ima, b, v);
	      break;
	    case 'g':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_downbox(ima, b, v);
	      mln::draw::internal::string_left_pipe_top(ima, b, v);
	      break;
	      
	    case 'H':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      break;
	    case 'h':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_right_pipe_down(ima, b, v);
	      break;
	      
	    case 'I':
	      mln::draw::internal::string_pipe(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;
	    case 'i':
	      mln::draw::internal::string_pipe_down(ima, b, v);
	      break;

	      case 'J':
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      mln::draw::internal::string_down_slash(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;
	    case 'j':
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      mln::draw::internal::string_down_slash(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;
	      
	      	      
	    case 'K':
	      mln::draw::internal::string_top_slash(ima, b, v);
	      mln::draw::internal::string_down_slash(ima, b, v);
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      break;
	    case 'k':
	      mln::draw::internal::string_down_slash(ima, b, v);
	      mln::draw::internal::string_top_slash(ima, b, v);
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      break;
	      
	    case 'L':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      break;
	    case 'l':
	      mln::draw::internal::string_left_pipe(ima, b, v); break;
	
	    case 'M':
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_pipe_top(ima, b, v);
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      break;
	    case 'm':
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_left_pipe_down(ima, b, v);
	      mln::draw::internal::string_pipe_down(ima, b, v);
	      mln::draw::internal::string_right_pipe_down(ima, b, v);
	      break;
	      
	      
	    case 'N':
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      break;
	    case 'n':
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_left_pipe_down(ima, b, v);
	      mln::draw::internal::string_right_pipe_down(ima, b, v);
	      break;
	      
	    case 'O':
	      mln::draw::internal::string_box(ima, b, v); break;
	    case 'o':
	      mln::draw::internal::string_downbox(ima, b, v); break;
	      
	    case 'P':
	      	      mln::draw::internal::string_topbox(ima, b, v);
		      mln::draw::internal::string_left_pipe(ima, b, v);
		      break;
	    case 'p':
	      	      mln::draw::internal::string_topbox(ima, b, v);
		      mln::draw::internal::string_left_pipe(ima, b, v);
		      break;
		      
	    case 'Q':
	      	      mln::draw::internal::string_topbox(ima, b, v);
		      mln::draw::internal::string_right_pipe(ima, b, v);
		      break;
	    case 'q':
	      	      mln::draw::internal::string_topbox(ima, b, v);
		      mln::draw::internal::string_right_pipe(ima, b, v);
		      break;
		      
	    case 'R':
		      mln::draw::internal::string_down_slash(ima, b, v);
		       mln::draw::internal::string_left_pipe(ima, b, v);
		       mln::draw::internal::string_topbox(ima, b,v);
		       break;
	    case 'r':
		      mln::draw::internal::string_down_slash(ima, b, v);
		       mln::draw::internal::string_left_pipe(ima, b, v);
		       mln::draw::internal::string_topbox(ima, b,v);
		       break;		      
	    case 'S':
		      mln::draw::internal::string_underscore(ima, b, v);
		      mln::draw::internal::string_topscore(ima, b, v);
		      mln::draw::internal::string_minus(ima, b, v);
		      mln::draw::internal::string_right_pipe_down(ima, b, v);
		      mln::draw::internal::string_left_pipe_top(ima, b, v);
		      break;
		      
	    case 's':
		      mln::draw::internal::string_underscore(ima, b, v);
		      mln::draw::internal::string_topscore(ima, b, v);
		      mln::draw::internal::string_minus(ima, b, v);
		      mln::draw::internal::string_right_pipe_down(ima, b, v);
		      mln::draw::internal::string_left_pipe_top(ima, b, v);
		      break;
		      
	    case 'T':
	      	      mln::draw::internal::string_pipe(ima, b, v);
		      mln::draw::internal::string_topscore(ima, b, v);
		      break;
	    case 't':
	      	      mln::draw::internal::string_pipe(ima, b, v);
		      mln::draw::internal::string_topscore(ima, b, v);
		      break;
		      
	    case 'U':
	      	      mln::draw::internal::string_underscore(ima, b, v);
		      mln::draw::internal::string_left_pipe(ima, b, v);
		      mln::draw::internal::string_right_pipe(ima, b, v);
		      break;
	    case 'u':
	      	      mln::draw::internal::string_underscore(ima, b, v);
		      mln::draw::internal::string_left_pipe_down(ima, b, v);
		      mln::draw::internal::string_right_pipe_down(ima, b, v);
		      break;
	    case 'V':
		      mln::draw::internal::string_left_pipe_top(ima, b, v);
		      mln::draw::internal::string_down_slash(ima, b, v);
		      mln::draw::internal::string_right_pipe(ima, b, v);
		      break;
	    case 'v':
		      mln::draw::internal::string_left_pipe_down(ima, b, v);
		      mln::draw::internal::string_down_slash(ima, b, v);
		      mln::draw::internal::string_right_pipe_down(ima, b, v);
		      break;
		      
	    case 'W':
	      	      mln::draw::internal::string_underscore(ima, b, v);
		      mln::draw::internal::string_left_pipe(ima, b, v);
		      mln::draw::internal::string_pipe_down(ima, b, v);
		      mln::draw::internal::string_right_pipe(ima, b, v);
		      break;
	    case 'w':
	      	      mln::draw::internal::string_underscore(ima, b, v);
		      mln::draw::internal::string_left_pipe_down(ima, b, v);
		      mln::draw::internal::string_right_pipe_down(ima, b, v);
		      mln::draw::internal::string_pipe_down(ima, b, v);
		      break;
		      
	    case 'X':
		      mln::draw::internal::string_left_slash(ima, b, v);
		      mln::draw::internal::string_right_slash(ima, b, v);
		      break;
	    case 'x':
		      mln::draw::internal::string_left_slash(ima, b, v);
		      mln::draw::internal::string_right_slash(ima, b, v);
		      break;
		      
	    case 'Y':
		      mln::draw::internal::string_pipe_top(ima, b, v);
		      mln::draw::internal::string_right_slash(ima, b, v);
		      break;
	    case 'y':
		      mln::draw::internal::string_pipe_top(ima, b, v);
		      mln::draw::internal::string_right_slash(ima, b, v);
		      break;
		      
	    case 'Z':
	      	      mln::draw::internal::string_topscore(ima, b, v);
		      mln::draw::internal::string_right_slash(ima, b, v);
		      mln::draw::internal::string_underscore(ima, b, v);
		      break;
	    case 'z':
	      	      mln::draw::internal::string_minus(ima, b, v);
		      mln::draw::internal::string_underscore(ima, b, v);
		      mln::draw::internal::string_down_slash(ima, b, v);
		      break;

	      
	    case '0':
	      mln::draw::internal::string_box(ima, b, v);
	      mln::draw::internal::string_dot(ima, b, v);
	      break;
	    case '1':
	      mln::draw::internal::string_pipe(ima, b, v);break;
	    case '2':
	      mln::draw::internal::string_right_pipe_top(ima, b, v);
	      mln::draw::internal::string_left_pipe_down(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;
	    case '3':
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;
	    case '4':
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_left_pipe_top(ima, b, v);
	      break;
	    case '5':
	      mln::draw::internal::string_underscore(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_right_pipe_down(ima, b, v);
	      mln::draw::internal::string_left_pipe_top(ima, b, v);
	      break;
	    case '6':
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_downbox(ima, b, v);
	      mln::draw::internal::string_left_pipe_top(ima, b, v);
	      break;
	      
	    case '7':
	      mln::draw::internal::string_right_pipe(ima, b, v); 
	      mln::draw::internal::string_topscore(ima, b, v);
	      break;
	    case '8':
	      mln::draw::internal::string_box(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      break;
	    case '9':
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      mln::draw::internal::string_topbox(ima, b, v);
	      break;
	    case '.':
	      mln::draw::internal::string_dot(ima, b, v);break;
	    case ' ':
	      break;
	    case '-':
	      mln::draw::internal::string_minus(ima, b, v);break;
	      break;
	    case '_':
	      mln::draw::internal::string_underscore(ima, b, v);
	      break;
	    case '=':
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      break;
	    case '$':
	      mln::draw::internal::string_right_pipe_top(ima, b, v);
	      mln::draw::internal::string_left_pipe_down(ima, b, v);
	      mln::draw::internal::string_minus(ima, b, v);
	      mln::draw::internal::string_underscore(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_pipe(ima, b, v);
	      break;
	      
	    case '(':
	      mln::draw::internal::string_underscore(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_left_pipe(ima, b, v);
	      break;
	      
	     case ')':
	      mln::draw::internal::string_underscore(ima, b, v);
	      mln::draw::internal::string_topscore(ima, b, v);
	      mln::draw::internal::string_right_pipe(ima, b, v);
	      break;
	      
	     case '|':
	      mln::draw::internal::string_pipe(ima, b, v);
	      break;
	     case '/':
	      mln::draw::internal::string_right_slash(ima, b, v);
	      break;
	     case '\\':
	      mln::draw::internal::string_left_slash(ima, b, v);
	      break;
	  }
    }


   template <typename I, typename B>
    inline
    void string(Image<I>& ima,
	     const char* s,
	     const Box<B>& _b,
	     const mln_value(I)& v)
    {
      B b = exact(_b);
      const char* ptr = &(s[0]);
      int Decal = 0;
      int DecalY = 0;
      int DecalX = 0;
      while(ptr[Decal])
      {
	if(ptr[Decal] == '\n'){DecalY++; DecalX = 0; ptr++; continue;}
	if(ptr[Decal] == '\t'){ DecalX += 5; ptr++; continue;}
	mln_VAR(cp_min, b.pmin());
	mln_VAR(cp_max, b.pmax());
	cp_min[1] += (b.len(1) + (b.len(1) / 2))  * DecalX;
	cp_max[1] += (b.len(1) + (b.len(1) / 2)) * DecalX;

	cp_min[0] += (b.len(0) + (b.len(1) / 2))  * DecalY;
	cp_max[0] += (b.len(0) + (b.len(1) / 2)) * DecalY;
	
	B cb(cp_min, cp_max);
	character(ima, ptr[Decal], cb, v);
	Decal++;
	DecalX++;
      }
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::draw

} // end of namespace mln


#endif // ! MLN_DRAW_BOX_HH
