
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

#ifndef OLENA_TOPO_TARJAN_FLAT_ZONE_HH
# define OLENA_TOPO_TARJAN_FLAT_ZONE_HH

# include <oln/basics2d.hh>
# include <oln/topo/union.hh>

# include <map>

namespace oln {

  namespace topo {

    namespace tarjan {

      //
      //  Class flat_zone<_I>
      //
      //////////////////////////////////

      template <class _I>
      struct flat_zone
      {
	typedef Point(_I) point_t;
	typedef Value(_I)  data_t;
	typedef Concrete(_I) image_t;
	typedef tarjan::tarjan_set<image_t, tarjan::EMPTY_CLASS> tarjan_cc;

	const image_t &input;
	tarjan_cc cc;
	image2d<unsigned> label;

	std::vector<point_t> look_up_table;

	image2d< std::vector<oln::point2d> > ima_region;

	unsigned _nlabels;

	flat_zone(const image_t& _input) : input(_input), cc(_input),
					   label(_input.size()),
					   ima_region(_input.size()),
					   _nlabels(0)
	{
	  doit();
	}

	void doit()
	{
	  // COMPUTE CC MAP
	  {
	    window2d c4;
	    c4.add(0,1).add(1,0);
	    typename image_t::bkd_iter p(input);
	    for_all(p)
	      {
		cc.make_set(p);
		window2d::neighb p_prime(c4, p);
		for_all (p_prime) if (input.hold(p_prime))
		  if (input[p] == input[p_prime])
		    cc.uni(p_prime.cur(), p);
	      }

	    cc.make_set(point_t(input.nrows(), input.ncols()));

	    // bottom-right corner -> bottom-left corner
	    for (int j = input.ncols() - 1; j >= 0; --j)
	      {
		cc.make_set(point_t(input.nrows(), j));
		cc.uni(point_t(input.nrows(), j + 1), point_t(input.nrows(), j));
	      }

	    // bottom-right corner -> top-right corner
	    for (int i = input.nrows() - 1; i >= 0; --i)
	      {
		cc.make_set(point_t(i, input.ncols()));
		cc.uni(point_t(i + 1, input.ncols()), point_t(i, input.ncols()));
	      }

	    // top-right corner -> top-left corner - 1
	    for (int j = input.ncols() - 1; j > 0; --j)
	      {
		cc.make_set(point_t(-1, j));
		cc.uni(point_t(-1, j + 1), point_t(-1, j));
	      }

	    // bottom-left corner -> top-left corner - 1
	    for (int i = input.nrows() - 1; i > 0; --i)
	      {
		cc.make_set(point_t(i, -1));
		cc.uni(point_t(i + 1, -1), point_t(i, -1));
	      }

	    cc.make_set(point_t(-1, -1));

	    cc.uni(point_t(-1, 0), point_t(-1, -1));
	    cc.uni(point_t(0, -1), point_t(-1, -1));
	  }

	  // COMPUTE LABEL MAP
	  {
	    typename image_t::fwd_iter p(input);

	    // Push_back because no label = 0;
	    look_up_table.push_back(point_t());

	    border::adapt_assign(label, 1, ++_nlabels);

	    look_up_table.push_back(cc.find_root(point_t(-1, -1)));

	    for_all(p)
	      {
		if (cc.is_root(p))
		  {
		    label[p] = ++_nlabels;
		    look_up_table.push_back(p);
		  }
		else
		  label[p] = label[cc.parent[p]];
	      }

	    {
	      typename image_t::fwd_iter p(input);
	      for_all(p)
		{
		  if (cc.is_root(p))
		    ima_region[p].push_back(p);
		  else
		    ima_region[cc.parent[p]].push_back(p);
		}
	    }

	  }
	}

	const unsigned get_label(const point_t & p) const
	{
	  return label[p];
	}

	const point_t & get_root(unsigned l) const
	{
	  return look_up_table[l];
	}

	const unsigned nlabels() const
	{
	  return _nlabels;
	}

	void merge(const int l1, const int l2)
	{
	  point_t root_l1 = look_up_table[l1];
	  point_t root_l2 = look_up_table[l2];
	  assertion(cc.is_root(root_l1));
	  assertion(cc.is_root(root_l2));
	  // merge
	  cc.uni(root_l2, root_l1);
	  // update our tables
	  look_up_table[l2] = root_l1;
	  for (typename std::vector<point_t>::iterator
		 i = ima_region[root_l2].begin();
	       i != ima_region[root_l2].end(); ++i)
            label[*i] = l1;
	  ima_region[root_l1].insert(ima_region[root_l1].end(),
				     ima_region[root_l1].begin(),
				     ima_region[root_l1].end());
	  ima_region[root_l2].clear();

	  --_nlabels;
	}

      }; // end class

    } // end tarjan

  } // end topo

} // end oln


#endif // !OLENA_TOPO_TARJAN_FLAT_ZONE_HH
