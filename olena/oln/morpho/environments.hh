// Copyright (C)  2004  EPITA Research and Development Laboratory
//
// This  file is  part of  the Olena  Library.  This  library  is free
// software; you can redistribute it  and/or modify it under the terms
// of the  GNU General  Public License version  2 as published  by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with  this library; see the  file COPYING.  If  not, write to
// the Free Software Foundation, 59  Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a  special exception, you  may use this  file as part of  a free
// software library without restriction.  Specifically, if other files
// instantiate templates  or use macros or inline  functions from this
// file, or  you compile  this file  and link it  with other  files to
// produce  an executable,  this file  does  not by  itself cause  the
// resulting  executable  to be  covered  by  the  GNU General  Public
// License.   This exception  does  not however  invalidate any  other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_MORPHO_ENVIRONMENTS_HH
# define OLN_MORPHO_ENVIRONMENTS_HH
# include <oln/basics.hh>
# include <mlc/type.hh>

namespace oln
{
  namespace morpho
  {
    /*! \brief Implementation of environments used by attributes.
     */
    namespace env
    {
      /*! brief Abstract stuff for environments.
       */
      namespace abstract
      {
	/*!
	** \brief Top of environment hierarchy.
	*/
	template <class Exact>
	struct env: public mlc_hierarchy::any<Exact>
	{
	};
      } // !abstract

      /*!
      ** \brief Useless environment.
      **
      ** This environment is an empty one.
      */
      struct NullEnv: public abstract::env<NullEnv>
      {
      };

      /*!
      ** \brief Environment containing image.
      **
      ** Used for image substitution in other_image attribute.
      */
      template <class I>
      struct OtherImageEnv: public abstract::env<OtherImageEnv<I> >
      {
	typedef oln::abstract::image<I>	im_type;

	OtherImageEnv(const oln::abstract::image<I> &im): im_(im)
	{};

	const im_type	&getImage() const
	{
	  return im_;
	}

      protected:
	const im_type	&im_;
      };

      /*!
      ** \brief Environment containing point.
      **
      ** Used for point substitution in other_point attribute.
      */
      template <class I>
      struct ParentEnv: public abstract::env<ParentEnv<I> >
      {
	typedef oln::abstract::image<I>	im_type;

	ParentEnv(const oln::abstract::image<I> &im): im_(im)
	{};

	const im_type	&getParent() const
	{
	  return im_;
	}

      protected:
	const im_type	&im_;
      };

    }
  }
}
#endif // !OLN_MORPHO_ENVIRONMENTS_HH
