// Copyright (C) 2001, 2003, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_FWD_WITER2D_HH
# define OLENA_CORE_FWD_WITER2D_HH

# include <oln/core/abstract/witer.hh>
# include <oln/core/2d/window2d.hh>

# include <string>

namespace oln {

  // fwd decl
  struct fwd_witer2d;

  // category
  template <>
  struct set_category<fwd_witer2d> { typedef category::witer ret; };

  // props
  template <>
  struct set_props < category::witer, fwd_witer2d > : public props_of<category::witer>
  {
    typedef window2d se_type;
  };
 
  struct fwd_witer2d : public abstract::witer< fwd_witer2d >
  {

    typedef abstract::witer<fwd_witer2d> super_type;

    fwd_witer2d(const se_type& se) :
        super_type(se)
    {
      this->exact_ptr = this;
      this->invalidate();
    }

  };

} // oln

#endif // OLENA_CORE_FWD_WITER2D_HH
