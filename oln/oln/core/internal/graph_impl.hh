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

#ifndef OLENA_CORE_INTERNAL_GRAPH_IMPL_HH
# define OLENA_CORE_INTERNAL_GRAPH_IMPL_HH

# include <oln/core/graph.hh>
# include <vector>
# include <list>

namespace oln {

  namespace internal {

    // Handler that encapsulates a T.
    template <class Tag, class T>
    struct handler
    {
      handler();
      handler(T v);
      T		value;
      operator T() const;
      bool operator==(const handler<Tag, T>&) const;
    };

    // Empty class used for typing purpose.
    struct edge_tag {};
    // Empty class used for typing purpose.
    struct node_tag {};

    // Strong type for edge handler (enable overloading of []).
    typedef handler<edge_tag, int> hedge_t;
    // Strong type for node handler (enable overloading of []).
    typedef handler<node_tag, int> hnode_t;

    // Mixin class to add edge stuff to EdgeValue.
    template <class EdgeValue>
    class edge_decorator : public EdgeValue
    {
    public:
      edge_decorator(hnode_t, hnode_t, const EdgeValue& v = EdgeValue());

      hnode_t   from() const;
      hnode_t&  from();
      hnode_t   to() const;
      hnode_t&  to();

    private:
      hnode_t from_;
      hnode_t to_;
    };

    // Mixin class to add node stuff to NodeValue.
    template <class NodeValue>
    struct node_decorator : public NodeValue
    {
      node_decorator(const NodeValue&);
      node_decorator();
      std::list<hedge_t> edges;
    };

    // Fully connected implementation of graph.
    template <class NodeValue, class EdgeValue>
    class heavy_graph : public graph<heavy_graph<NodeValue, EdgeValue> >
    {
    public:
      typedef NodeValue				node_value_t;
      typedef EdgeValue				edge_value_t;
      typedef node_decorator<NodeValue>		decorated_node_value_t;
      typedef edge_decorator<EdgeValue>		decorated_edge_value_t;
      typedef std::vector<decorated_node_value_t>		nodes_set_t;
      typedef std::vector<decorated_edge_value_t>		edges_set_t;
      typedef oln::internal::hnode_t		hnode_t;
      typedef oln::internal::hedge_t		hedge_t;
      typedef heavy_graph<NodeValue, EdgeValue> self_t;
      typedef std::list<hedge_t>		edges_of_node_set_t;
      typedef typename nodes_set_t::const_iterator  nodes_set_const_iterator;
      typedef typename edges_set_t::const_iterator  edges_set_const_iterator;
      
      // we guarentee that the handler of node are indexed from 0 to
      // initial_number_of_node - 1 when using this constructor.
      heavy_graph(unsigned initial_number_of_node,
		  unsigned number_of_edge_initially_allocated);
      heavy_graph();

      // "virtual" methods from the abstract class "graph".
      const nodes_set_t&	nodes() const;
      const edges_set_t&	edges() const;
      nodes_set_t&		nodes();
      edges_set_t&		edges();
      hnode_t			hnode_of(nodes_set_const_iterator) const;
      hedge_t			hedge_of(edges_set_const_iterator) const;
      hnode_t			add_node(const node_value_t& t = node_value_t());
      void			del_node(hnode_t n);
      hedge_t			add_edge(hnode_t h1, hnode_t h2, 
					 const edge_value_t& v = edge_value_t());
      void			del_edge(hedge_t e);
      hnode_t			from(hedge_t e1) const;
      hnode_t			to(hedge_t e2) const;
      hnode_t			from(edges_set_const_iterator e1) const;
      hnode_t			to(edges_set_const_iterator e2) const;
      const edges_of_node_set_t&edges_of(hnode_t n) const;
      node_value_t&		operator[](hnode_t n);
      const node_value_t&	operator[](hnode_t n) const;
      edge_value_t&		operator[](hedge_t n);
      const edge_value_t&	operator[](hedge_t n) const;
      bool			hold(hnode_t n) const;
      bool			hold(hedge_t n) const;
      self_t&			clone() const;
      static std::string	name();

    private:
      nodes_set_t		nodes_;
      edges_set_t		edges_;
      std::list<hnode_t>	removed_nodes_;
      std::list<hedge_t>	removed_edges_;
    };

  } // end of internal.

    // traits for this implementation:
  template <class NodeValue, class EdgeValue>
  struct graph_traits<internal::heavy_graph<NodeValue, EdgeValue> >
  {
    typedef NodeValue					node_value_t;
    typedef EdgeValue					edge_value_t;
    typedef std::vector<node_value_t>			nodes_set_t;
    typedef std::vector<edge_value_t>			edges_set_t;
    typedef oln::internal::hnode_t			hnode_t;
    typedef oln::internal::hedge_t			hedge_t;
    typedef internal::heavy_graph<NodeValue, EdgeValue> self_t;
    typedef std::list<hedge_t>				edges_of_node_set_t;
  };

} // end of oln.

# include <oln/core/internal/graph_impl.hxx>

#endif // OLENA_CORE_INTERNAL_GRAPH_IMPL_HH
