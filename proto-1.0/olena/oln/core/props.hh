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

#ifndef PROTO_OLN_CORE_PROPS_HH
# define PROTO_OLN_CORE_PROPS_HH

# include <mlc/types.hh>

# include <oln/core/cats.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  /*! \class default_props
  **
  ** \brief Class that defines properties by default, so properties are
  ** undefined.  // FIXME: this doc should be modified...
  **
  ** Practically all typedefs of default_props are thus set to
  ** mlc::undefined_type.
  **
  ** When props<E> is specialized, the programmer should derive that
  ** specialization from another props<E'> or from default_props.
  ** That ensures that an undefined property is set to mlc::undefined_type.
  **
  ** \see props<E>
  */
  template < typename category >
  struct default_props;


  /*! \class props<E>
  **
  ** Declaration of the trait class for properties.
  ** Parameter E is the targeted type.  FIXME: rewrite doc.
  */
  template <typename category, typename type>
  struct props : public default_props <category>
  {};

  template <typename category, typename type>
  struct props <category, const type> : public props <category, type>
  {};



} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_PROPS_HH
