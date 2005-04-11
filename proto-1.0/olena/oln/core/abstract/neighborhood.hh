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

# include <vector>
# include <utility>
# include <iostream>

# include <oln/core/typedefs.hh>
# include <oln/core/coord.hh>
# include <oln/core/abstract/dpoint.hh>
# include <oln/core/accum.hh>


# define oln_nbh_type_of(NeighborhoodType, Alias)  \
mlc_type_of(oln, oln::category::neighborhood, NeighborhoodType, Alias)


namespace oln {

  namespace category
  {
    struct neighborhood;
  }

  /// Default properties of any neighborhood type.

  template <>
  struct set_default_props < category::neighborhood >
  {
    typedef mlc::undefined_type dpoint_type;
    typedef mlc::undefined_type size_type;
    typedef mlc::undefined_type window_type;
  };


  /// Retrieval of any image type properties (FIXME: say 'packing').

  template <typename N>
  struct get_props < category::neighborhood, N >
  {
    typedef oln_nbh_type_of(N, dpoint) dpoint_type;
    typedef oln_nbh_type_of(N, size)   size_type;
    typedef oln_nbh_type_of(N, window) window_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::neighborhood, " << mlc_to_string(N) << " ) =" << std::endl
	   << "\t dpoint_type = " << mlc_to_string(dpoint_type) << std::endl
	   << "\t size_type   = " << mlc_to_string(size_type)   << std::endl
	   << "\t window_type = " << mlc_to_string(window_type) << std::endl
	   << std::endl;
    }

  };




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

      typedef oln_nbh_type_of(E, dpoint) dpoint_type;

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

      coord_t
      get_delta() const
      {
	return this->exact().impl_get_delta();
      }

      coord_t
      delta_update(const dpoint_type& dp)
      {
	return this->exact().impl_delta_update(dp);
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
        this->delta_update(dp);
	return this->exact();
      }

      coord_t
      impl_get_delta() const
      {
        return delta_;
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

      neighborhood() : dp_(), delta_(0)
      {};

      neighborhood(unsigned size) : dp_(), delta_(0)
      {
        dp_.reserve(size);
      };

      std::vector<dpoint_type> dp_;
      max_accumulator<coord_t> delta_;

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
