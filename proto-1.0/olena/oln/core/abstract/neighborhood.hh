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

# include <oln/core/abstract/any.hh>
# include <oln/core/typedefs.hh>


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

    typedef mlc::undefined_type iter_type;
    typedef mlc::undefined_type fwd_iter_type;
    typedef mlc::undefined_type bkd_iter_type;
  };


  /// Retrieval of any neighborhood type properties.

  template <typename N>
  struct get_props < category::neighborhood, N >
  {
    typedef oln_nbh_type_of(N, dpoint) dpoint_type;
    typedef oln_nbh_type_of(N, size)   size_type;
    typedef oln_nbh_type_of(N, window) window_type;
    typedef oln_nbh_type_of(N, iter)     iter_type;
    typedef oln_nbh_type_of(N, fwd_iter) fwd_iter_type;
    typedef oln_nbh_type_of(N, bkd_iter) bkd_iter_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::neighborhood, " << mlc_to_string(N) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t dpoint_type   = " << mlc_to_string(dpoint_type)   << std::endl
	   << "\t size_type     = " << mlc_to_string(size_type)     << std::endl
	   << "\t window_type   = " << mlc_to_string(window_type)   << std::endl
	   << "\t iter_type     = " << mlc_to_string(iter_type)     << std::endl
	   << "\t fwd_iter_type = " << mlc_to_string(fwd_iter_type) << std::endl
	   << "\t bkd_iter_type = " << mlc_to_string(bkd_iter_type) << std::endl
	   << "}" << std::endl;
    }

    static void ensure()
    {
      mlc::is_ok< dpoint_type >::ensure();
      mlc::is_ok< size_type >::ensure();
      mlc::is_ok< window_type >::ensure();
      mlc::is_ok< iter_type >::ensure();
      mlc::is_ok< fwd_iter_type >::ensure();
      mlc::is_ok< bkd_iter_type >::ensure();
    }
  };




  namespace abstract {

    /*!
    ** Class for neighborhoods.
    **
    */
    template <typename E>
    struct neighborhood : public oln::abstract::any<E>
    {

    protected:
      
      neighborhood()
      {}

      ~neighborhood()
      {
	get_props<category::neighborhood, E>::ensure();
      }

    };

  } // end of namespace abstract::oln

} // end of namespace oln


# include <oln/core/abstract/niter.hh>


#endif // ! OLENA_CORE_NEIGHBORHOOD_HH
