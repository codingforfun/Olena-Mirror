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

#ifndef OLENA_FANCY_PRINT_HH
# define OLENA_FANCY_PRINT_HH


# include <iostream>
# include <string>


# include <ntg/real/int_u.hh> // FIXME: no coupling like that!

# include <oln/core/abstract/image_dimension.hh>
# include <oln/core/1d/point1d.hh>
# include <oln/core/2d/point2d.hh>
# include <oln/core/3d/point3d.hh>
# include <oln/core/coord.hh>


namespace oln {

  namespace fancy {



    // fwd decls

    namespace impl {

      template <typename E>
      void print(const abstract::image1d<E>& input, std::ostream& ostr);
      template <typename E>
      void print(const abstract::image2d<E>& input, std::ostream& ostr);
      template <typename E>
      void print(const abstract::image3d<E>& input, std::ostream& ostr);

    } // end of namespace impl



    // facades

    template <typename I>
    void print(const abstract::image<I>& input,
	       std::ostream& ostr = std::cout)
    {
      impl::print(input.exact(), ostr);
    }

    template <typename I>
    void println(const abstract::image<I>& input,
		 std::ostream& ostr = std::cout)
    {
      print(input, ostr);
      ostr << std::endl;
    }


    // impls

    namespace impl {

      namespace internal {

	// FIXME: awful

	template <typename T>
	struct pp_type { typedef T ret; };

	template <typename T>
	const typename pp_type<T>::ret pp(const T& t) { return t; }

	template <> struct pp_type <  ntg::int_u8 > { typedef unsigned int ret; };
	template <> struct pp_type <unsigned char > { typedef unsigned int ret; };
	template <> struct pp_type <  signed char > { typedef   signed int ret; };
	template <> struct pp_type <         char*> { typedef  std::string ret; };

      }

      template <typename E>
      void print(const abstract::image1d<E>& input, std::ostream& ostr)
      {
	for (coord_t index = 0; index < input.size().nindices(); ++index)
	  ostr << internal::pp<oln_type_of(E, value)>(input[point1d(index)])
	       << ' ';
	ostr << std::endl;
      }

      template <typename E>
      void print(const abstract::image2d<E>& input, std::ostream& ostr)
      {
	for (coord_t row = 0; row < input.size().nrows(); ++row)
	  {
	    for (coord_t col = 0; col < input.size().ncols(); ++col)
	      ostr << internal::pp<oln_type_of(E, value)>(input[point2d(row,col)]) << ' ';
	    ostr << std::endl;
	  }
      }


      template <typename E>
      void print(const abstract::image3d<E>& input, std::ostream& ostr)
      {
	for (coord_t slice = 0; slice < input.size().nslices(); ++slice)
        {
	  for (coord_t row = 0; row < input.size().nrows(); ++row)
	  {
	    for (coord_t col = 0; col < input.size().ncols(); ++col)
	      ostr << internal::pp<oln_type_of(E, value)>(input[point3d(slice,row,col)])
                   << ' ';
            ostr << std::endl;
	  }
	  ostr << "---" << std::endl;
	}
      }


    } // end of namespace impl


  } // end of namespace fancy


} // end of namespace oln


#endif // ! OLENA_FANCY_PRINT_HH
