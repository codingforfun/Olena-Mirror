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

#ifndef INTEGRE_COLOR_RGB_8_HH
# define INTEGRE_COLOR_RGB_8_HH

# include <mlc/traits.hh>

# include <ntg/core/props.hh>
# include <ntg/color/color.hh>

namespace ntg {

  struct rgb_8;

  template <>
  struct category_type< rgb_8 > { typedef cat::color ret; };


  template <>
  struct props<cat::color, rgb_8> : default_props<cat::color>
  {
    enum { max_val = 255 };
    enum { depth = 3 };

    typedef char io_type;
    typedef unsigned char comp_type;
  };

  enum {
    rgb_red = 0,
    rgb_green = 1,
    rgb_blue = 2
  };


  struct rgb_8: public color <rgb_8>
  {
    rgb_8()
    {
      this->value_[rgb_red] = 0;
      this->value_[rgb_green] = 0;
      this->value_[rgb_blue] = 0;
    }

    rgb_8(const unsigned char init[3])
    {
      for (unsigned i = 0; i < 3; i++)
	value_[i] = init[i];
    }

    rgb_8(unsigned char red,
	  unsigned char green,
	  unsigned char blue)
    {
      this->value_[rgb_red] = red;
      this->value_[rgb_green] = green;
      this->value_[rgb_blue] = blue;
    }

    rgb_8(const rgb_8& rhs)
    {
      this->value_[rgb_red] = rhs[rgb_red];
      this->value_[rgb_green] = rhs[rgb_green];
      this->value_[rgb_blue] = rhs[rgb_blue];
    }

    rgb_8& impl_assign(const rgb_8& rhs)
    {
      this->value_[rgb_red] = rhs[rgb_red];
      this->value_[rgb_green] = rhs[rgb_green];
      this->value_[rgb_blue] = rhs[rgb_blue];
      return *this;
    }

    bool impl_eq(const rgb_8& rhs) const
    {
      return this->value_[rgb_red] == rhs[rgb_red] &&
	this->value_[rgb_green] == rhs[rgb_green] &&
	this->value_[rgb_blue] == rhs[rgb_blue];
    }

    unsigned char& impl_op_sqbr(unsigned int i)
    {
      assert(i < 3);
      return value_[i];
    }

    const unsigned char impl_op_sqbr(unsigned int i) const
    {
      assert(i < 3);
      return value_[i];
    }

  private:

    unsigned char value_[3];
  };


} // end of namespace ntg


# include <ostream>

std::ostream& operator<<(std::ostream& ostr, const ntg::rgb_8& to_print)
{
  ostr << "(" << unsigned(to_print[0])
       << "," << unsigned(to_print[1])
       << "," << unsigned(to_print[2])
       << ")";
  return ostr;
}



namespace mlc {

  template <>
  struct traits < ntg::rgb_8 >
  {
    typedef ntg::rgb_8 encoding_type;
  };

} // end of namespace mlc



#endif // ! NTG_COLOR_RGB_8_HH
