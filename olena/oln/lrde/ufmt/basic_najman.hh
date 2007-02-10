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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_LRDE_UFMT_BASIC_NAJMAN_HH
# define OLENA_LRDE_UFMT_BASIC_NAJMAN_HH

# include <list>

# include <oln/level/fill.hh>
# include <oln/lrde/ufmt/utils.hh>
# include <oln/lrde/ufmt/ap_maxtree.hh>


namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {

      template <class I>
      struct basic_najman : public ap_maxtree<I>
      {
	typedef ap_maxtree<I> super;
	using super::f;
	using super::nbh;
	using super::par;
	oln_lrde_ufmt_import_ap_maxtree_typedefs;

	struct node {
	  int level;
	  int area;
	  int highest;
	  std::list<point> children;

	  void addChildren(const node& n)
	  {
	    typename std::list<point>::const_iterator it;
	    for (it = n.children.begin();
		 it != n.children.end();
		 ++it)
	      children.push_back((*it));
	  }
	  void addChild(const point n)
	  {
	    children.push_back(n);
	  }
	};

	image2d<point> Par_tree;
	// Par_node is handled by par (from ap_maxtree)
	image2d<int> Rnk_tree;
	image2d<int> Rnk_node;
	image2d<point> lowestNode;
	image2d<node> nodes;
	image2d<bool> isproc;
	point Root;
        std::vector<point> S;
	// std::map<point,point> M; // to keep the root of any point.

	
	basic_najman(const abstract::image<I>& f,
		     const oln_neighborhood_type(I)& nbh)
          : ap_maxtree<I>(f, nbh),
	  Par_tree(f.size()),
	  Rnk_tree(f.size()),
	  Rnk_node(f.size()),
	  lowestNode(f.size()),
	  nodes(f.size()),
          isproc(f.size())
        {
        }

 	void MakeSet_tree(point x)
 	{
 	  Par_tree[x] = x;
	  Rnk_tree[x] = 0;
 	}

 	void MakeSet_node(point x)
 	{
	  par[x] = x; // was Par_node[x] = x;
	  Rnk_node[x] = 0;
 	}

	point Find_tree(point x)
	{
	  if (Par_tree[x] != x)
	    Par_tree[x] = Find_tree(Par_tree[x]);
	  return Par_tree[x];
	}

	point Find_node(point x)
	{
	  return find_level_root(x);
	}

        void go()
        {
          init();
	  BuildComponentTree();
        }

        void init()
        {
          S = histogram_reverse_sort_p(f);
          level::fill(isproc, false);
	  for (int ip = 0; ip < int(S.size()); ++ip)
	    {
	      point p = S[ip];
	      MakeSet_node(p);
	      MakeSet_tree(p);
// 	      if (lowestNode.hold(p))
	      lowestNode[p] = p;
// 	      if (nodes.hold(p))
	      nodes[p] = MakeNode(f[p]);
	    }
	}

	void BuildComponentTree()
	{
	  for (int ip = 0; ip < int(S.size()); ++ip)
	    {
	      point p = S[ip];
	      isproc[p] = true;
	      
	      point curTree = Find_tree(p);
	      point curNode = Find_node(lowestNode[curTree]);

	      oln_neighb_type(Nbh) q(nbh, p);
	      for_all(q)
		if (f.hold(q) and isproc[q] and f[q] >= f[p]) // f[q] >= f[p] parce qu'on prend l'ordre de l'histogramme
		  {
		    point adjTree = Find_tree(q);
		    point adjNode = Find_node(lowestNode[adjTree]);
		    if (curNode != adjNode)
		      {
			if (nodes[curNode].level == nodes[adjNode].level)
			  curNode = MergeNode(adjNode, curNode);
			else
			  {
			    // we have nodes[curNode].level < nodes[adjNode].level
			    nodes[curNode].addChild(adjNode);
			    //apparemment NON :Par_node[adjNode] = curNode; // car adjNode devient fils de curNode
			    nodes[curNode].area += nodes[adjNode].area;
			    nodes[curNode].highest += nodes[adjNode].highest;
			  }
		      }
		    curTree = Link_tree(adjTree, curTree);
		    lowestNode[curTree] = curNode;
		  }
	    }
	  Root = lowestNode[Find_tree(Find_node(point(0, 0)))];
	  // Pour garder une map de correspondance point <-> local_root
//            for (int ip = 0; ip < int(S.size()); ++ip)
//              {
//                point p = S[ip];
// 	       M(p) = Find_node(p);
// 	     }
	}

	point MergeNode(point& node1, point& node2)
	{
	  point tmpNode = Link_node(node1, node2);
	  point tmpNode2;
	  if (tmpNode == node2)
	    {
	      nodes[node2].addChildren(nodes[node1]);
	      tmpNode2 = node1;
	    }
	  else
	    {
	      nodes[node1].addChildren(nodes[node2]);
	      tmpNode2 = node2;
	    }
	  nodes[tmpNode].area += nodes[tmpNode2].area;
	  nodes[tmpNode].highest += nodes[tmpNode2].highest;
	  return tmpNode;
	}

	point Link_tree(point x, point y)
	{
	  if (Rnk_tree[x] > Rnk_tree[y])
	    swap(x, y);
	  else
	    if (Rnk_tree[x] == Rnk_tree[y])
	      Rnk_tree[y] += 1;
	  Par_tree[x] = y;
	  return y;
	}
	
	point Link_node(point x, point y)
	{
	  if (Rnk_node[x] > Rnk_node[y])
	    swap(x, y);
	  else
	    if (Rnk_node[x] == Rnk_node[y])
	      Rnk_node[y] += 1;
	  par[x] = y; // was Par_node[x] = y;
	  return y;
	}
	
	node MakeNode(int level)
	{
	  node n;
	  n.level = level;
	  n.area = 1;
	  n.highest = level;
	  return n;
	}

// 	void image_output(image2d<point>& img)
// 	{
// 	  for(int i = 0; i < img.nrows(); ++i)
// 	    {
// 	      for(int j = 0; j < img.ncols(); ++j)
// 		{
// 		  int l = (img[point(i, j)]).row() * img.ncols() + (img[point(i, j)]).col();
// 		  std::cout << " " << l << " ";
// 		}
// 	      std::cout << std::endl;
// 	    }
// 	}



// 	void print_tree(point p)
// 	{
// 	  node& n = nodes[p];
// 	  std::cout << "point " << p << "=" << (p.row() * lowestNode.ncols() + p.col()) << " : ";

// 	  typename std::list<point>::const_iterator it;
// 	  for (it = n.children.begin();
// 	       it != n.children.end();
// 	       ++it)
// 	    {
// 	      point q = *it;
// 	      std::cout << q << "=" << (q.row() * lowestNode.ncols() + q.col()) << " ";
// 	    }
// 	  std::cout << std::endl;

// 	  for (it = n.children.begin();
// 	       it != n.children.end();
// 	       ++it)
// 	    print_tree(*it);
// 	}

      }; // end of struct basic_najman
      
    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_BASIC_NAJMAN_HH
