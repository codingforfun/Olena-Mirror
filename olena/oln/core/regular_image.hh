// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_REGULAR_IMAGE_HH
# define OLENA_CORE_REGULAR_IMAGE_HH

# include <oln/core/image.hh>
# include <oln/core/image_size.hh>
# include <oln/core/point.hh>
# include <oln/core/dpoint.hh>

namespace oln {

  template<unsigned Dim, // 1D, 2D, etc.
	   class Exact>
  struct regular_image : public image< Exact >
  {

    enum { dim = Dim };
    typedef typename point_for_dim<Dim>::ret      point;
    typedef typename dpoint_for_dim<Dim>::ret     dpoint;
    typedef typename image_size_for_dim<Dim>::ret image_size;

    static std::string name()
    {
      return
	std::string("regular_image<") + dim + ", " + Exact::name() + ">";
    }
  protected:
    regular_image() {}
  };

} // oln


#endif // OLENA_CORE_REGULAR_IMAGE_HH
