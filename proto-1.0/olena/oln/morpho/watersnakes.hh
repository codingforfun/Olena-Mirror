// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_WATERSNAKES_HH
# define OLENA_MORPHO_WATERSNAKES_HH

# include <string>

# include <oln/basics.hh>
# include <oln/morpho/erosion.hh>
# include <oln/morpho/geodesic_erosion.hh>
# include <oln/morpho/reconstruction.hh>
# include <oln/level/compare.hh>
# include <oln/morpho/gradient_morpho.hh>

# include <oln/basics2d.hh>
# include <oln/level/fill.hh>
# include <ntg/basics.hh>

# include "misc.hh"
# include "Rs.hh"


const float INFTY = 1000000000000.f;
# define WSHED -2
# define MASK_VALUE -1
typedef enum version_t { TARJAN = 0, NORMAL = 1 } version;

namespace oln {

  namespace morpho {

    namespace impl {

      template<class T>
      image2d<float>
      lower_slope(const image2d<T>& input,
		  const window2d& ng)
      {
	image2d<float> LS(input.size());
	oln_iter_type(image2d<T>) p(input);

	for_all(p)
	  {
	    LS[p] = 0;
	    oln_neighb_type(window2d) n(ng, p);
	    for_all(n)
	      {
		if (input.hold(n))
		  if (p != n)
		    if (input[n] < input[p])
		      LS[p] = LS[p] > (float)(input[p] - input[n]) ? LS[p] : (float)(input[p] - input[n]);
	      }
	  }

	return LS;
      }

      template <class T>
      float
      local_grad(const image2d<T>& input,
		 const image2d<float>& LS,
		 const point2d& p,
		 const point2d& n)
      {
	if (input[p] < input[n])
	  return LS[n];
	if (input[p] > input[n])
	  return LS[p];
	return std::min(LS[p], LS[n]);
      }


      float chamfer_distance(const point2d& p, const point2d& p2)
      {
	return sqrt((p.row() - p2.row()) * (p.row() - p2.row()) +
		    (p.col() - p2.col()) * (p.col() - p2.col()));
      }


      template <typename T, typename T2>
      image2d<T>
      reconstruction_by_erosion(const image2d<T>& input,
				const image2d<T2>& marqueur)
      {
	image2d<T> out(input.size());
	image2d<T> tmp;
	bool stop = false;

	oln_iter_type(image2d<T>) p(input);

	for_all(p)
	  out[p] = marqueur[p] == false ? 0 : 255;

	while (stop == false)
	  {
	    stop = true;
	    tmp = morpho::erosion(out, win_c4p());
	    for_all(p)
	      {
		if (out[p] != ntg::max(input[p], tmp[p]))
		  {
		    stop = false;
		    out[p] = ntg::max(input[p], tmp[p]);
		  }
	      }
	  }
	return out;
      }

      bool futur_fwd(const point2d& n, const point2d& p)
      {
	if ((n.row() == p.row() and n.col() == p.col() + 1) or
	    n.row() > p.row())
	  return true;
	return false;
      }


      bool futur_bkd(const point2d& n, const point2d& p)
      {
	if ((n.row() == p.row() and n.col() == p.col() - 1) or
	    n.row() < p.row())
	  return true;
	return false;
      }

      template <typename T>
      const T point_at(const image2d<T>& ima, const point2d& p, int x, int y)
      {
	int x1 = p.row();
	int y1 = p.col();

	if (x1 + x >= 0 and x1 + x < ima.size().nrows() and
	    y1 + y >= 0 and y1 + y < ima.size().ncols())
	  return ima(x1 + x, y1 + y);
	return 0;
      }

      template <typename T>
      int nb_label(const image2d<T>& ima, const point2d& p, int x, int y, const T& l)
      {
	if (point_at(ima, p, x, y) == l)
	  return 1;
	return 0;
      }

      float compute_perimeter(const image2d<int>& label,
			      const point2d& p,
			      unsigned nb,
			      int b)
      {
	float n4 = nb_label(label, p, -1, 0, (int)nb) + nb_label(label, p, 1, 0, (int)nb)
	  + nb_label(label, p, 0, 1, (int)nb) + nb_label(label, p, 0, -1, (int)nb);

	float n8 = nb_label(label, p, -1, -1, (int)nb) + nb_label(label, p, 1, 1, (int)nb)
	  + nb_label(label, p, -1, 1, (int)nb) + nb_label(label, p, 1, -1, (int)nb); // p

	float nk = nb_label(label, p, -2, 1, (int)nb) + nb_label(label, p, -2, -1, (int)nb)
	  + nb_label(label, p, -1, 2, (int)nb) + nb_label(label, p, -1, -2, (int)nb) +
	  nb_label(label, p, 2, -1, (int)nb) + nb_label(label, p, 2, 1, (int)nb)
	  + nb_label(label, p, 1, 2, (int)nb) + nb_label(label, p, 1, -2, (int)nb); // p
	return 2 * b * (0.26 * n4 + 0.19 * n8 + 0.06 * nk);
      }


