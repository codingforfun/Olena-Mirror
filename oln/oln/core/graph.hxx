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

#ifndef OLENA_CORE_GRAPH_HXX
# define OLENA_CORE_GRAPH_HXX

# include <oln/core/contract.hh>
# include <oln/meta/basics.hh>

namespace oln {

  template <class U>
  const U&
  graph<U>::self() const
  {
    return static_cast<const U&>(*this);
  }

  template <class U>
  U&
  graph<U>::self()
  {
    return static_cast<U&>(*this);
  }
  
  template <class U>
  const typename graph<U>::nodes_set_t&
  graph<U>::nodes() const
  {
    return self().nodes();
  }

  template <class U>
  const typename graph<U>::edges_set_t&
  graph<U>::edges() const
  {
    return self().edges();
  }

  template <class U>
  typename graph<U>::nodes_set_t&
  graph<U>::nodes() 
  {
    return self().nodes();
  }

  template <class U>
  typename graph<U>::edges_set_t&
  graph<U>::edges() 
  {
    return self().edges();
  }

  template <class U>
  typename graph<U>::hedge_t
  graph<U>::hedge_of(edges_set_const_iterator i) const
  {
    return self().handler_of(i);
  }

  template <class U>
  typename graph<U>::hnode_t
  graph<U>::hnode_of(nodes_set_const_iterator i) const
  {
    return self().handler_of(i);
  }

  template <class U>
  typename graph<U>::hnode_t  
  graph<U>::add_node(const node_value_t& t)
  {
    return self().add_node(t);
  }

  template <class U>
  void  graph<U>::del_node(hnode_t n)
  {
    return self().del_node(n);
  }

  template <class U>
  typename graph<U>::hedge_t  graph<U>::add_edge(hnode_t h1, hnode_t h2,
						 const edge_value_t& ev)
  {
    return self().add_edge(h1, h2, ev);
  }

  template <class U>
  void	   graph<U>::del_edge(hedge_t e)
  {
    return self().del_edge(e);
  }

  template <class U>
  typename graph<U>::hnode_t  graph<U>::from(hedge_t e1) const
  {
    return self().from(e1);
  }

  template <class U>
  typename graph<U>::hnode_t  graph<U>::to(hedge_t e2) const
  {
    return self().to(e1);
  }

  template <class U>
  typename graph<U>::node_value_t&  graph<U>::operator[](hnode_t n)
  {
    return self()[n];
  }
  
  template <class U>
  const typename graph<U>::node_value_t& graph<U>::operator[](hnode_t n) const
  {
    return self()[n];
  }

  template <class U>
  typename graph<U>::edge_value_t&  graph<U>::operator[](hedge_t n)
  {
    return self()[n];
  }
  
  template <class U>
  const typename graph<U>::edge_value_t& graph<U>::operator[](hedge_t n) const
  {
    return self()[n];
  }

  template <class U>
  bool     graph<U>::hold(hnode_t n) const
  {
    return self().hold(n);
  }

  template <class U>
  bool     graph<U>::hold(hedge_t n) const
  {
    return self().hold(n);
  }

  template <class U>
  graph<U>& graph<U>::clone() const
  {
    return self().clone();
  }

  template <class U>
  std::string graph<U>::name()
  {
    return std::string("graph<") +
      U::name() + ">";
  }

  template <class U>
  graph<U>::graph<U>()
  {}

} // end of oln.

#endif // OLENA_CORE_GRAPH_HXX
