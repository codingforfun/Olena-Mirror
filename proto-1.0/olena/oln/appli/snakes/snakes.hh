// Copyright (C) 2001-2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_SNAKES_HH
# define OLENA_SNAKES_HH

# include <mlc/is_a.hh>
# include <oln/basics2d.hh>
# include <ntg/all.hh>
# include <oln/morpho/gradient_morpho.hh>
# include <ntg/int.hh>
# include <vector>

#define sqr(u) ((u)*(u))
typedef enum SNAKE_TYPE { FERME = 0, OUVERT = 1 } snake_type;
typedef enum FORCE_TYPE { GRADIENT = 0, VARIANCE = 1 } force_type;
const float PI = 3.14159265;


namespace oln {

  namespace snakes {

    namespace impl {

      inline
      float euclidian_distance(int x1, int x2, int y1, int y2) // sqr
      {
	return (sqr(x1 - x2) + sqr(y1 - y2));
      }


      template <typename T>
      void verify_integrity_(const image2d<T>& force,
			     int &x1,
			     int &x2,
			     int &y1,
			     int &y2)
      {
	if (x1 < 0)
	  x1 = 0;
	if (x2 < 0)
	  x2 = 0;
	if (x1 > force.nrows())
	  x1 = force.nrows() - 1;
	if (x2 > force.nrows())
	  x2 = force.nrows() - 1;

	if (y1 < 0)
	  y1 = 0;
	if (y2 < 0)
	  y2 = 0;
	if (y1 > force.ncols())
	  y1 = force.ncols() - 1;
	if (y2 > force.ncols())
	  y2 = force.ncols() - 1;
      }


      std::vector<point2d> draw_line_(const point2d& p,
				      const point2d& q)
      {
	std::vector<point2d> res;

	point2d p_cur = p;
	if (q.col() == p.col()) // cas vertical
	  {
	    int sens = q.row() > p.row() ? 1 : -1;
	    while (p_cur != q)
	      {
		double pc_p = euclidian_distance(p_cur.row(), p.row(), p_cur.col(), p.col());
		double pc_q = euclidian_distance(p_cur.row(), q.row(), p_cur.col(), q.col());
		double p_q = euclidian_distance(p.row(), q.row(), p.col(), q.col());

		res.push_back(p_cur);
		p_cur.row() = p_cur.row() + sens;
	      }
	  }
	else
	  {
	    if (q.row() == p.row()) // cas horizontal
	      {
		int sens = q.col() > p.col() ? 1 : -1;
		while (p_cur != q)
		  {
		    double pc_p = euclidian_distance(p_cur.row(), p.row(), p_cur.col(), p.col());
		    double pc_q = euclidian_distance(p_cur.row(), q.row(), p_cur.col(), q.col());
		    double p_q = euclidian_distance(p.row(), q.row(), p.col(), q.col());

		    res.push_back(p_cur);
		    p_cur.col() = p_cur.col() + sens;
		  }
	      }
	    else // cas diagonal
	      {
		long decalage = 0;

		long nom = q.row() - p.row();
		long denom = q.col() - p.col();
		int signe = (nom * denom) > 0 ? 1 : -1;
		nom = nom * signe;
		denom = denom * signe;

		int vsens = q.row() > p.row() ? 1 : -1;
		int hsens = q.col() > p.col() ? 1 : -1;

		int deboule = 0;

		while (p_cur != q)
		  {
		    double pc_p = euclidian_distance(p_cur.row(), p.row(), p_cur.col(), p.col());
		    double pc_q = euclidian_distance(p_cur.row(), q.row(), p_cur.col(), q.col());
		    double p_q = euclidian_distance(p.row(), q.row(), p.col(), q.col());

		    res.push_back(p_cur);

		    if (abs(decalage) >= abs(denom))
		      {
			p_cur.row() = p_cur.row() + vsens;
			decalage -= vsens * denom;
		      }
		    else
		      {
			p_cur.col() = p_cur.col() + hsens;
			decalage += nom * hsens;
		      }
		  }
	      }
	  }
	return res;
      }



