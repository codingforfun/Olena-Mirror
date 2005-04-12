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
    struct window : public mlc::any<W>
    {
    protected:
      window()
      {}
    };


  } // end of namespace oln::abstract

} // end of namespace oln



#endif // ! OLENA_CORE_ABSTRACT_WINDOW_HH
