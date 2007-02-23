// Copyright (C) 2007 EPITA Research and Development Laboratory.
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef PLUG_HH_
# define PLUG_HH_

# include "../local/scoop.hh"

# include <mlc/flags.hh>
# include <mlc/typedef.hh>
# include <mlc/ret.hh>
# include <mlc/assert.hh>
# include <mlc/abort.hh>
# include <mlc/bool.hh>
# include <mlc/pair.hh>
# include <mlc/cmp.hh>
# include <mlc/if.hh>
# include <mlc/is_a.hh>
# include <mlc/case.hh>

# include "forward.hh"

namespace oln
{

  namespace internal
  {
    // first selector  -> Image kind
    typedef selector<Image, 1> Image_dim;
    typedef selector<Image, 2> Image_Mut;
    typedef selector<Image, 3> Image_Access;

    template <typename E>
    struct case_< Image_dim, E,  1 > : where_< mlc_eq(stc_type(E, grid), grid2d) >
    {
      typedef Image2d<E> ret;
    };

    template <typename E>
    struct case_< Image_dim, E,  2 > : where_< mlc_eq(stc_type(E, grid), grid1d) >
    {
      typedef Signal<E> ret;
    };

    template <typename Exact>
    struct case_< Image_Mut, Exact,  1 > : where_< stc_type_is_found(lvalue) >
    {
      typedef Mutable_Image<Exact> ret;
    };

    template <typename Exact>
    struct case_< Image_Access, Exact,  1 > : where_< mlc_eq(stc_type(Exact, psite), stc_type(Exact, psite)) >
    {
      typedef Point_Wise_Accessible_Image<Exact> ret;
    };
  }
}

#endif /* !PLUG_HH_ */
