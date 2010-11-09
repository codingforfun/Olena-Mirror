/* Copyright (C) 2005, 2010 EPITA Research and Development Laboratory (LRDE).

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#ifndef XTC_DATA_HXX
# define XTC_DATA_HXX

# include <string>
# include <cassert>
# include <iostream>

# include "function.hh"
# include "name_of.hh"

namespace xtc {

  template <typename T>
  T data::convert_to() const
  {
    static fun xtc_data_cast(std::string("data_cast< ") +
                             type() + ", " + mlc_name<T>::of() + " >");
    return xtc_data_cast(*this);
  }


  template <typename T>
  T& data::get_ref_on()
  {
    assert(proxy_);
    data_proxy_by_ref<T>* reinterpret_cast_returned_pointer = reinterpret_cast<data_proxy_by_ref<T>*>(proxy_);
    assert(reinterpret_cast_returned_pointer);
    return reinterpret_cast_returned_pointer->obj();
  }


  template <typename T>
  const T& data::get_ref_on() const
  {
    assert(proxy_);
    data_proxy<T>* reinterpret_cast_returned_pointer = reinterpret_cast<data_proxy<T>*>(proxy_);
    assert(reinterpret_cast_returned_pointer);
    return reinterpret_cast_returned_pointer->const_ref();
  }


  template <typename T>
  data& data::operator=(const T& rhs)
  {
    assert(proxy_);
    static fun xtc_data_assign(std::string("data_assign<") + proxy()->type() + ", " + mlc_name<T>::of() + ">");
    xtc_data_assign(*this, rhs);
    return *this;
  }

}

std::ostream& operator<<(std::ostream& ostr, const xtc::data& d);
std::istream& operator>>(std::istream& istr, xtc::data& d);
xtc::data& operator++(xtc::data& d);
xtc::data& operator--(xtc::data& d);
bool operator!=(const xtc::data& lhs, const xtc::data& rhs);
bool operator==(const xtc::data& lhs, const xtc::data& rhs);
xtc::data operator+(const xtc::data& lhs, const xtc::data& rhs);

#endif // ! XTC_DATA_HXX
