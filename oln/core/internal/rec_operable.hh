// Copyright 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_INTERNAL_REC_OPERABLE_HH
# define OLENA_CORE_INTERNAL_REC_OPERABLE_HH


// warning: do not use namespace to work around a bug in g++ <= 2.95.2




//
//  rec_logic_operable<Self>
//
////////////////////////////////


template <typename Self>
struct rec_logic_operable {

  // needs op==() and op<()

  friend const Self operator|(const Self& lhs, const Self& rhs)
  {
    Self tmp(lhs);
    tmp |= rhs;
    return tmp;
  }

  friend const Self operator&(const Self& lhs, const Self& rhs)
  {
    Self tmp(lhs);
    tmp &= rhs;
    return tmp;
  }

  friend const Self operator^(const Self& lhs, const Self& rhs)
  {
    Self tmp(lhs);
    tmp ^= rhs;
    return tmp;
  }
};




//
//  rec_eq_operable<Self>
//
////////////////////////////////


template <typename Self>
struct rec_eq_operable
{
  // needs op==()

  friend bool operator!=(const Self& lhs, const Self& rhs)
  {
    return !(lhs == rhs);
  }
};



//
//  rec_cmp_operable<Self>
//
////////////////////////////////


template <typename Self>
struct rec_cmp_operable {

  // needs op<()

  friend bool operator>=(const Self& lhs, const Self& rhs)
  {
    return !(lhs < rhs);
  }

  friend bool operator>(const Self& lhs, const Self& rhs)
  {
    return rhs < lhs;
  }

  friend bool operator<=(const Self& lhs, const Self& rhs)
  {
    return !(rhs < lhs);
  }
};




//
//  rec_cmp_operable<Self>
//
////////////////////////////////


template <typename Self>
struct rec_arith_operable {

  // needs op+=(), op-=(), op*=(), op/=(), unitmp()

  friend const Self operator+(const Self& lhs, const Self& rhs)
  {
    Self tmp(lhs);
    tmp += rhs;
    return tmp;
  }

  friend const Self operator-(const Self& lhs, const Self& rhs)
  {
    Self tmp(lhs);
    tmp -= rhs;
    return tmp;
  }

  friend const Self operator*(const Self& lhs, const Self& rhs)
  {
    Self tmp(lhs);
    tmp *= rhs;
    return tmp;
  }

  friend const Self operator/(const Self& lhs, const Self& rhs)
  {
    Self tmp(lhs);
    tmp /= rhs;
    return tmp;
  }

  friend Self& operator++(Self& lhs)
  {
    lhs += 1;
    return lhs;
  }

  friend Self operator++(Self& lhs, int)
  {
    Self temp(lhs);
    lhs += 1;
    return temp;
  }

  friend Self& operator--(Self& lhs)
  {
    lhs -= 1;
    return lhs;
  }

  friend Self operator--(Self& lhs, int)
  {
    Self temp(lhs);
    lhs -= 1;
    return temp;
  }

  friend Self operator+(const Self& lhs)
  {
    return lhs;
  }

  friend Self operator-(const Self& lhs)
  {
    return 0 - lhs;
  }
};




#endif // ! OLENA_CORE_INTERNAL_REC_OPERABLE_HH
