// Copyright (C) 2007 EPITA Research and Development Laboratory.
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

#ifndef TOOLS_HH_
# define TOOLS_HH_

# include <assert.h>
# include <ostream>

namespace oln
{
  template <typename T>
  struct singleton
  {
    singleton() {}
    singleton(T val) : value_(val) {}
    T value_;
  };

  template <unsigned n, typename T>
  class vec
  {
  public:
    T operator[] (unsigned i) const { assert(i < n);
    return comp[i]; }
    T& operator[] (unsigned i) { assert(i < n);
    return comp[i]; }

  protected:
    T comp[n];
  };

  template <unsigned n, typename T>
  std::ostream& operator<<(std::ostream& ostr, const vec<n, T> v)
  {
    for (unsigned i = 0; i < n; ++i)
      ostr << v[i] << std::endl;
    return ostr;
  }
}

#endif /* !TOOLS_HH_ */
