// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_GRAPH_HH
# define OLENA_CORE_GRAPH_HH

# include <oln/meta/basics.hh>

namespace oln {

  // A set of types that must be defined by graph implementation.
  template <class T>
  struct graph_traits
  {
    typedef meta::undefined node_value_t;
    typedef meta::undefined edge_value_t;
    typedef meta::undefined nodes_set_t;
    typedef meta::undefined edges_set_t;
    typedef meta::undefined edges_of_node_set_t;
    // hnode_t must be different to hedge_t.
    // handler are equivalent to unsigned.
    typedef meta::undefined hnode_t;
    typedef meta::undefined hedge_t;
    typedef meta::undefined iterator;
  };

  // Abstract class for a graph structure.
  // Every implementation of graph provides these services.
  template <class U>
  class graph 
  {
  public:
    // value attached to each node.
    typedef typename graph_traits<U>::node_value_t	  node_value_t;
    // value attached to each edge.
    typedef typename graph_traits<U>::edge_value_t	  edge_value_t;
    // handler of node (equivalent to an unsigned).
    typedef typename graph_traits<U>::hnode_t		  hnode_t;
    // handler of edge (equivalent to an unsigned).
    typedef typename graph_traits<U>::hedge_t		  hedge_t;
    // set of node values.
    typedef typename graph_traits<U>::nodes_set_t         nodes_set_t;
    // set of edge values.
    typedef typename graph_traits<U>::edges_set_t         edges_set_t;
    // set of edge linked to a node.
    typedef typename graph_traits<U>::edges_of_node_set_t edges_of_node_set_t;
    // const iterator on node values.
    typedef typename nodes_set_t::const_iterator     nodes_set_const_iterator;
    // const iterator on edge values.
    typedef typename edges_set_t::const_iterator     edges_set_const_iterator;
    // iterator on node values.
    typedef typename nodes_set_t::iterator		nodes_set_iterator;
    // iterator on edge values.
    typedef typename edges_set_t::iterator		edges_set_iterator;

    // Static dispatch stuff (will be erased when graph is integrated
    // into the static hierarchy).
    U			&self();
    const U		&self() const;

    // Set of node values accessor (const version).
    const nodes_set_t&  nodes() const;
    // Set of edge values accessor (const version).
    const edges_set_t&  edges() const;
    // Set of node values accessor (mutable version).
    nodes_set_t&	nodes();
    // Set of edge values accessor (mutable version).
    edges_set_t&	edges();
    // Return the node handler associated with a particular node value.
    hnode_t		handler_of(nodes_set_const_iterator) const;
    // Return the edge handler associated with a particular edge value.
    hedge_t		handler_of(edges_set_const_iterator) const;
    // Add a new node optionally initialized with a node value.
    hnode_t		add_node(const node_value_t& t = node_value_t());
    // Remove a node.  The coherence maintenance of the graph is
    // implementation-dependent because of algorithmic issues.
    void		del_node(hnode_t n);
    // Add a new edge between two nodes.
    hedge_t		add_edge(hnode_t h1, hnode_t h2);
    // Remove an edge. The coherence maintenance of the graph is
    // implementation-dependent because of algorithmic issues.
    void		del_edge(hedge_t e);
    // Return the "first" node of an edge. The graph is not directed,
    // "from" is just a mnenomic name. (FIXME: node1 or first ?)
    hnode_t		from(hedge_t e1) const;
    // Return the "second" node of an edge. (See "from").
    hnode_t		to(hedge_t e2) const;
    // Set of edges that are linked to a node.
    edges_of_node_set_t edges_of(hnode_t n) const;
    // Node value accessor (mutable version).
    node_value_t&	operator[](hnode_t n);
    // Node value accessor (const version).
    const node_value_t& operator[](hnode_t n) const;
    // Edge value accessor (mutable version).
    edge_value_t&	operator[](hedge_t n);
    // Edge value accessor (const version).
    const edge_value_t& operator[](hedge_t n) const;
    // Tests if the node handler if valid.
    bool		hold(hnode_t n) const;
    // Tests if the node handler if valid.
    bool		hold(hedge_t n) const;
    // Return a fresh graph equal to "this".
    graph<U>&		clone() const;
    // Static type name of this class.
    static std::string  name();
  protected:
    // Default constructor is protected since it is an abstract class.
    graph();
  };

} // end of oln.

# include <oln/core/graph.hxx>

#endif // OLENA_CORE_GRAPH_HH
