// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_TOPO_COMBINATORIAL_MAP_CMAP_HH
# define OLENA_TOPO_COMBINATORIAL_MAP_CMAP_HH

# include <oln/topo/tarjan/flat-zone.hh>
# include <oln/topo/inter-pixel/inter-pixel.hh>
# include <oln/topo/combinatorial-map/internal/zeta.hh>
# include <oln/topo/combinatorial-map/internal/allfunc.hh>
# include <oln/topo/combinatorial-map/internal/level.hh>

# include <algorithm>
# include <iterator>

namespace oln {

  namespace topo {

    namespace combinatorial_map {

      template <class I>
      class cmap
      {
      public:
	typedef oln_dpoint_type(I)		dpoint_type;
	typedef oln_point_type(I)		point_type;
	typedef oln_fwd_dir_iter_type(I)	fwd_dir_iter_type;
	typedef oln_bkd_dir_iter_type(I)	bkd_dir_iter_type;
	typedef oln_head_type(I)		head_type;
	typedef oln_zeta_type(I)		zeta_type;

      public:
	cmap(const I& input, 
	     const inter_pixel::interpixel<I>& ip, 
	     tarjan::flat_zone<I>& cc) :
	  ndarts_(0),
	  zeta_(input.nrows() + 1, input.ncols() + 1),
	  cc_(cc),
	  level_(cc.nlabels()),
	  beta_(cc.nlabels())
	{
	  build_zeta_(ip);

	  sigma_.resize(ndarts_);
	  lambda_.resize(ndarts_);

	  build_functions_(ip);
	}

	/*-------------.
	| neighborhood |
	`-------------*/

	// ext_neighb:
	// give a labels' set of connected component of given label which
	// are ouside of l.

	// FIXME: why is this code disabled? Should it be removed?

#if 0

	void
	ext_neighb(const unsigned l, labels_type & n)
	{
	  if (beta_(l) == 0)
	    return;

	  unsigned d = beta_(l);

	  do
	    {
	      n.insert(lambda_(alpha_(d)));

	      d = sigma_(alpha_(d));
	    }
	  while (d != beta_(l));
	}

	// int_neighb:
	// give a labels' set of connected component of given label which
	// are inside of l.
	void
	int_neighb(const unsigned l, labels_type & n)
	{
	  labels_type ext;

	  labels_type::const_iterator it;
	  for (it = daugh_(l).begin(); it != daugh_(l).end(); ++it)
	    {
	      ext.clear();
	      ext_neighb(*it, ext);

	      if (ext.find(l) != ext.end())
		n.insert(*it);
	    }
	}

	// neighb
	// give a label' set of both inside and outside connected component
	void
	neighb(const unsigned l, labels_type & n)
	{
	  ext_neighb(l, n);
	  int_neighb(l, n);
	}

# endif

	/*------------------.
	| merging algorithm |
	`------------------*/

	void
	merge(const unsigned l1, const unsigned l2)
	{
	  static std::vector<unsigned> inter;

	  // FIXME: Do not check whether a label is valide on beta function
	  assertion(beta_(l1) && beta_(l2));

	  if (level_.father(l2) != l1)
	    {
	      // l1 and l2 are connected
	      assertion(make_inter1_(l1, l2, inter));
	    }
	  else
	    {
	      // l2 is included in l1
	      assertion(make_inter2_(l1, l2, inter));
	    }

	  update_darts_(l1, l2, inter);

	  level_.merge(l1, l2);

	  cc_.merge(l1, l2);

	  inter.clear();
	}

	/*------.
	| print |
	`------*/

	std::ostream&
	print(std::ostream & ostr) const
	{
	  ostr << sigma_ << std::endl;
	  ostr << lambda_ << std::endl;
	  ostr << beta_ << std::endl;

 	  ostr << level_ << std::endl;

	  return ostr;
	}

      private:

	/*---------------------------.
	| building combinatorial map |
	`---------------------------*/