      template <typename T>
      bool is_a_boundary_point(const image2d<int>& label,
			       const T& p,
			       unsigned& label_b)
      {
	oln_neighb_type(window2d) q(win_c4p(), p);
	for_all (q)
	  if (label.hold(q))
	    if (label[p] != label[q])
	      {
		label_b = label[q];
		return 1;
	      }
	return 0;
      }

      template <typename T>
      bool
      snake_iteration(image2d<T>& label,
		      const image2d<float *>& dist,
		      int b)
      {
	float dist_min = INFTY;
	point2d p_replace;
	unsigned label_replace = 0;

	oln_iter_type(image2d<T>) p(label);
	for_all(p)
	  {
	    unsigned label_b;
	    if (is_a_boundary_point(label, p, label_b))
	      {
		float tmp = - dist[p][label[p] - 1] + compute_perimeter(label, p, label[p], b) +
		  dist[p][label_b - 1] - compute_perimeter(label, p, label_b, b);
		if (dist_min > tmp)
		  {
		    dist_min = tmp;
		    p_replace = p;
		    label_replace = label_b;
		  }
	      }
	  }
	if (dist_min < 0)
	  {
	    label[p_replace] = label_replace;
	    return 1;
	  }
	return 0;
      }


      template <class T, typename T2>
      image2d<float*>
      topographic_distance(const image2d<T>& input,
			   const image2d<float>& LS,
			   const image2d<T2>& marqueur,
			   image2d<int>& label,
			   const window2d& ng,
			   unsigned nb_compo)
      {
	image2d<float*> dist(input.size());
	oln_iter_type(image2d<T>) p(input);
	oln_bkd_iter_type(image2d<T>) p2(input);

	for_all(p)
	  {
	    dist[p] = new float[nb_compo];

	    if (marqueur[p] == false)
	      {
		dist[p][label[p] - 1] = input[p];
		for (unsigned i = 0; i < nb_compo; i++)
		  if ((int)i != label[p] - 1)
		    dist[p][i] = INFTY;
	      }
	    else
	      for (unsigned i = 0; i < nb_compo; i++)
		dist[p][i] = INFTY;
	  }

	int i = 0;

	bool stop = false;
	while (stop == false)
	  {
	    stop = true;
	    // forward
	    for_all(p)
	      {
		oln_neighb_type(window2d) n(ng, p);
		for_all(n)
		  {
		    if (input.hold(n) and futur_fwd(n, p))
		      {
			float grad = local_grad(input, LS, p, n);
			float tmp = chamfer_distance(p, n) * grad;

			for (unsigned k = 0; k < nb_compo; k++)
			  if (dist[p][k] + tmp < dist[n][k])
			    {
			      dist[n][k] = dist[p][k] + tmp;
			      stop = false;
			    }
		      }
		  }
	      }

	    // backward
	    for_all(p2)
	      {
		oln_neighb_type(window2d) n(ng, p2);
		for_all(n)
		  {
		    if (input.hold(n) and futur_bkd(n, p2))
		      {
			float grad = local_grad(input, LS, p2, n);
			float tmp = chamfer_distance(p2, n) * grad;

			for (unsigned k = 0; k < nb_compo; k++)
			  if (dist[p2][k] + tmp < dist[n][k])
			    {
			      dist[n][k] = dist[p2][k] + tmp;
			      stop = false;
			    }
		      }
		  }
	      }
	    if (++i > 20)
	      stop = true;
	  }
	return dist;
      }

      template <typename T>
      std::ostream& operator<<(std::ostream& ostr, const image2d<T>& ima)
      {
	for (int i = 0; i < ima.size().nrows(); i++)
	  {
	    for (int j = 0; j < ima.size().ncols(); j++)
	      ostr << ima(i,j) << "\t";
	    ostr << std::endl;
	  }
	return ostr;
      }

      template <typename T, typename T2>
      void propagate_minima_(const image2d<T>& input,
			     image2d<T2>& mark,
			     const window2d& ng,
			     const point2d& p)
      {
	if (mark[p] == false)
	  {
	    oln_neighb_type(window2d) n(ng, p);
	    for_all(n)
	      {
		if (input.hold(n) and input[p] == input[n] and mark[n] == true)
		  {
		    mark[n] = false;
		    propagate_minima_(input, mark, ng, n);
		  }
	      }
	  }
      }


