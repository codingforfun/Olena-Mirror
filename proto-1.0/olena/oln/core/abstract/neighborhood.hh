// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_NEIGHBORHOOD_HH
# define OLENA_CORE_NEIGHBORHOOD_HH

# include <mlc/types.hh>
# include <oln/core/properties.hh>
# include <oln/core/coord.hh>
# include <oln/core/abstract/dpoint.hh>
# include <vector>
# include <utility>
# include <iostream>

namespace oln {

  // fwd decl
  namespace abstract {
    template<class Exact>
    struct neighborhood;
  }

  // category
  template <typename E>
  struct set_category< abstract::neighborhood<E> > 
  {
    typedef category::neighborhood ret;
  };

  /// properties of any type in category::neighborhood

  template <typename type>
  struct props_of <category::neighborhood, type>
  {
    typedef mlc::true_type user_defined_;

    mlc_decl_prop(category::neighborhood, dpoint_type);
    mlc_decl_prop(category::neighborhood, size_type);

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( category::neighborhood, "
	   << typeid(type).name() << ") = {"
	   << "  size_type = " << typeid(size_type).name() << "," << std::endl
	   << "  dpoint_type = " << typeid(dpoint_type).name() << "  }" << std::endl;
    }
 
  };

  mlc_register_prop(category::neighborhood, dpoint_type);
  mlc_register_prop(category::neighborhood, size_type);


  namespace abstract {

    /*!
    ** Structuring elements (set of dpoints).
    **
    ** This abstract class defines several virtual methods for its
    ** subclasses. Its goal is to deal with a set of 'move' points.
    */
    template<class E>
    class neighborhood : public mlc::any__best_memory<E>
    {

    public:

      typedef oln_type_of(E, dpoint) dpoint_type;

      typedef E exact_type;

      static std::string
      name()
      {
	return std::string("neighborhood<") + exact_type::name() + ">";
      }

      bool
      has(const dpoint_type& dp) const
      {
	return this->exact().impl_has(dp.exact());
      }

      unsigned
      card() const
      {
	return this->exact().impl_card();
      }

      exact_type&
      add(const dpoint_type& dp)
      {
        return this->exact().impl_add(dp);
      }

      dpoint_type
      dp(unsigned i) const
      {
	return this->exact().impl_at(i);
      }

      const dpoint_type
      operator[](unsigned i) const
      {
	return this->exact().impl_at(i);
      }

    protected:

      bool
      impl_has(const dpoint_type& dp) const
      {
	return std::find(dp_.begin(), dp_.end(), dp) != dp_.end();
      } 
  
      exact_type&
      impl_add(const dpoint_type& dp)
      {
	if (!(impl_has(dp)))
        {
	  this->dp_.push_back(dp);
	  this->dp_.push_back(-dp);
        }
	return this->exact();
      }

      unsigned
      impl_card() const
      {
	return dp_.size();
      } 

      const dpoint_type
      impl_at(unsigned i) const
      {
	precondition(i < this->card());
	return dp_[i];
      } 

      neighborhood() : dp_()
      {};

      neighborhood(unsigned size) : dp_()
      {
        dp_.reserve(size);
      };

      std::vector<dpoint_type> dp_;

    };
  } // end of abstract

} // end of oln

template<class E>
std::ostream&
operator<<(std::ostream& o, const oln::abstract::neighborhood<E>& se)
{
  unsigned c = se.card();
  o << "[";
  for (unsigned i = 0; i < c; ++i)
    o << se.dp(i);
  o << "]";
  return o;
}
 

#endif // ! OLENA_CORE_NEIGHBORHOOD_HH
