// Copyright (C) 2001, 2002, 2003, 2004 EPITA Research and Development
// Laboratory
//
// This  file is  part of  the Olena  Library.  This  library  is free
// software; you can redistribute it  and/or modify it under the terms
// of the  GNU General  Public License version  2 as published  by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with  this library; see the  file COPYING.  If  not, write to
// the Free Software Foundation, 59  Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a  special exception, you  may use this  file as part of  a free
// software library without restriction.  Specifically, if other files
// instantiate templates  or use macros or inline  functions from this
// file, or  you compile  this file  and link it  with other  files to
// produce  an executable,  this file  does  not by  itself cause  the
// resulting  executable  to be  covered  by  the  GNU General  Public
// License.   This exception  does  not however  invalidate any  other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_CORE_ABSTRACT_NEIGHBORHOOD_HH
# define OLN_CORE_ABSTRACT_NEIGHBORHOOD_HH

#include <oln/core/abstract/dpoint_set.hh>
#include <oln/core/abstract/flat_se.hh>

namespace oln {

  namespace abstract {

    template <typename E>
    struct neighborhood: public dpoint_set<E>
    {
      /*!
      ** \brief Add a point to the neighborhood.
      ** \arg dp The new point.
      ** \pre !dp.is_centered().
      **
      ** Add a new member to the neighborhood.
      */
      E& add_impl(const oln_dpoint_type(E)& dp)
      {
	precondition(!dp.is_centered());
	this->sdps_.insert(dp);
	this->sdps_.insert(-dp);
	this->change_ = true;
	return this->exact();
      }

      const oln_flat_se_type(E) flat_se() const
      {
	oln_flat_se_type(E) win;
	win.clone(*this);
	return win;
      }

      // backward compatibility
      const oln_window_type(E) window() const
      {
	return flat_se();
      }

    protected:
      neighborhood() {}
    };

  } // end of namespace abstract

} // end of namespace oln


#endif // !OLN_CORE_ABSTRACT_NEIGHBORHOOD_HH
