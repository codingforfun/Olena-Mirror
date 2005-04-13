// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_GRID_HH
# define OLENA_CORE_ABSTRACT_GRID_HH

# include <mlc/any.hh>
# include <oln/core/typedefs.hh>


# define oln_grd_type_of(GridType, Alias) \
mlc_type_of(oln, oln::category::grid, GridType, Alias)



namespace oln {


  namespace category
  {
    struct grid;
  }


  template <>
  struct set_default_props < category::grid >
  {
    typedef mlc::undefined_type point_type;
    typedef mlc::no_type        dpoint_type;
    typedef mlc::undefined_type size_type;
    typedef mlc::undefined_type coord_type;
    typedef mlc::undefined_type dimvalue_type;
  };


  template <typename G>
  struct get_props < category::grid, G >
  {
    typedef oln_grd_type_of(G, point)    point_type;
    typedef oln_grd_type_of(G, dpoint)   dpoint_type;
    typedef oln_grd_type_of(G, coord)    coord_type;
    typedef oln_grd_type_of(G, size)     size_type;
    typedef oln_grd_type_of(G, dimvalue) dimvalue_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::grid, " << mlc_to_string(G) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t point_type    = " << mlc_to_string(point_type)    << std::endl
	   << "\t dpoint_type   = " << mlc_to_string(dpoint_type)   << std::endl
	   << "\t coord_type    = " << mlc_to_string(coord_type)    << std::endl
	   << "\t size_type     = " << mlc_to_string(size_type)     << std::endl
	   << "\t dimvalue_type = " << mlc_to_string(dimvalue_type) << std::endl
	   << "}" << std::endl;
    }

    static void ensure()
    {
      mlc::is_ok< point_type >::ensure();
      mlc::is_ok< dpoint_type >::ensure();
      mlc::is_ok< coord_type >::ensure();
      mlc::is_ok< size_type >::ensure();
      mlc::is_ok< dimvalue_type >::ensure();
    }
  };


  namespace abstract {

    template <typename E>
    struct grid : public mlc::any<E>
    {
    protected:
      grid()
      {}

      ~grid()
      {
	get_props<category::grid, E>::ensure();
      }
    };

  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_GRID_HH
