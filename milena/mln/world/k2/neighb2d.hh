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
/// \brief 2D neighborhood working in K2.

#ifndef MLN_WORLD_K2_NEIGHB2D_HH
# define MLN_WORLD_K2_NEIGHB2D_HH

# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/win/multiple.hh>
# include <mln/make/double_neighb2d.hh>

namespace mln
{

  namespace world
  {

    namespace k2
    {

      /*! \brief Non-primary to primary faces neighborhood.

	\verbatim
	o| |o
	-.-.-
	 |x|
	-.-.-
	o| |o
	 \endverbatim
       */
      const neighb2d& cross_np2p_faces();

      /*! \brief Non-primary to primary faces neighborhood.

	\verbatim
	|o|
	.-.
	|x|
	.-.
	|o|
	 \endverbatim
       */
      const neighb2d& vertical_np2p_faces();

      /*! \brief Non-primary to primary faces neighborhood.

	\verbatim
	.-.-.-.
	|o|x|o|
	.-.-.-.
	 \endverbatim
       */
      const neighb2d& horizontal_np2p_faces();


# ifndef MLN_INCLUDE_ONLY

      const neighb2d& cross_np2p_faces()
      {
	static neighb2d it;
	if (it.size() == 0)
	{
	  static bool vals[] = { 1, 0, 0, 0, 1,
				 0, 0, 0, 0, 0,
				 0, 0, X, 0, 0,
				 0, 0, 0, 0, 0,
				 1, 0, 0, 0, 1 };
	  convert::from_to(vals, it);
	}
	return it;
      }


      const neighb2d& vertical_np2p_faces()
      {
	static neighb2d it;
	if (it.size() == 0)
	{
	  static bool vals[] = { 0, 0, 1, 0, 0,
				 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0,
				 0, 0, 1, 0, 0 };
	  convert::from_to(vals, it);
	}
	return it;
      }


      const neighb2d& horizontal_np2p_faces()
      {
	static neighb2d it;
	if (it.size() == 0)
	{
	  static bool vals[] = { 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0,
				 1, 0, 0, 0, 1,
				 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0 };
	  convert::from_to(vals, it);
	}
	return it;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k2

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K2_NEIGHB2D_HH
