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

#ifndef INTEGRE_REAL_INT_U8_HH
# define INTEGRE_REAL_INT_U8_HH

# include <mlc/traits.hh>

# include <ntg/core/cats.hh>
# include <ntg/core/props.hh>
# include <ntg/real/integer.hh>

namespace ntg {

  struct int_u8;

  template <>
  struct category_type< int_u8 > { typedef cat::integer ret; };

  template <>
  struct props<cat::integer, int_u8> : public default_props<cat::integer>
  {
    enum { max_val = 255 };
    typedef char io_type;
  };


  struct int_u8: public integer<int_u8>
  {
    int_u8() :
      value_(0)
    {
    }

    int_u8(unsigned char value) :
      value_(value)
    {
    }

    int_u8(const int_u8& rhs) :
      value_(rhs)
    {
    }

    int_u8& impl_assign(const int_u8& rhs)
    {
      this->value_ = rhs;
      return *this;
    }

    int_u8& impl_assign(const unsigned char& rhs)
    {
      this->value_ = rhs;
      return *this;
    }

    operator unsigned char() const
    {
      return value_;
    }

//     template <typename V>
//     bool impl_eq(const V& rhs) const
//     {
//       return this->value_ == rhs;
//     }

    bool impl_eq(int rhs) const
    {
      return this->value_ == rhs;
    }

    bool impl_eq(const int_u8& rhs) const
    {
      return this->value_ == (unsigned char)rhs;
    }

    template <typename V>
    int_u8 impl_add(const V& rhs) const
    {
      int_u8 tmp(this->value_ + rhs);
      return tmp;
    }

  private:

    unsigned char value_;
  };


} // end of namespace ntg



namespace mlc {

  template <>
  struct traits < ntg::int_u8 >
  {
    typedef unsigned char encoding_type;
  };

} // end of namespace mlc



#endif // ! INTEGRE_REAL_INT_U8_HH
