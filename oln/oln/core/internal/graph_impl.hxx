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

#ifndef CORE_INTERNAL_GRAPH_IMPL_HXX
# define CORE_INTERNAL_GRAPH_IMPL_HXX

# include <oln/core/contract.hh>
# include <vector>
# include <oln/core/graph.hh>

namespace oln {

  namespace internal {

    template <class Tag, class T>
    handler<Tag, T>::handler(T v) :
      value(v)
    {}

    template <class Tag, class T>
    handler<Tag, T>::handler() :
      value()
    {}
    
    template <class Tag, class T>
    handler<Tag, T>::operator T()
    {
      return value;
    }

    template <class Tag, class T>
    bool
    handler<Tag, T>::operator==(const handler<Tag, T>& other) const
    {
      return value == other.value;
    }

    template <class EdgeValue>
    edge_decorator<EdgeValue>::edge_decorator(hnode_t h1,
					      hnode_t h2, 
					      const EdgeValue& v) :
      from_(h1),
      to_(h2),
      EdgeValue(v)
    {}

    template <class EdgeValue>
    hnode_t
    edge_decorator<EdgeValue>::from() const
    {
      return from_;
    }

    template <class EdgeValue>
    hnode_t&
    edge_decorator<EdgeValue>::from() 
    {
      return from_;
    }

    template <class EdgeValue>
    hnode_t
    edge_decorator<EdgeValue>::to() const
    {
      return to_;
    }

    template <class EdgeValue>
    hnode_t&
    edge_decorator<EdgeValue>::to() 
    {
      return to_;
    }

    template <class NodeValue>
    node_decorator<NodeValue>::node_decorator()
    {}

    template <class NodeValue>
    node_decorator<NodeValue>::node_decorator(const NodeValue& n) :
      NodeValue(n)
    {}
    
    template <class NodeValue, class EdgeValue>
    heavy_graph<NodeValue, EdgeValue>::heavy_graph
    (unsigned initial_number_of_node, 
     unsigned reserve_number_of_edge)
    {
      nodes_.resize(initial_number_of_node);
      edges_.reserve(reserve_number_of_edge);
    }

    template <class NodeValue, class EdgeValue>
    const typename heavy_graph<NodeValue, EdgeValue>::edges_of_node_set_t&
    heavy_graph<NodeValue, EdgeValue>::edges_of(hnode_t n) const
    {
      precondition(hold(n));
      return nodes_[n].edges;
    }

    template <class NodeValue, class EdgeValue>
    heavy_graph<NodeValue, EdgeValue>::heavy_graph()
    {
    }

