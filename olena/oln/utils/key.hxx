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

#ifndef OLENA_OLN_UTILS_KEY_HXX
# define OLENA_OLN_UTILS_KEY_HXX

// Ctor implementation.
inline
key::key(const std::vector<value_type> &data)
{
  for (unsigned i = 0; i < 16; ++i)
    data_[i] = data[i];
}

// [] operator implementation
inline
key::value_type &key::operator[](unsigned i)
{
  precondition(i < 16);
  return data_[i];
};

// [] operator
inline
const key::value_type &key::operator[](unsigned i) const
{
  precondition(i < 16);
  return data_[i];
};

// == operator
inline
bool key::operator ==(const key &k)
{
  for (unsigned i = 0; i < 16; ++i)
    if (data_[i] != k[i])
      return false;
  return true;
}

// != operator
inline
bool key::operator !=(const key &k)
{
  return !(*this == k);
}

#endif // !OLENA_OLN_UTILS_KEY_HXX
