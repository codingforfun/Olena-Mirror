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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef OLENA_LRDE_UFMT_GENERIC_SIMPLIFIED_SALEMBIER_HH
# define OLENA_LRDE_UFMT_GENERIC_SIMPLIFIED_SALEMBIER_HH


# include <climits> // For INT_MAX.
# include <cfloat>  // For FLT_MAX.
# include <queue>
# include <set>
# include <map>

# include <algorithm>
# include <iterator>

# include <oln/level/fill.hh>
# include <oln/lrde/ufmt/utils.hh>



namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {

      /*-------------.
      | point_less.  |
      `-------------*/

      template <typename I, typename P>
      struct point_less
      {
	// To be specialized.
      };

      template <typename I>
      struct point_less<I, oln::point2d>
      {
	point_less (const abstract::image<I>& f) :
	  f_(f.exact())
	{
	}

	bool
	operator() (const oln::point2d& p, const oln::point2d& q)
	{
	  // Ensure the equivalence of the ``strict'' weak ordering
	  // (http://www.sgi.com/tech/stl/StrictWeakOrdering.html) is
	  // preserved when P and Q are the same point.
	  if (p == q)
	    return false;

	  // Compare the values.
	  // FIXME: Use a more robust comparison function (for floats)?
	  float val_diff = f_[p] - f_[q];
	  if (val_diff < 0.f)
	    return true;
	  else if (val_diff > 0.f)
	    return false;
	  else
	    {
	      // Then compare the row numbers.
	      int row_diff = p.row() - q.row();
	      if (row_diff < 0)
		return true;
	      else if (row_diff > 0)
		return false;
	      else
		{
		  // Finally, compare the row numbers.
		  int col_diff = p.col() - q.col();
		  if (col_diff < 0)
		    return true;
		  else
		    return false;
		}
	    }
	}

	const I& f_;
      };


      /*----------------.
      | point_greater.  |
      `----------------*/

      template <typename I, typename P>
      struct point_greater
      {
	// To be specialized.
      };

      template <typename I>
      struct point_greater<I, oln::point2d>
      {
	point_greater (const abstract::image<I>& f) :
	  f_(f.exact())
	{
	}

	bool
	operator() (const oln::point2d& p, const oln::point2d& q)
	{
	  // Ensure the equivalence of the ``strict'' weak ordering
	  // (http://www.sgi.com/tech/stl/StrictWeakOrdering.html) is
	  // preserved when P and Q are the same point.
	  if (p == q)
	    return false;

	  // Compare the values.
	  // FIXME: Use a more robust comparison function (for floats)?
	  float val_diff = f_[p] - f_[q];
	  if (val_diff > 0.f)
	    return true;
	  else if (val_diff < 0.f)
	    return false;
	  else
	    {
	      // Then compare the row numbers.
	      int row_diff = p.row() - q.row();
	      if (row_diff > 0)
		return true;
	      else if (row_diff < 0)
		return false;
	      else
		{
		  // Finally, compare the row numbers.
		  int col_diff = p.col() - q.col();
		  if (col_diff > 0)
		    return true;
		  else
		    return false;
		}
	    }
	}

	const I& f_;
      };


      /* A simplified version of Salembier et al. algorithm.

         pset : a set of point, ordered by increasing pix values,
         then increasing rows, then increasing cols.

         points : the set of all the point, ordered by *decreasing* pix
         values, then increasing rows, then increasing cols.
         (a vector might be a good idea ?)

         node_at_levels : a set of points or a bit-array of points.

           

           flood (h)  // h is a point here, not a level!
  
             // FIXME: Use a fuzzy (tolerant) version of find?
             i = pset.find(q)
             while (i != pset.end())
               p <- *i                   // (Or : p <- pset.lower_bound(h) ?)
               STATUS(p) <- true
               for every neighbor q of p
                 if STATUS(q) == false
                   pset.insert(q)
                   STATUS(q) <- true
                   node-at-level.insert(q)
                   if (ORI(q) > ORI(p))
		     m = q
                     repeat
                       m <- flood(m)
                     until ORI(m) <= ORI(h)
               // FIXME: Use a fuzzy (tolerant) version of find?
               i = pset.find(q)
  
             // find first level lower than h beeing part of node_at_level.
             // m = sites.upper_bound(h)      // Recall that sites are sorted
                                              // in *descending* order.
             while (m != sites.end() and      // Was: « m >= 0 ».
                    node_at_level.find(*m) == node_at_level.end())
	       ++m;                           // Was: « m = h - 1 ».
             if m != sites.end()
               father of C(h) <- C(*m)
             else
               C(h) has no father       // C(*h) is root node.
             node-at-level.erase(h)
             return m
      */

      template <class I>
      struct generic_simplified_salembier
      {
	typedef oln_point_type(I) point;
	typedef oln_value_type(I) value;
	typedef oln_neighborhood_type(I) Nbh;
	typedef oln_iter_type(Nbh) niter;

	const I& f;
	const Nbh& nbh;

	// FIXME: In an index-based version, STATUS could be a bit
	// vector (of the STL)
	typename mute<I, bool>::ret status;

	// FIXME: Rename this attribute.
	// node_at_level.
 	typedef std::set< point, point_less<I, point> > node_at_level_t;
 	node_at_level_t node_at_level;

	// pset.
	typedef std::set< point, point_less<I, point> > pset_t;
	typedef typename pset_t::const_iterator pset_iterator;
	// Set of points to be processed (replaces the hqueue data structure).
	pset_t pset;
	point p_min;

	// All sites, sorted by increasing levels (then by row/col).
	typedef std::set< point, point_greater<I, point> > sites_t;
	typedef typename sites_t::const_iterator site_iterator;
	// Set of all values present in the input image, sorting in
	// descending order.
	sites_t sites;

	std::map<point, point, point_less<I, point> > father;

# ifdef OLN_DEBUG
	unsigned npoints;
# endif // OLN_DEBUG

	generic_simplified_salembier(const abstract::image<I>& f,
			  const oln_neighborhood_type(I)& nbh) :
	  f(f.exact()),
	  nbh(nbh),
  	  status(f.size()),
	  node_at_level(point_less<I, point>(f.exact())),
	  pset(point_less<I, point>(f.exact())),
	  sites(point_greater<I, point>(f.exact())),
	  father(point_less<I, point>(f.exact()))
# ifdef OLN_DEBUG
	  , npoints(f.npoints())
# endif // OLN_DEBUG
	{
	}


	void go()
	{
	  init();
//  	  // -- DEBUG ---------------------------------------------------
//  	  std::cerr << "p_min = " << p_min << std::endl;
//  	  std::cerr << "f[p_min] = " << f[p_min] << std::endl;
//  	  // --------------------------------------------------- DEBUG --
	  flood(p_min);
	}


	void init()
	{
	  level::fill(status, false);

	  oln_iter_type(I) p(f);
	  p = mlc::begin;
	  p_min = p;
	  value h_min = f[p];
	  for_all(p)
	    {
	      // Init sites.
	      sites.insert(p);
	      // Find the global minimum.
	      if (f[p] < h_min)
		{
		  h_min = f[p];
		  p_min = p;
		}
	    }
// 	  // -- DEBUG ---------------------------------------------------
//  	  std::cerr << "p_min = " << p_min << std::endl;
//  	  std::cerr << "h_min = " << h_min << std::endl;
// 	  // --------------------------------------------------- DEBUG --
	  pset.insert(p_min);
// 	  // -- DEBUG ---------------------------------------------------
//  	  std::cerr << "p_min = " << p_min << std::endl;
//  	  std::cerr << "h_min = " << h_min << std::endl;
// 	  // --------------------------------------------------- DEBUG --
	}

# ifdef OLN_DEBUG
	void update_status (std::ostream& ostr)
	{
	  static unsigned nupdates = 0;
	  static const unsigned step = 1;
	  static unsigned next_step = step;
	  ++nupdates;
	  unsigned percent = nupdates * 100 / npoints;
	  if (percent >= next_step)
	    {
	      // Print.
	      std::cerr << percent << "% points processed" << std::endl;
	      // Increase the next step.
	      while (percent >= next_step)
		next_step += step;
	    }
	}
# endif // OLN_DEBUG


	// H is a point here, not a level!
	// FIXME: Change the name of this argument, as it's confusing.
	site_iterator flood(const point& h)
	{
// 	  // -- DEBUG ---------------------------------------------------
// 	  std::cerr << "pset = ";
// 	  for (typename pset_t::const_iterator j = pset.begin();
// 	       j != pset.end(); ++j)
// 	    std::cerr << *j << " ";
// 	  std::cerr << std::endl;
// 	  // --------------------------------------------------- DEBUG --

//  	  // -- DEBUG ---------------------------------------------------
//  	  std::cerr << "h = " << h << std::endl;
//  	  // --------------------------------------------------- DEBUG --

	  site_iterator m;

	  pset_iterator i;
	  i = pset.find(h);
	  while (i != pset.end())
	    {
// 	      // -- DEBUG -----------------------------------------------
// 	      std::cerr << "  pset = ";
// 	      for (typename pset_t::const_iterator j = pset.begin();
// 		   j != pset.end(); ++j)
// 		std::cerr << *j << " ";
// 	      std::cerr << std::endl;
// 	      // ----------------------------------------------- DEBUG --

	      point p = *i;
// 	      // -- DEBUG -----------------------------------------------
// 	      std::cerr << "  p = " << p << std::endl;
// 	      // ----------------------------------------------- DEBUG --
	      pset.erase(i);

	      status[p] = true;
#ifdef OLN_DEBUG
	      update_status(std::cerr);
#endif // OLN_DEBUG

// 	      // -- DEBUG -----------------------------------------------
// 	      std::cerr << "  status = " << std::endl;
// 	      print_image(status, std::cerr);
// 	      // ----------------------------------------------- DEBUG --

	      oln_neighb_type(Nbh) q(nbh, p);
	      for_all(q)
		{
// 		  // -- DEBUG -------------------------------------------
// 		  std::cerr << "    q = " << q << std::endl;
// 		  // ------------------------------------------- DEBUG --

		  if (f.hold(q))
		    {
		      if (status[q] == false)
			{
			  pset.insert(q);
// 			  // -- DEBUG -----------------------------------
// 			  std::cerr << "    pset = ";
// 			  for (typename pset_t::const_iterator j =
// 				 pset.begin(); j != pset.end(); ++j)
// 			    std::cerr << *j << " ";
// 			  std::cerr << std::endl;
// 			  // ----------------------------------- DEBUG --
			  status[q] = true;
			  node_at_level.insert(q); // err. 1
			  if (f[q] > f[p])
			    {
			      m = sites.find(q);
			      if (m == sites.end())
				abort();
			      while (m != sites.end() and
				     f[*m] > f[h]) // err. 2
				m = flood(*m);
			    }
			}
		    }
		}
	      i = pset.find(h);
	    }

	  m = sites.upper_bound(h);
 	  if (m != sites.end())
	    ++m;                                // Was: « m = h - 1 ».
	  while (m != sites.end() and           // Was: « m >= 0 ».
		 node_at_level.find(*m) == node_at_level.end())
	    ++m;                                // Was: « m = h - 1 ».
	  if (m != sites.end())                 // Was: « m >= 0 ».
	    {
// 	      // -- DEBUG -----------------------------------------------
// 	      std::cerr << "father[" << h << "] = " << (*m) << std::endl;
// 	      // ----------------------------------------------- DEBUG --
	      father[h] = *m;
	    }
	  else
	    {
// 	      // -- DEBUG -----------------------------------------------
 	      std::cerr << "father[" << h << "] is root" << std::endl;
// 	      // ----------------------------------------------- DEBUG --
	      father[h] = h;
	    }
	  node_at_level.erase(h);
	  return m;
	}


	unsigned n_level_roots() const
	{
	  return father.size();
	}

	template <typename T>
	void
	print_image (const image2d<T>& ima, std::ostream& o)
	{
	  for (coord row = 0; row < ima.nrows(); ++row)
	    {
	      for (coord col = 0; col < ima.ncols(); ++col)
		o << ima(row, col) << " ";
	      o << std::endl;
	    }
	}


      }; // end of struct generic_simplified_salembier


    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_GENERIC_SIMPLIFIED_SALEMBIER_HH
