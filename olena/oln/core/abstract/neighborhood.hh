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

#ifndef OLENA_CORE_ABSTRACT_NEIGHBORHOOD_HH
# define OLENA_CORE_ABSTRACT_NEIGHBORHOOD_HH

# include <mlc/type.hh>
# include <oln/core/abstract/dpoint.hh>
# include <oln/core/abstract/struct_elt.hh>

namespace oln 
{

  namespace abstract
  {
    template<class Exact>
    struct neighborhood; // fwd_decl
  } // end of abstract

  template<class Exact>
  struct struct_elt_traits<abstract::neighborhood<Exact> > 
  {
    typedef abstract::neighborhood<Exact> abstract_type;
  };

  namespace abstract 
  {

    template<class Exact>
    struct neighborhood : public mlc_hierarchy::any<Exact>
    {
      typedef Exact exact_type;
      typedef neighborhood<Exact> self_type;
      typedef typename struct_elt_traits<Exact>::iter_type iter_type;
      typedef typename struct_elt_traits<Exact>::neighb_type neighb_type;
      typedef typename struct_elt_traits<Exact>::win_type win_type;
      
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;
      typedef typename struct_elt_traits<Exact>::abstract_type abstract_type;
      enum { dim = struct_elt_traits<Exact>::dim };
 
      static std::string 
      name()
      {
	return std::string("neighborhood<") + Exact::name() + ">";
      }
      
      bool 
      has(const abstract::dpoint<dpoint_type>& dp) const
      {
	return this->exact().has_(dp.exact());
      }
      
      unsigned 
      card() const
      {
	return this->exact().card_();
      }
      
      bool 
      is_centered() const
      {
	return this->exact().is_centered_();
      }
      
      const dpoint_type 
      dp(unsigned i) const
      {
	return this->exact()[i];
      }

      bool 
      operator==(const self_type& win) const
      {
	return this->exact().is_equal(win.exact());
      }

      coord 
      delta() const
      {
	return this->exact().get_delta();
      }

      const dpoint_type 
      operator[](unsigned i) const
      {
	return this->exact().at(i);
      }

      exact_type& 
      add(const abstract::dpoint<dpoint_type>& dp)
      {
	this->exact().add_(dp.exact());
	return this->exact().add_(-dp.exact());
      }
      // obsolete
      exact_type 
      operator-() const
      {
	return this->exact();
      }

    protected:
      
      void 
      sym()
      {
	this->exact().sym_();
      }

      neighborhood() 
      {}
    };
   
  } // end of abstract  

    template<class E>
    inline E 
    inter(const abstract::neighborhood<E> &lhs, 
	  const abstract::neighborhood<E> &rhs)
    {
      E neighb;
      for (unsigned j = 0; j < rhs.card(); ++j)
	if (lhs.has(rhs.dp(j)) && ! neighb.has(rhs.dp(j)))
	  neighb.add(rhs.dp(j));
      return neighb;
    }
  
    template<class E>
    inline E 
    uni(const abstract::neighborhood<E> &lhs, 
	const abstract::neighborhood<E> &rhs)
    {
      E neighb;
      for (unsigned j = 0; j < rhs.card(); ++j)
	if (! neighb.has(rhs.dp(j)))
	  neighb.add(rhs.dp(j));
      for (unsigned j = 0; j < lhs.card(); ++j)
	if (! neighb.has(lhs.dp(j)))
	  neighb.add(lhs.dp(j));
      return neighb;
    }

  template<class E>
  inline typename struct_elt_traits<E>::win_type 
  mk_win_from_neighb(const abstract::neighborhood<E>& n)
  {
    typename struct_elt_traits<E>::win_type win(n.card());
    for (unsigned i = 0; i < n.card(); ++i)
      win.add(n.dp(i));
    return win;
  }

  
} // end of oln
  

#endif // OLENA_CORE_ABSTRACT_NEIGHBORHOOD_HH
