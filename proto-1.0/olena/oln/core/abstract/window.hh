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

#ifndef OLENA_CORE_ABSTRACT_WINDOW_HH
# define OLENA_CORE_ABSTRACT_WINDOW_HH

# include <vector>
# include <utility>
# include <iostream>

# include <mlc/types.hh>
# include <oln/core/typedefs.hh>
# include <oln/core/coord.hh>
# include <oln/core/abstract/dpoint.hh>
# include <oln/core/accum.hh>


# define oln_wn_type_of(WindowType, Alias) \
mlc_type_of(oln, oln::category::window, WindowType, Alias)



namespace oln {

  namespace category
  {
    struct window;
  }

  template <>
  struct set_default_props < category::window >
  {
    typedef mlc::undefined_type dpoint_type;
    typedef mlc::undefined_type size_type;
    typedef mlc::undefined_type fwd_qiter_type;
  };

  template <typename W>
  struct get_props < category::window, W >
  {
    typedef oln_wn_type_of(W, dpoint)    dpoint_type;
    typedef oln_wn_type_of(W, size)      size_type;
    typedef oln_wn_type_of(W, fwd_qiter) fwd_qiter_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::window, " << mlc_to_string(W) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t dpoint_type    = " << mlc_to_string(dpoint_type)    << std::endl
	   << "\t size_type      = " << mlc_to_string(size_type)      << std::endl
	   << "\t fwd_qiter_type = " << mlc_to_string(fwd_qiter_type) << std::endl
	   << "}" << std::endl;
    }
  };



  namespace abstract {

    /*!
    ** FIXME: Structuring elements (set of dpoints).
    **
    ** FIXME: This abstract class defines several virtual methods for
    ** its subclasses. Its goal is to deal with a set of 'move'
    ** points.
    */
    template<class W>
    class window : public mlc::any<W>
    {

    public:

      typedef oln_wn_type_of(W, dpoint) dpoint_type;

      typedef W exact_type;

      static std::string
      name()
      {
	return std::string("window<") + exact_type::name() + ">";
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

      exact_type
      operator-() const
      {
	exact_type se(this->exact());

	se.sym();
	return se;
      }

      void
      sym()
      {
	this->exact().impl_sym();
      }

    protected:

      void
      impl_sym()
      {
	for (unsigned i = 0; i < this->card(); ++i)
	  dp_[i] = - dp_[i];
      }

      bool
      impl_has(const dpoint_type& dp) const
      {
	return std::find(dp_.begin(), dp_.end(), dp) != dp_.end();
      }

      exact_type&
      impl_add(const dpoint_type& dp)
      {
	if (!(impl_has(dp)))
	  this->dp_.push_back(dp);
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

      window() : dp_(), delta_(0)
      {};

      window(unsigned size) : dp_(), delta_(0)
      {
        dp_.reserve(size);
      };

      std::vector<dpoint_type> dp_;
      max_accumulator<coord_t> delta_;

    };
  } // end of abstract

} // end of oln

template<class W>
std::ostream&
operator<<(std::ostream& o, const oln::abstract::window<W>& se)
{
  unsigned c = se.card();
  o << "[";
  for (unsigned i = 0; i < c; ++i)
    o << se.dp(i);
  o << "]";
  return o;
}


#endif // ! OLENA_CORE_ABSTRACT_WINDOW_HH
