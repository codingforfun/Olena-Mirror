// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
///
/// \brief 2D neighborhood working in K1.

#ifndef MLN_WORLD_K1_NEIGHB2D_HH
# define MLN_WORLD_K1_NEIGHB2D_HH

# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/win/multiple.hh>
# include <mln/make/double_neighb2d.hh>

namespace mln
{

  namespace world
  {

    namespace k1
    {

      typedef neighb< win::multiple<window2d, bool(*)(const point2d&)> > dbl_neighb2d;

      /*! \brief 2 faces to 1 faces neighborhood.

	\verbatim
	 -
	|x|
	 -
	 \endverbatim
       */
      const neighb2d& faces_2to1();


      /*! \brief 1 faces to 2 faces neighborhood.

	\verbatim
	         o
	oxo  or  x
	         o
	 \endverbatim

       */
      const dbl_neighb2d& faces_1to2();


      /*! \brief 1 faces to 1 faces neighborhood.

	\verbatim
          |
        -   -           |  |
	  x       or  -      -
        -   -           |  |
	  |
	 \endverbatim
       */
      const dbl_neighb2d& faces_1to1();


      /*! \brief 2 faces to 2 faces neighborhood (4-connectivity).

	\verbatim
	  x
	  -
	x|o|x
	  -
	  x
	 \endverbatim
       */
      const neighb2d& faces_2to2_c4();


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	inline
	bool is_row_odd(const point2d& p)
	{
	  return p.row() % 2;
	}

      }


      const neighb2d& faces_2to1()
      {
	return c4();
      }

      const dbl_neighb2d& faces_1to2()
      {
	static bool face1_h[] = { 0, 1, 0,
				  0, 0, 0,
				  0, 1, 0 };
	static bool face1_v[] = { 0, 0, 0,
				  1, 0, 1,
				  0, 0, 0 };
	static dbl_neighb2d
	  nbh = make::double_neighb2d(internal::is_row_odd, face1_h, face1_v);
	return nbh;
      }


      const dbl_neighb2d& faces_1to1()
      {
	static bool e2e_h[] = { 0, 0, 1, 0, 0,
				0, 1, 0, 1, 0,
				0, 0, 0, 0, 0,
				0, 1, 0, 1, 0,
				0, 0, 1, 0, 0 };
	static bool e2e_v[] = { 0, 0, 0, 0, 0,
				0, 1, 0, 1, 0,
				1, 0, 0, 0, 1,
				0, 1, 0, 1, 0,
				0, 0, 0, 0, 0 };
	static dbl_neighb2d nbh = make::double_neighb2d(internal::is_row_odd, e2e_h, e2e_v);
	return nbh;
      }

      const neighb2d& faces_2to2_c4()
      {
	static neighb2d it;
	if (it.size() == 0)
	{
	  static bool vals[] = { 0, 0, 1, 0, 0,
				0, 0, 0, 0, 0,
				1, 0, 0, 0, 1,
				0, 0, 0, 0, 0,
				0, 0, 1, 0, 0 };
	  convert::from_to(vals, it);
	}
	return it;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k1

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K1_NEIGHB2D_HH