      template <typename T>
      std::vector<point2d> compute_normal_points_(const image2d<T>& force,
						  const point2d& p1,
						  const point2d& p2,
						  const point2d& p3,
						  unsigned int dmax)
      {
	float x_c = p2.row();
	float y_c = p2.col();
	float a_p1_p3 = (p1.col() - p3.col()) != 0 ? (float)(((float)(p1.row() - p3.row())) / (p1.col() - p3.col())) : 0;
	float a = -a_p1_p3;
	float b = p2.col() - a * p2.row(); // use of - slope of p1 et p3

	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;

	if (a != 0)
	  {
	    float A = 1 + sqr(a);
	    float B = -2 * (x_c + a * y_c - a * b);
	    float C = sqr(x_c) + sqr((b - y_c)) - sqr(dmax);

	    float delta = sqr(B) - (4 * A * C);

	    if (delta > 0)
	      {
		x1 = (int)(roundf((-B + sqrt(delta)) / (2 * A)));
		x2 = (int)(roundf((-B - sqrt(delta)) / (2 * A)));

		if (x1 == x2)
		  {
		    float tmp1 = roundf((-B + sqrt(delta)) / (2 * A));
		    float tmp2 = roundf((-B - sqrt(delta)) / (2 * A));
		    if (tmp1 > tmp2)
		      x1++;
		    else
		      x2++;
		  }

		y1 = (int)(a * x1 + b);
		y2 = (int)(a * x2 + b);
	      }
	  }
	else
	  {
	    x1 = p2.row() - dmax;
	    y1 = p2.col();
	    x2 = p2.row() + dmax;
	    y2 = p2.col();
	  }

	verify_integrity_(force, x1, x2, y1, y2);

	return draw_line_(point2d(x1, y1), point2d(x2, y2));
      }


      // we can compute the second derivative.
      float angle_force(const point2d& p1, const point2d& p2, const point2d& p3)
      { // al kashi
	float b = euclidian_distance(p1.row(), p2.row(), p1.col(), p2.col());
	float c = euclidian_distance(p3.row(), p2.row(), p3.col(), p2.col());
	float a = euclidian_distance(p1.row(), p3.row(), p1.col(), p3.col());
	float angle =  acos((b + c - a) / (2 * sqrt(b) * sqrt(c))); // rad
	angle = (angle * 180) / PI; //deg
	return sqr(180 - angle) / sqr(180);
      }

      template <typename T>
      double variance(const image2d<T>& input,
		      const point2d& p,
		      unsigned int fen)
      {
	double moy = 0;
	double res = 0;

	for (int i = p.row() - fen / 2; i < p.row() + fen / 2; i++)
	  for (int j = p.col() - fen / 2; j < p.col() + fen / 2; j++)
	    {
	      point2d p(i, j);
	      if (input.hold(p))
		moy += input[p];
	    }

	moy /= sqr(fen);

	for (int i = p.row() - fen / 2; i < p.row() + fen / 2; i++)
	  for (int j = p.col() - fen / 2; j < p.col() + fen / 2; j++)
	    {
	      point2d p(i, j);
	      if (input.hold(p))
		res += sqr(input[p] - moy);
	    }

	return res / sqr(fen);
      }

      template <typename T>
      std::vector<point2d> gen_snakes_(const image2d<T>& force,
				       const std::vector<point2d>& v,
				       force_type f,
				       unsigned int fen,
				       float lambda,
				       unsigned int dmax,
				       snake_type type)
      {
	std::vector<point2d>::const_iterator it = v.begin();
	std::vector<point2d> res;

	for (; it != v.end(); it++)
	  {
	    point2d p1 = *it;
	    point2d p2 = *it;
	    point2d p3 = *it;

	    if (type)
	      {
		if (it != v.begin())
		  p1 = *(it - 1);
		if (it + 1 != v.end())
		  p3 = *(it + 1);
	      }
	    else
	      {
		if (it == v.begin())
		  continue;
		if (it + 1 == v.end())
		  continue;
		p1 = *(it - 1);
		p3 = *(it + 1);
	      }

	    std::vector<point2d> v_normal = compute_normal_points_(force, p1, p2, p3, dmax);
	    std::vector<point2d>::const_iterator it2 = v_normal.begin();

	    float max = -1000000000;
	    point2d n_pt(-1,-1);

	    for (; it2 != v_normal.end(); it2++)
	      {
		float v_force = 0;

		if (f == GRADIENT)
		  v_force = (float)(force[*it2]) / 255 - angle_force(p1, *it2, p3) * lambda;
		else
		  v_force = variance(force, *it2, fen) / 128 - angle_force(p1, *it2, p3) * lambda;

		if (max < v_force)
		  {
		    max = v_force;
		    n_pt = *it2;
		  }
	      }
	    if (n_pt.row() != -1)
	      res.push_back(n_pt);
	  }

	if (type == FERME)
	  {
	    res.push_back(res[0]);
	    res.push_back(res[1]);
	  }
	else
	  res.push_back(*(v.end() - 1));

	return res;
      }


