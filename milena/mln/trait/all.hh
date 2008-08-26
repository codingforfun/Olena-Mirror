// Copyright (C) 2006, 2008 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef MLN_TRAIT_ALL_HH
# define MLN_TRAIT_ALL_HH

/*!
 * \file   mln/trait/all.hh
 *
 * \brief  Definition of all traits.
 *
 */


namespace mln
{

  /// Namespace where traits are defined.
  namespace trait
  {

    /// \internal Image namespace of trait namespace.
    namespace image {}

    /// \internal Implementation namespace of trait namespace.
    namespace impl {}

    /// \internal Internal namespace of trait namespace.
    namespace internal {}

  }

} // end of namespace mln


// meta-program to solve a trait call
# include <mln/trait/solve.hh>

// promote
# include <mln/trait/promote.hh>

// ops
# include <mln/trait/op/all.hh>


#endif // ! MLN_TRAIT_ALL_HH
