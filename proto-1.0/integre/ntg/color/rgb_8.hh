// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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


namespace ntg {


  enum {
    rgb_red = 0,
    rgb_green = 1,
    rgb_blue = 2
  };

  struct rgb_8
  {
    rgb_8()
    {
      this->value_[rgb_red] = 0;
      this->value_[rgb_green] = 0;
      this->value_[rgb_blue] = 0;
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
      this->value_[rgb_red] = rhs.red();
      this->value_[rgb_green] = rhs.green();
      this->value_[rgb_blue] = rhs.blue();
    }

    rgb_8& operator=(const rgb_8& rhs)
    {
      this->value_[rgb_red] = rhs.red();
      this->value_[rgb_green] = rhs.green();
      this->value_[rgb_blue] = rhs.blue();
      return *this;
    }

    bool operator==(const rgb_8& rhs) const
    {
      return this->value_[rgb_red] == rhs.red() &&
	this->value_[rgb_green] == rhs.green() &&
	this->value_[rgb_blue] == rhs.blue();
    }

    template <typename V>
    bool operator!=(const V& rhs) const
    {
      return this->value_[rgb_red] != rhs.red() ||
	this->value_[rgb_green] != rhs.green() ||
	this->value_[rgb_blue] != rhs.blue();
    }

    unsigned char& red()
    {
      return value_[rgb_red];
    }

    const unsigned char red() const
    {
      return value_[rgb_red];
    }

    unsigned char& green()
    {
      return value_[rgb_green];
    }

    const unsigned char green() const
    {
      return value_[rgb_green];
    }

    unsigned char& blue()
    {
      return value_[rgb_blue];
    }

    const unsigned char blue() const
    {
      return value_[rgb_blue];
    }

  private:

    unsigned char value_[3];
  };


} // end of namespace ntg



namespace mlc {

  template <>
  struct traits < ntg::rgb_8 >
  {
    typedef unsigned char* encoding_type;
  };

} // end of namespace mlc



#endif // ! INTEGRE_COLOR_RGB_8_HH