      template <typename T>
      void init_view(const image2d<T>& input,
		     const std::vector<point2d>& v)
      {
	oln_iter_type(image2d<T>) p(input);
	image2d<ntg::int_u8> out(input.size());
	image2d<bool> mask(input.size());

	for_all(p)
	  mask[p] = false;

	std::vector<point2d>::const_iterator it = v.begin();
	for (; it != v.end(); it++)
	  mask[*it] = true;

	for_all(p)
	  {
	    if (mask[p] == true)
	      out[p] = 255;
	    else
	      out[p] = input[p];
	  }

	save(out, "init.pgm");
      }


      std::vector<point2d> regen_snaxels(std::vector<point2d>& v,
					 snake_type type,
					 unsigned max_pts)
      {
	std::vector<point2d> res;
	std::vector<point2d> tmp;
	std::vector<point2d>::iterator it = v.begin();
	std::vector<point2d>::iterator it2;

	if (2 * v.size() > max_pts)
	  return v;

	for (it = v.begin(); (it + 1) != v.end(); it++)
	  {
	    std::vector<point2d> tmp2 = draw_line_(*it, *(it+1));
	    for (it2 = tmp2.begin(); it2 != tmp2.end(); it2++)
	      tmp.push_back(*it2);
	  }

	int nb_pts = v.size() * 2;
	int cpt = 0;

	for (it2 = tmp.begin(); it2 != tmp.end(); it2++, cpt++)
	  if ((cpt % (tmp.size() / nb_pts)) == 0)
	    res.push_back(*it2);

	if (type == FERME)
	  {
	    res.push_back(res[0]);
	    res.push_back(res[1]);
	  }
 	else
 	  res.push_back(*(v.end() - 1));

	return res;
      }


      std::vector<point2d> redispatch(const std::vector<point2d>& v,
				      int nb_snaxels)
      {
	std::vector<point2d> res;
	std::vector<point2d> tmp;
	std::vector<point2d>::const_iterator it = v.begin();
	int cpt = 0;

	for (; it + 1 != v.end(); it++)
	  {
	    std::vector<point2d> vec = draw_line_(*it, *(it+1));
	    std::vector<point2d>::const_iterator it2 = vec.begin();

	    for (; it2 != vec.end(); it2++)
	      tmp.push_back(*it2);
	  }

	for (it = tmp.begin(); it != tmp.end(); it++, cpt++)
	  if ((cpt % (int)(roundf(((float)(tmp.size()) / nb_snaxels)))) == 0)
	    res.push_back(*it);

	res.push_back(*(v.end() - 1));
	return res;
      }


      template <typename T>
      void clean_ima(image2d<T>& ima)
      {
	for (int j = 0; j < ima.size().ncols(); j++)
	  {
	    ima(-1, j) = ima(0, j);
	    ima(ima.size().nrows(), j) = ima(ima.size().nrows() - 1, j);
	  }

	for (int j = 0; j < ima.size().nrows(); j++)
	  {
	    ima(j, -1) = ima(j, 0);
	    ima(j, ima.size().ncols()) = ima(j, ima.size().ncols() - 1);
	  }
      }


      template <typename T>
      std::vector<point2d>
      snakes_(const image2d<T>& input,
	      const std::vector<point2d>& v_init,
	      force_type f,
	      unsigned int fen,
	      unsigned int nb_gen,
	      unsigned int max_pts,
	      float lambda,
	      unsigned int dmax,
	      snake_type type)
      {
	image2d<T> force(input.size());

	if (f == GRADIENT)
	  force = morpho::gradient_morpho(input, win_c4p());
	else
	  {
	    oln_iter_type(image2d<T>) p(input);
	    for_all(p)
	      force[p] = input[p];
	  }

	clean_ima(force);

	std::vector<point2d> res;
	std::vector<point2d>::const_iterator it = v_init.begin();
	for (it = v_init.begin(); it != v_init.end(); it++)
	  res.push_back(*it);

	if (type == FERME)
	  {
	    res.push_back(res[0]);
	    res.push_back(res[1]);
	  }

	int i = 0;
	while (i < nb_gen)
	  {
	    res = gen_snakes_(force, res, f, fen, lambda, dmax, type);
	    res = redispatch(res, max_pts);
	    i++;
	    if (i < nb_gen)
	      res = regen_snaxels(res, type, max_pts);
	  }

	return res;
      }


    } // end of namespace oln::impl::snakes

    template <typename T>
    std::vector<point2d>
    snakes(const image2d<T>& input,
	   const std::vector<point2d>& v_init,
	   force_type f,
	   unsigned int fen,
	   unsigned int nb_gen,
	   unsigned int max_pts,
	   float lambda,
	   unsigned int dmax,
	   snake_type type = OUVERT)
    {
      return impl::snakes_(input, v_init, f, fen, nb_gen, max_pts, lambda, dmax, type);
    }

  } // end of namespace oln::appli

} // end of namespace oln


#endif // ! OLENA_SNAKES_HH
