// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_MORPHO_WATERSHED_HH
# define OLENA_MORPHO_WATERSHED_HH

# include <vector>

# include <mlc/cmp.hh>

# include <oln/utils/record.hh>
# include <oln/basics.hh>
# include <oln/morpho/local.hh>
# include <oln/core/2d/window2d.hh>
# include <oln/utils/clone.hh>
# include <oln/level/fill.hh>

#include <oln/io/write_image.hh>

# define WSHED 255
# define MASK_VALUE -1

namespace oln {

  namespace morpho {

    // Watershed by hill climbing
    template<typename I, typename W>
    oln_type_of(I, concrete) watershed(const abstract::image<I>& input,
				       const abstract::window<W>& win);


    namespace impl {


      template <typename I, typename II, typename W>
      void propagate_minima_(const abstract::image<I>& input,
			     abstract::image<II>& minima,
			     const abstract::window<W>& win,
			     const oln_type_of(I, point)& p)
      {
	if (minima[p] == false)
	  {
	    oln_wn_type_of(W, fwd_iter) q(win);
	    for_all_q_of_p (q, p)
	      if (input.hold(q) and input[p] == input[q] and minima[q] == true)
		{
		  minima[q] = false;
		  propagate_minima_(input, minima, win, q);
		}
	  }
      }


      template <typename I, typename W>
      oln_ch_concrete_type(I, ntg::bin)
      set_minima_zero_(abstract::image<I>& input,
		       const abstract::window<W>& win)
      {
	oln_ch_concrete_type(I, ntg::bin) output(input.size(), "output");
	bool is_min = true;

	oln_type_of(I, fwd_piter) p(input.size());
	for_all_p (p)
	  {
	    is_min = true;
	    oln_wn_type_of(W, fwd_iter) q(win);
	    for_all_q_of_p (q, p)
	      if (input.hold(q) and input[p] > input[q])
		is_min = false;
	    output[p] = is_min;
	  }
	for_all_p (p)
	  if (output[p] == false)
	    propagate_minima_(input, output, win, p);
	for_all_p (p)
	  if (output[p] == true)
	    input[p] = 0;
	return output;
      }


      template <typename I, typename II, typename III, typename W>
      void mark_compo_(const abstract::image<I>& input,
		       abstract::image<II>& label,
		       const abstract::image<III>& minima,
		       const abstract::window<W>& win,
		       const oln_type_of(I, point)& p,
		       unsigned& compo)
      {
	if (minima[p] == true and label[p] == MASK_VALUE)
	  {
	    bool test = false;
	    oln_wn_type_of(W, fwd_iter) q(win);
	    for_all_q_of_p (q, p)
	      if (input.hold(q) and minima[q] == true and label[q] != MASK_VALUE)
		{
		  test = true;
		  label[p] = label[q];
		}
	    if (test == true)
	      {
		for_all_q_of_p (q, p)
		  if (input.hold(q) and minima[q] == true and label[q] == MASK_VALUE)
		    mark_compo_(input, label, minima, win, q, compo);

	      }
	    if (test == false)
	      {
		label[p] = ++compo;
		for_all_q_of_p (q, p)
		  if (input.hold(q) and minima[q] == true)
		    mark_compo_(input, label, minima, win, q, compo);
	      }
	  }
      }



      template <typename I, typename II, typename W>
      oln_ch_concrete_type(I, int)
	init_watershed_(const abstract::image<I>& input,
			const abstract::image<II>& minima,
			const abstract::window<W>& win)
      {
	oln_ch_concrete_type(I, int) output(input.size(), "mask");

	level::fill(output, MASK_VALUE);
	unsigned cpt = 0;
	unsigned tmp;

	oln_type_of(I, fwd_piter) p(input.size());
	for_all_p (p)
	  {
	    if (minima[p] == true)
	      {
		mark_compo_(input, output, minima, win, p, cpt);
	      }
	  }
	return output;
      }

