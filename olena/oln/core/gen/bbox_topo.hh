// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_GEN_BBOX_TOPO_HH
# define OLENA_CORE_GEN_BBOX_TOPO_HH

# include <oln/core/topo_entry.hh>
# include <oln/core/gen/bbox.hh>



namespace oln
{


  // Forward declarations.
  template <typename point> class bbox_topo_;


  // Super type declaration.
  template <typename point>
  struct set_super_type< bbox_topo_<point> >
  {
    typedef bbox_topo_<point> self_t;
    typedef topo_entry<self_t> ret;
  };


  /// Virtual types associated to oln::bbox_<point>.
  template <typename point>
  struct vtypes< bbox_topo_<point> >
  {
    typedef bbox_<point> bbox_type;
    typedef point        point_type;
    typedef mlc::true_   ra_type;
  };


  /// Bounding box topology based on a point class.
  template <typename point>
  class bbox_topo_ : public topo_entry< bbox_topo_<point> >
  {
    typedef bbox_<point> bbox_t;

  public:

    bbox_topo_()
    {
    }

    bbox_topo_(const bbox_t& bb)
      : bb_(bb)
    {
    }

    const bbox_t& impl_bbox() const
    {
      return bb_;
    }

  protected:

    bbox_<point> bb_;

  };


} // end of namespace oln


#endif // ! OLENA_CORE_GEN_BBOX_TOPO_HH