	void
	build_zeta_(const inter_pixel::interpixel<I> & ip)
	{
	  unsigned father = 0;

	  std::list<point_type> acc;
	  fwd_dir_iter_type i;

	  for (unsigned l = 2; l <= cc_.nlabels(); ++l)
	    if (level_.father(l) == 0)
	      {
		acc.clear();

		point_type n1 = cc_.get_root(l);

		father = cc_.get_label(n1 + dpoint_type(0, -1));

		head_type h1 = ip.folw(head_type(n1, oln_dir_traits_type(I)::first()));

		acc.push_back(h1.first);

		while (!acc.empty())
		  {
		    n1 = acc.front();

		    for_all(i)
		      if (ip[n1].get(i) && !zeta_[n1][i])
			{
			  unsigned l = cc_.get_label(n1 + ip.neighb_[i]);

			  if (father != l && !level_.father(l))
			    level_.insert(father, l);

			  zeta_[n1][i] = ++ndarts_;
			  head_type h2 = ip.folw(head_type(n1, i));

			  if (zeta_[h2.first].empty())
			    acc.push_back(h2.first);

			  zeta_[h2.first][h2.second] = ++ndarts_;
			}

		    acc.pop_front();
		  }
	      }
	}

	void
	build_functions_(const inter_pixel::interpixel<I> & ip)
	{
	  oln_iter_type(zeta_type) it(zeta_);
	  fwd_dir_iter_type i;
	  
	  for_all(it)
	    {
	      if (!zeta_[it].empty())
		for_all(i)
		{
		  unsigned d = zeta_[it][i];

		  if (d)
		    {
		      fwd_dir_iter_type j(i.next());
		      for_all(j)
			{
			  unsigned s = zeta_[it][j];

			  if (s)
			    {
			      unsigned l = cc_.get_label(it + ip.neighb_[i]);

			      sigma_.assign(d, s);
			      lambda_.assign(d, l);
			      beta_.assign(l, d);

			      break;
			    }
			}
		    }
		}
	    }
	}

	/*---------------------------.
	| Internal methods for merge |
	`---------------------------*/

	bool 
	make_inter1_(const unsigned l1,
		     const unsigned l2,
		     std::vector<unsigned> & inter)
	{
	  static std::vector<bool> buff(ndarts_);

 	  unsigned d = beta_(l1);

 	  do
 	    {
 	      buff[d] = true;
 	      d = sigma_(internal::alpha<unsigned>::result(d));
 	    }
 	  while (d != beta_(l1));

 	  d = beta_(l2);

	  unsigned d_ = 0;

 	  do
 	    {
	      d_ = internal::alpha<unsigned>::result(d);

 	      if (buff[d_])
		inter.push_back(d_);
	      d = sigma_(d_);
 	    }
 	  while (d != beta_(l2));

	  buff.clear();

	  return !inter.empty();
	}

	bool
	make_inter2_(const unsigned l1,
		     const unsigned l2,
		     std::vector<unsigned> & inter)
	{
	  unsigned d = beta_(l2);
	  unsigned d_;

	  do
	    {
	      d_ = internal::alpha<unsigned>::result(d);

	      if (lambda_(d_) == l1)
		inter.push_back(d_);

 	      d = sigma_(d_);
	    }
	  while (d != beta_(l2));

	  return !inter.empty();
	}

	void
	update_darts_(const unsigned l1,
		      const unsigned l2,
		      std::vector<unsigned> & inter)
	{
	  std::vector<unsigned>::const_iterator i;
	  for (i = inter.begin(); i != inter.end(); ++i)
	    {
	      unsigned d = *i;

	      sigma_.erase(d);
	      lambda_.erase(d);
	    }

	  // FIXME: find a way to get this in constant time
	  for (unsigned j = 1; j < ndarts_; ++j)
	    if (lambda_(j) == l2)
	      lambda_.assign_(j, l1);

	  beta_.erase(l2);
	}

      private:
	unsigned			ndarts_;
	zeta_type			zeta_;
	tarjan::flat_zone<I> &		cc_;
	internal::level<unsigned>	level_;

	internal::beta<unsigned>	beta_;
	internal::sigma<unsigned>	sigma_;
	internal::lambda<unsigned>	lambda_;
      };

    } // end of namespace combinatorial_map

  } // end of namespace topo

} // end of namespace oln

template<class I>
inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::combinatorial_map::cmap<I> & cm)
{
  return cm.print(ostr);
}

#endif // ! OLENA_TOPO_COMBINATORIAL_MAP_CMAP_HH
