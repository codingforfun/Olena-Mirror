// Copyright (C) 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_BEHAVIOR_HH
# define OLENA_CORE_ABSTRACT_BEHAVIOR_HH
# include <mlc/type.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/coord.hh>


namespace oln {


  namespace abstract {

    /*! \class behavior
    ** Behavior hierarchy.
    **
    ** The aim of this one is to describe how an algorithm should work
    ** on borders.
    */
    template <class Exact>
    class behavior: public mlc_hierarchy::any<Exact>
    {
    public:
      typedef behavior<Exact>				self_type; 
      /*!< The self type.*/
      typedef mlc_exact_vt_type(self_type, Exact)	exact_type; 
      /*!< The exact type.*/

      /*!
      ** \brief Adapt the border of an image.
      **
      ** Adapt the border of an image regarding the kind of behavior wanted.
      */
      template <class I>
      void adapt_border(oln::abstract::image<I> &im, coord border_size) const
      {
	mlc_dispatch(adapt_border)(im, border_size);
      };
    protected:
      /*!
      ** \brief CTor
      **
      ** Do nothing, used only by sub-classes.
      */
      behavior() {};
    };
  } // !abstract
}

#endif // !OLENA_CORE_ABSTRACT_BEHAVIOR_HH