      template <typename I, typename II, typename W>
      std::vector<std::vector<oln_type_of(I, point)> >
      compute_list_(const abstract::image<I>& input,
		    const abstract::image<II>& minima,
		    const abstract::window<W>& win)
      {
	typedef oln_type_of(I, point) point_type;
	std::vector<std::vector<point_type> > l(256);
	oln_type_of(I, fwd_piter) p(input.size());
	bool add = false;

	for_all_p (p)
	  {
	    add = false;
	    oln_wn_type_of(W, fwd_iter) q(win);
	    for_all_q_of_p (q, p)
	      if (input.hold(q))
		if (input[q] != input[p])
		  add = true;
	    if (add == true)
	      l[input[p]].push_back(p);
	  }
	return l;
      }

      template <typename I, typename W>
      oln_ch_concrete_type(I, std::vector<oln_type_of(I, point)>)
	inverse_LS_(const abstract::image<I>& input,
		    const abstract::window<W>& win)
      {
	oln_ch_concrete_type(I, std::vector<oln_type_of(I, point)>) output(input.size(), "inverse_ls");


	oln_type_of(I, fwd_piter) p(input.size());
	for_all_p (p)
	  {
	    oln_type_of(I, point) pt = p;
	    oln_wn_type_of(W, fwd_iter) q(win);
	    int max = 0;
	    for_all_q_of_p (q, p)
	      if (input.hold(q))
		max = (int)(input[q] - input[p]) > max ? (int)(input[q] - input[p]) : max;
	    if (max)
	      for_all_q_of_p (q, p)
		if (input.hold(q) and (int)(input[q] - input[p]) == max)
		  output.at(p).push_back(q);
	  }
	return output;
      }

      // Fwd decl of watershed facade.

      template<typename I, typename W>
      oln_type_of(I, concrete)
      watershed_(const abstract::image<I>& input,
		 const abstract::window<W>& win)
      {
	oln_type_of(I, concrete) output(input.size(), "out");
	oln_type_of(I, concrete) in;
	oln_ch_concrete_type(I, int) label;
	oln_ch_concrete_type(I, ntg::bin) mask(input.size(), "mask");
	oln_ch_concrete_type(I, std::vector<oln_type_of(I, point)>) inverse_ls;
	std::vector<std::vector<oln_type_of(I, point)> > S;

	in = clone(input);
	mask = set_minima_zero_(in, win);
	label = init_watershed_(in, mask, win);
	S = compute_list_(in, mask, win);
	inverse_ls = inverse_LS_(in, win);

	typename std::vector<oln_type_of(I, point)>::const_iterator it;

	for (int i = 0; i < 256; i++)
	  for (it = S[i].begin(); it != S[i].end(); it++)
	    {
	      typename std::vector<oln_type_of(I, point)>::const_iterator it2;
	      for (it2 = inverse_ls.at(*it).begin(); it2 != inverse_ls.at(*it).end(); it2++)
		{

		  bool stop = false;
		  typename std::vector<oln_type_of(I, point)>::const_iterator it3;
		  for (int j = i + 1; j < 256 and stop == false; j++)
		    for (it3 = S[j].begin(); it3 != S[j].end() and stop == false; it3++)
		      if (*it3 == *it2)
			stop = true;

		  if (stop == true)
		    {
		      if (label[*it2] == MASK_VALUE)
			label[*it2] = label[*it];
		      else
			if (label[*it2] != WSHED and label[*it2] != label[*it])
			  label[*it2] = WSHED;
		    }
		}
	    }

	oln_type_of(I, fwd_piter) p(input.size());
	for_all_p (p)
	  output[p] = label[p] == MASK_VALUE ? 0 : label[p];

	return output;
      }


    } // end of namespace oln::morpho::impl




    /// Generic watershed (facade).

    template<typename I, typename W>
    oln_type_of(I, concrete) watershed(const abstract::image<I>& input,
				       const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
      entering("morpho::watershed");

      oln_type_of(I, concrete) output("output");
      output = impl::watershed_(input.exact(), win.exact());

      exiting("morpho::watershed");
      return output;
    }


  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_WATERSHED_HH
