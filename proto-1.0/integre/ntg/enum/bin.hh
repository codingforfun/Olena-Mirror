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

#ifndef INTEGRE_ENUM_BIN_HH
# define INTEGRE_ENUM_BIN_HH

# include <mlc/traits.hh>

# include <ntg/core/props.hh>
# include <ntg/core/cats.hh>
# include <ntg/enum/enum.hh>

namespace ntg {

  struct bin;

  template <>
  struct category_type< bin > { typedef cat::enum_value ret; };


  template <>
  struct props<cat::enum_value, bin> : public default_props<cat::enum_value>
  {
    enum { max_val = 1 };
    typedef unsigned char io_type;
  };

  struct bin : public enum_value<bin>
  {
    bin() :
      value_(0)
    {
    }

    bin(unsigned char value) :
      value_(value)
    {
    }

    bin(const bin& rhs) :
      value_(rhs)
    {
    }

    template <typename V>
    bin& impl_assign(const V& rhs)
    {
      this->value_ = (int)rhs % 2;
      return *this;
    }

    operator unsigned char() const
    {
      return value_;
    }

    bool impl_eq(const unsigned char& rhs) const
    {
      return this->value_ == rhs;
    }

    template <typename V>
    bool impl_eq(const V& rhs) const
    {
      return this->value_ == (unsigned char)rhs;
    }


    template <typename V>
    bin impl_add(const V& rhs) const
    {
      bin tmp((this->value_ + rhs) % 2);
      return tmp;
    }

  private:

    unsigned char value_;
  };


} // end of namespace ntg



namespace mlc {

  template <>
  struct traits < ntg::bin >
  {
    typedef unsigned char encoding_type;
  };

} // end of namespace mlc



#endif // ! INTEGRE_ENUM_BIN_HH
