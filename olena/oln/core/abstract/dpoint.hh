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

#ifndef OLENA_CORE_ABSTRACT_DPOINT_HH
# define OLENA_CORE_ABSTRACT_DPOINT_HH

# include <mlc/type.hh>


namespace oln {

  namespace abstract {
    template<class Exact>
    struct dpoint; // fwd_decl
  } // end of abstract

  template<class Exact>
  struct dpoint_traits;

  template<class Exact>
  struct dpoint_traits<abstract::dpoint<Exact> >
  {

  };

  namespace abstract {
    template<class Exact>
    struct dpoint : public mlc::any<Exact>
    {
     
      typedef Exact exact_type;
      typedef dpoint<Exact> self_type;
      enum { dim = dpoint_traits<exact_type>::dim };
      typedef typename dpoint_traits<exact_type>::point_type point_type;

      explicit dpoint(const point_type& p)
      {
	
	for (unsigned i = 0; i < dim; ++i)
	  nth(i) = p.nth(i);
      }
      
      coord nth(const unsigned dim) const
      {
	return coord_[dim];
      }
      
      coord& nth(const unsigned dim)
      {
	return coord_[dim];
      }

      bool operator==(const self_type& p) const
      {
	for (unsigned i = 0; i < dim; ++i)
	  if (p.nth(i) != nth(i))
	    return false;
	return true;
      }
      
      bool operator!=(const self_type& p) const
      {
	for (unsigned i = 0; i < dim; ++i)
	  if (p.nth(i) != nth(i))
	    return true;
	return false;
      }
      
      bool is_centered(void) const
      {
	for (unsigned i = 0; i < dim; ++i)
	  if (nth(i) != 0)
	    return false;
	return true;
      }

      static std::string name() { return std::string("dpoint<") +
				    Exact::name() + ">"; }
    protected:
      dpoint() {}
    private:
      coord coord_[dim];
    };
    
  } // end of abstract

  namespace internal
  {
    template<class Exact>
    struct default_less< abstract::dpoint<Exact> >
    {
      bool operator()(const abstract::dpoint<Exact>& l,
		      const abstract::dpoint<Exact>& r) const
      {
	for (unsigned i = 0; i < abstract::dpoint<Exact>::dim; ++i)
	  if (l.nth(i) < r.nth(i))
	    return true;
	  else if (l.nth(i) > r.nth(i))
	    return false;
	return false;
      }
    };

  } // internal


} // end of oln


#endif // ! OLENA_CORE_ABSTRACT_DPOINT_HH
