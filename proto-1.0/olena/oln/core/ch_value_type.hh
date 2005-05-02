// Copyright (C)  2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_CH_VALUE_TYPE_HH
# define OLENA_CORE_CH_VALUE_TYPE_HH

# include <oln/core/abstract/image.hh>


# define oln_ch_value_type(I, T) \
  typename ch_value_type< I, T >::ret


// FIXME: bad name...
# define oln_ch_concrete_type(I, T) \
  typename ch_value_type< oln_type_of(I, concrete), T >::ret


namespace oln {

  template<class I, class T = oln_type_of(I, value)>
  struct ch_value_type
  {
    // FIXME: Use/extend oln/core/properties.hh mechanisms, instead of
    // using mlc/properties.hh directly.
    typedef typename
      internal::get_props<category::image, I>::template ch_value_type<T>::ret
      ret;
  };

} // end of namespace oln

#endif // ! OLENA_CORE_CH_VALUE_TYPE_HH
