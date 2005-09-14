// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_OLN_CORE_GEN_CST_HH
# define OLENA_OLN_CORE_GEN_CST_HH

# include <oln/core/abstract/any.hh>
# include <oln/core/gen/internal/value_box.hh>

namespace oln
{

  template <typename T>
  class constant : public abstract::any< constant<T> >
  {
  public:
    constant(const T value) : value_(value) {}
    operator T() const { return value_; }
    const T value() const { return value_; }
    typedef T value_type;
  private:
    T value_;
  };

  template <typename T>
  const constant<T> cst(const T& value)
  {
    constant<T> tmp(value);
    return tmp;
  }

  template <typename I>
  const constant<oln_type_of(I, value)> cst(const value_box<I>& vb)
  {
    constant<oln_type_of(I, value)> tmp(vb.value());
    return tmp;
  }

} // end of namespace oln

#endif // !OLENA_OLN_CORE_GEN_CST_HH
