// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_TOPO_INTER_PIXEL_INTERNAL_DIR_HH
# define OLENA_TOPO_INTER_PIXEL_INTERNAL_DIR_HH

# include <oln/config/system.hh>

# include <iostream>

namespace oln {

  namespace topo {

    namespace inter_pixel {
      /// \brief Internal purpose only.
      namespace internal {
	/// Provides the enum dir.
	template<unsigned Dim>
	struct dir_traits
	{};
	/// Provides the enum dir for 2D.
	template<>
	struct dir_traits<2>
	{
	  //// Direction in 2D.
	  typedef enum dir { east, north, west, south } ret;

	  /// First direction.
	  static ret
	  first()
	  {
	    return east;
	  }

	  /// Last direction.
	  static ret
	  last()
	  {
	    return south;
	  }
	  /// Prev direction (with Prev(first()) == last()).
	  static ret
	  prev(ret i)
	  {
	    return i == first() ? last() : ret(i - 1);
	  }

	  /// Next direction (with next(last()) == first()).
	  static ret
	  next(ret i)
	  {
	    return i == last() ? first() : ret(i + 1);
	  }

	  /*! Opposit direction.
	  **
	  ** \todo FIXME: no modulus.
	  */
	  static ret
	  opposite(ret i)
	  {
	    return ret((i + 2) % 4);
	  }
	};
/// Direction trait for an image \a ImgType.
# define oln_dir_traits_type(ImgType)				\
oln::topo::inter_pixel::internal::dir_traits< ImgType::dim >

/// Direction for an image \a ImgType.
# define oln_dir_type(ImgType)			\
typename oln_dir_traits_type(ImgType)::ret

      } // end of namespace internal

    } // end of namespace inter_pixel

  } // end of namespace topo

} // end of namespace oln

std::ostream & operator<<(std::ostream& o,
			  oln::topo::inter_pixel::internal::dir_traits<2>::ret i)
{
  typedef oln::topo::inter_pixel::internal::dir_traits<2> traits;

  switch (i)
    {
    case traits::east: return o << "east";
    case traits::north: return o << "north";
    case traits::west: return o << "west";
    case traits::south: return o << "south";
    default: return o;
    }
}

#endif // !OLENA_TOPO_INTER_PIXEL_INTERNAL_DIR_HH
