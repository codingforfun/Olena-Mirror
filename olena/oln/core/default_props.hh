// Copyright (C) 2004  EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_DEFAULT_PROPS
# define OLN_CORE_DEFAULT_PROPS
# include <oln/core/macros.hh>

namespace oln {

  template <class Dim>
  struct default_props
  {
    //  enum { linbuf_value = oln_linbuf_value(Dim) };
    enum { dim_value = oln_dim_val(Dim) };

    typedef Dim				dim_type;
    //    typedef oln_image_size_type(Dim) image_size_type;

    typedef oln_point_type(Dim)		point_type;
    typedef oln_dpoint_type(Dim)	dpoint_type;

    //    typedef oln_iter_type(Dim) iter_type;
    //    typedef oln_fwd_iter_type(Dim) fwd_iter_type;
    //typedef oln_bkd_iter_type(Dim) bkd_iter_type;

    typedef oln_neighborhood_type(Dim)	neighborhood_type;
    typedef oln_window_type(Dim)	window_type;
    typedef oln_flat_se_type(Dim)	flat_se_type;

    typedef oln_concrete_type(Dim)	concrete_type;
 //    typedef oln_niter_type(Dim) niter_type;
//     typedef oln_fwd_niter_type(Dim) fwd_niter_type;
//
    // typedef oln_bkd_niter_type(Dim) bkd_niter_type;
  };
} // !oln

#endif // OLN_CORE_DEFAULT_PROPS
