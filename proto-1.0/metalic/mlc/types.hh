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

#ifndef METALIC_TYPES_HH
# define METALIC_TYPES_HH


/*! \namespace mlc
** \brief mlc namespace.
*/
namespace mlc {


  /*! \class undefined_type
  **
  ** Type by default.  If the programmer forgets to define a type,
  ** actually this type should however exists and should be set to
  ** undefined_type.
  */
  class undefined_type { private: undefined_type(); };

  // FIXME: document.
  class unknown_type { private: unknown_type(); };
  class no_type      { private: no_type(); };
  class default_type { private: default_type(); };


} // end of namespace mlc


#endif // ! METALIC_TYPES_HH