      template <typename T, typename T2, typename T3>
      void mark_compo_(const image2d<T>& input,
		       image2d<T2>& label,
		       const image2d<T3>& minima,
		       const window2d& win,
		       const point2d& p,
		       unsigned& compo)
      {
	if (minima[p] == false and label[p] == MASK_VALUE)
	  {
	    bool test = false;
	    oln_neighb_type(window2d) q(win, p);
	    for_all (q)
	      if (input.hold(q) and minima[q] == false and label[q] != MASK_VALUE)
		{
		  test = true;
		  label[p] = label[q];
		}
	    if (test == true)
	      {
		for_all (q)
		  if (input.hold(q) and minima[q] == false and label[q] == MASK_VALUE)
		    mark_compo_(input, label, minima, win, q, compo);
	      }
	    if (test == false)
	      {
		label[p] = ++compo;
		for_all (q)
		  if (input.hold(q) and minima[q] == false)
		    mark_compo_(input, label, minima, win, q, compo);
	      }
	  }
      }


      template <typename T, typename T2>
      image2d<int>
      init_watershed_(const image2d<T>& input,
		      const image2d<T2>& minima,
		      const window2d& win,
		      unsigned& cpt)
      {
	image2d<int> output(input.size());

	level::fill(output, MASK_VALUE);

	oln_iter_type(image2d<T>) p(input);
	for_all (p)
	  if (minima[p] == false)
	    mark_compo_(input, output, minima, win, p, cpt);

	return output;
      }

      template<typename T, typename T2>
      std::vector<point2d>
      watershed_line(const image2d<T>& input,
		     const image2d<T>& orig,
		     const image2d<T2>& marqueur,
		     const window2d& ng,
		     int b,
		     version v)
      {
	image2d<int> final(input.size());
	image2d<int> label;
	image2d<T2> mark(input.size());
	oln_iter_type(image2d<T>) p(input);
	image2d<bool> is_wshed_line(input.size());
	std::vector<point2d> res;
	unsigned nb_compo = 0;

	for_all(p)
	  mark[p] = marqueur[p];

	for_all(p)
	  propagate_minima_(input, mark, ng, p);

	image2d<T> recon;

	if (v == NORMAL)
	  recon = reconstruction_by_erosion(input, mark);
	else
	  { // version de tarjan
	    typedef Rs::tarjan< image2d<T2>, image2d<T> > op_type;
	    op_type op(marqueur, input, ng);
	    recon = op.output;
	  }
	image2d<float> LS = lower_slope(recon, ng);

	label = init_watershed_(recon, mark, ng, nb_compo);

	image2d<float*> topo = topographic_distance(recon, LS, mark, label, ng, nb_compo);

	for_all(p)
	  {
	    unsigned mini_label = 1;
	    float mini_topo = topo[p][0];
	    for (unsigned h = 1; h < nb_compo; h++)
	      if (mini_topo > topo[p][h])
		{
		  mini_topo = topo[p][h];
		  mini_label = h + 1;
		}
	    label[p] = mini_label;
	  }

	while (snake_iteration(label, topo, b))
	  ;

	level::fill(final, 0);
	for_all(p)
	  {
	    oln_neighb_type(window2d) q(win_c4p(), p);
	    for_all (q)
	      if (input.hold(q))
		if (label[p] != label[q] and final[q] != WSHED)
		  final[p] = WSHED;
	    if (final[p] == 0)
	      final[p] = orig[p];
	  }

	for_all(p)
	  if (final[p] == WSHED)
	    res.push_back(p);

	return res;
      }



      template<typename T, typename T2>
      std::vector<point2d>
      watersnakes_(const image2d<T>& input,
		   const image2d<T2>& marqueur,
		   const window2d& ng,
		   int b,
		   version v)
      {
	image2d<T> grad(input.size());

	grad = morpho::gradient_morpho(input, ng);
 	return watershed_line(grad, input, marqueur, ng, b, v);
      }


    } //end of oln::morpho::impl


    template<typename T, typename T2>
    std::vector<point2d>
    watersnakes(const image2d<T>& input,
		const image2d<T2>& marqueur,
		const window2d& ng,
		int b,
		version v = TARJAN)
    {
      return impl::watersnakes_(input, marqueur, ng, b, v);
    }

  } // end of oln::morpho

} // end of oln

#endif // ! OLENA_MORPHO_WATERSNAKES_HH
