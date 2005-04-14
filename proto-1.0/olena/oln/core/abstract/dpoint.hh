// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

# include <mlc/any.hh>
# include <mlc/contract.hh>
# include <oln/core/coord.hh>
# include <oln/core/typedefs.hh>


/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {

  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {

    /*! \class abstract::dpoint<E>
    **
    ** The abstract::dpoint class is the base class from whom derives
    ** every concrete dpoint.  A dpoint is a difference between a couple
    ** of points.
    **
    ** Parameter E is the exact type of dpoint.
    */
    template <typename E>
    struct dpoint : public mlc::any<E>
    {
      typedef E exact_type;

      /// Test equality of two dpoints.
      bool operator==(const exact_type& rhs) const
      {
	return this->exact().impl_eq(rhs.exact());
      }

      /// Anteriority w.r.t. to a fwd image browsing.
      bool fwd_less(const exact_type& rhs) const
      {
	return this->exact().impl_fwd_less(rhs.exact());
      }
 
      /// Anteriority w.r.t. to a bkd image browsing.
      bool bkd_less(const exact_type& rhs) const
      {
	return ! *this == rhs && ! this->fwd_less(rhs);
      }
 
      const coord_t nth(unsigned i) const
      {
	precondition(i < this->dim());
	return this->exact().impl_nth(i);
      }

      coord_t& nth(unsigned i)
      {
	precondition(i < this->dim());
	return this->exact().impl_nth(i);
      }

      unsigned dim() const
      {
	return this->exact().impl_dim();
      }


      /// Test difference between two dpoints.
      bool operator!=(const exact_type& rhs) const
      {
	return ! this->operator==(rhs);
      }

      /// Unarity minus.
      const exact_type operator-() const
      {
	exact_type tmp;
	for (unsigned c = 0; c < this->dim(); ++c)
	  tmp.nth(c) = - this->nth(c);
	return tmp;
      }

    protected:

      dpoint() {}

      ~dpoint()
      {
	mlc_check_method_impl(E, bool,          eq,       const exact_type&, const);
	mlc_check_method_impl(E, bool,          fwd_less, const exact_type&, const);
	mlc_check_method_impl(E, const coord_t, nth,      unsigned,          const);
	mlc_check_method_impl(E, coord_t&,      nth,      unsigned,               );
	mlc_check_method_impl(E, unsigned,      dim,      ,                  const);
      }
    };


  } // end of namespace abstract


  struct fwd_less_dpoint
  {
    template <typename D>
    bool operator()(const abstract::dpoint<D>& lhs, const abstract::dpoint<D>& rhs) const
    {
      return lhs.fwd_less(rhs.exact());
    }
  };


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_DPOINT_HH
