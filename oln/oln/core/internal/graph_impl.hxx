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
    handler<Tag, T>::operator T() const
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
      EdgeValue(v),
      from_(h1),
      to_(h2)
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
    heavy_graph<NodeValue, EdgeValue>::heavy_graph()
    {
    }

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
    const typename heavy_graph<NodeValue, EdgeValue>::neighbors_of_node_t&
    heavy_graph<NodeValue, EdgeValue>::neighbors_of(hnode_t n) const
    {
      precondition(hold(n));
      return nodes_[n].neighbors;
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
    hedge_of(edges_set_const_iterator i) const
    {
      precondition(i != edges_.end());
      return i - edges_.begin();
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::hnode_t
    heavy_graph<NodeValue, EdgeValue>::
    hnode_of(nodes_set_const_iterator i) const
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
	  nodes_.push_back(decorated_node_value_t(t));
	  return nodes_.size() - 1;
	}
      hnode_t n = *removed_nodes_.begin();
      removed_nodes_.erase(n);
      assertion(n < int(nodes_.size()));
      nodes_[n] = t;
      nodes_[n].edges.clear();
      nodes_[n].neighbors.clear();
      postcondition(hold(n));
      return n;
    }

    template <class NodeValue, class EdgeValue>
    void			
    heavy_graph<NodeValue, EdgeValue>::del_node(hnode_t n)
    {
      if (!hold(n))
	return;
      const decorated_node_value_t& v = nodes_[n];
      edges_of_node_set_t tmp = v.edges;
      for (typename edges_of_node_set_t::const_iterator e = tmp.begin();
	   e != tmp.end();
	   ++e)
	this->del_edge(*e);
      removed_nodes_.insert(n);
      postcondition(! hold(n));
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
	  e = *removed_edges_.begin();
	  removed_edges_.erase(e);
	  assertion(e < int(edges_.size()));
	  edges_[e].from() = h1;
	  edges_[e].to() = h2;
	}
      nodes_[h1].edges.insert(e);
      nodes_[h2].edges.insert(e);
      nodes_[h1].neighbors.insert(h2);
      nodes_[h2].neighbors.insert(h1);
      postcondition(hold(e));
      return e;
    }

    template <class NodeValue, class EdgeValue>
    void			
    heavy_graph<NodeValue, EdgeValue>::del_edge(hedge_t e)
    {
      if (!hold(e))
	return;
      precondition(hold(e));
      const decorated_edge_value_t& ev = edges_[e];
      nodes_[ev.from()].edges.erase(e);
      nodes_[ev.to()].edges.erase(e);
      nodes_[ev.to()].neighbors.erase(ev.from());
      nodes_[ev.from()].neighbors.erase(ev.to());
      removed_edges_.insert(e);
      postcondition(! hold(e));
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
    typename heavy_graph<NodeValue, EdgeValue>::hnode_t			
    heavy_graph<NodeValue, EdgeValue>::from(edges_set_const_iterator e1) const
    {
      precondition(hold(hedge_of(e1)));
      return e1->from();
    }

    template <class NodeValue, class EdgeValue>
    typename heavy_graph<NodeValue, EdgeValue>::hnode_t			
    heavy_graph<NodeValue, EdgeValue>::to(edges_set_const_iterator e2) const
    {
      precondition(hold(hedge_of(e2)));
      return e2->to();
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
      bool ret = (removed_nodes_.find(n) == removed_nodes_.end()) 
	&& (n < int(nodes_.size()));
      if (ret == false)
	{
	  for (int i = 0; i < int(edges_.size()); ++i)
	    if (removed_edges_.find(hedge_t(i)) == removed_edges_.end())
	      postcondition (((edges_[i].from() != n) &&
			      (edges_[i].to() != n)));
		
	}
      return ret;
    }

    template <class NodeValue, class EdgeValue>
    bool			
    heavy_graph<NodeValue, EdgeValue>::hold(hedge_t n) const
    {
      bool ret =  
	((std::find(removed_edges_.begin(), removed_edges_.end(), n) 
	 == removed_edges_.end())
	&& (n < int(edges_.size())));
      if (ret == false)
	{
	  for (int i = 0; i < int(nodes_.size()); ++i)
	    if (std::find(removed_nodes_.begin(), removed_nodes_.end(), 
			  hnode_t(i)) 
	     == removed_nodes_.end())
	      postcondition(std::find(nodes_[i].edges.begin(), 
				      nodes_[i].edges.end(),
				      n) ==
			    nodes_[i].edges.end());
	}
      return ret;
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