    template <class NodeValue, class EdgeValue>
    const typename heavy_graph<NodeValue, EdgeValue>::nodes_set_t&	
    heavy_graph<NodeValue, EdgeValue>::nodes() const
    {
      return nodes_;
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::nodes_set_t&	
    heavy_graph<NodeValue, EdgeValue>::nodes() 
    {
      return nodes_;
    }
      
    template <class NodeValue, class EdgeValue>
    const typename heavy_graph<NodeValue, EdgeValue>::edges_set_t&	
    heavy_graph<NodeValue, EdgeValue>::edges() const
    {
      return edges_;
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::edges_set_t&
    heavy_graph<NodeValue, EdgeValue>::edges() 
    {
      return edges_;
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::hedge_t
    heavy_graph<NodeValue, EdgeValue>::
    handler_of(edges_set_const_iterator i) const
    {
      precondition(i != edges_.end());
      return i - edges_.begin();
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::hnode_t
    heavy_graph<NodeValue, EdgeValue>::
    handler_of(nodes_set_const_iterator i) const
    {
      precondition(i != nodes_.end());
      return i - nodes_.begin();
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::hnode_t	
    heavy_graph<NodeValue, EdgeValue>::add_node(const node_value_t& t)
    {
      if (removed_nodes_.size() == 0)
	{
	  nodes_.push_back(t);
	  return nodes_.size() - 1;
	}
      hnode_t n = removed_nodes_.front();
      removed_nodes.pop_front();
      assertion(n < nodes_.size());
      nodes_[n] = t;
      return n;
    }

    template <class NodeValue, class EdgeValue>
    void			
    heavy_graph<NodeValue, EdgeValue>::del_node(hnode_t n)
    {
      precondition(hold(n));
      const node_value_t& v = nodes_[n];
      for (typename edges_of_node_set_t::const_iterator e = v.edges.begin();
	   e != v.edges.end();
	   ++e)
	this->del_edge(*e);
      removed_nodes_.push_front(n);
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::hedge_t			
    heavy_graph<NodeValue, EdgeValue>::add_edge(hnode_t h1, hnode_t h2,
						const edge_value_t& v)
    {
      precondition(hold(h1) && hold(h2));
      hedge_t e;
      if (removed_edges_.size() == 0)
	{
	  edges_.push_back(decorated_edge_value_t(h1, h2, v));
	  e = edges_.size() - 1;
	}
      else
	{
	  e = removed_edges_.front();
	  removed_edges_.pop_front();
	  assertion(e < edges_.size());
	  edges_[e].from() = h1;
	  edges_[e].to() = h2;
	}
      nodes_[h1].edges.push_back(e);
      nodes_[h2].edges.push_back(e);
      return e;
    }

    template <class NodeValue, class EdgeValue>
    void			
    heavy_graph<NodeValue, EdgeValue>::del_edge(hedge_t e)
    {
      precondition(hold(e));
      const edge_value_t& ev = edges_[e];
      ev->from.edges.remove(n);
      ev->to.edges.remove(n);
      removed_edges_.push_front(e);
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::hnode_t			
    heavy_graph<NodeValue, EdgeValue>::from(hedge_t e1) const
    {
      precondition(hold(e1));
      return edges_[e1].from();
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::hnode_t			
    heavy_graph<NodeValue, EdgeValue>::to(hedge_t e2) const
    {
      precondition(hold(e2));
      return edges_[e2].to();
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::node_value_t&		
    heavy_graph<NodeValue, EdgeValue>::operator[](hnode_t n)
    {
      precondition(hold(n));
      return nodes_[n];
    }

    template <class NodeValue, class EdgeValue>
    const typename heavy_graph<NodeValue, EdgeValue>::node_value_t&	
    heavy_graph<NodeValue, EdgeValue>::operator[](hnode_t n) const
    {
      precondition(hold(n));
      return nodes_[n];
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::edge_value_t&		
    heavy_graph<NodeValue, EdgeValue>::operator[](hedge_t n)
    {
      precondition(hold(n));
      return edges_[n];
    }

    template <class NodeValue, class EdgeValue>
    const typename heavy_graph<NodeValue, EdgeValue>::edge_value_t&	
    heavy_graph<NodeValue, EdgeValue>::operator[](hedge_t n) const
    {
      precondition(hold(n));
      return edges_[n];
    }

    template <class NodeValue, class EdgeValue>
    bool			
    heavy_graph<NodeValue, EdgeValue>::hold(hnode_t n) const
    {
      return 
	((std::find(removed_nodes_.begin(), removed_nodes_.end(), n) 
	 == removed_nodes_.end())
	&& (n < nodes_.size()));
    }

    template <class NodeValue, class EdgeValue>
    bool			
    heavy_graph<NodeValue, EdgeValue>::hold(hedge_t n) const
    {
      return 
	((std::find(removed_edges_.begin(), removed_edges_.end(), n) 
	 == removed_edges_.end())
	&& (n < edges_.size()));
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::self_t&			
    heavy_graph<NodeValue, EdgeValue>::clone() const
    {
      // not implemented yet.
      assert(0);
    }

    template <class NodeValue, class EdgeValue>
    std::string	
    heavy_graph<NodeValue, EdgeValue>::name()
    {
      return "heavy_graph";
    }

  } // end of internal.

} // end of oln.

#endif // CORE_INTERNAL_GRAPH_IMPL_HXX
