// Copyright (C) 2001, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_COORD_HH
# define OLENA_CORE_COORD_HH

# include <iostream>
# include <limits.h>
# include <mlc/contract.hh>


// FIXME: doc


namespace oln {


  class coord_t
  {
  public:

    typedef int value_type;
    static const value_type min_value = INT_MIN;
    static const value_type max_value = INT_MAX;

    coord_t() :
      value_(undef_())
    {
    }

    coord_t(value_type value) :
      value_(value)
    {
    }

    coord_t& operator=(value_type rhs)
    {
      this->value_ = rhs;
      return *this;
    }

    operator value_type() const
    {
      precondition(this->is_defined());
      return value_;
    }

    bool operator==(value_type rhs) const
    {
      if (! this->is_defined())
	{
	  std::cerr << "oops" << std::endl;
	}
      precondition(this->is_defined());
      return value_ == rhs;
    }

    bool operator!=(value_type rhs) const
    {
      precondition(this->is_defined());
      return value_ != rhs;
    }

    const coord_t operator+(value_type rhs) const
    {
      precondition(this->is_defined());
      coord_t tmp(value_ + rhs); return tmp;
    }

    coord_t& operator++()
    {
      precondition(this->is_defined());
      ++value_;
      return *this;
    }

    const coord_t operator++(int)
    {
      precondition(this->is_defined());
      coord_t tmp = *this;
      ++value_;
      return tmp;
    }

    const coord_t operator-(value_type rhs) const
    {
      precondition(this->is_defined());
      coord_t tmp(value_ - rhs);
      return tmp;
    }

    const coord_t operator-() const
    {
      precondition(this->is_defined());
      coord_t tmp(-value_); return tmp;
    }

    coord_t& operator--()
    {
      precondition(this->is_defined());
      --value_;
      return *this;
    }

    const coord_t operator--(int)
    {
      precondition(this->is_defined());
      coord_t tmp = *this;
      --value_;
      return tmp;
    }

    bool is_defined() const
    {
      return value_ != undef_();
    }

    bool is_undefined() const
    {
      return value_ == undef_();
    }

    static const coord_t& infty()
    {
      static coord_t infty_ = max_value;
      return infty_;
    }

    static const coord_t& _infty()
    {
      static coord_t _infty_ = min_value + 1;
      return _infty_;
    }

    // FIXME: to be continued

  private:

    value_type value_;

    static const value_type undef_()
    {
      return min_value;
    }

  };


} // end of namespace oln



std::ostream& operator<<(std::ostream& ostr, const oln::coord_t& c)
{
  precondition(c.is_defined());
  return ostr << oln::coord_t::value_type(c);
}
 

#endif // ! OLENA_CORE_COORD_HH
