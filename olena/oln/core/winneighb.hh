// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_WINNEIGHB_HH
# define OLENA_CORE_WINNEIGHB_HH

# include <mlc/objs.hh>
# include <oln/core/image.hh>
# include <string>

namespace oln
{
  template< class Win >
  struct winneighb {
    typedef Point(Win) point;
    typedef DPoint(Win) dpoint;

    template< class T >
    winneighb(const typename Win::abstract_type& win, const T& anchor) :
      win_(to_exact(win)), pos_(0), anchor_(anchor.point_ref()) {}

    const point& point_ref() const
    {
      return cur_;
    }

    mlc::_begin
    operator=(mlc::_begin)
    {
      pos_ = 0;
      cur_ = anchor_ + win_.dp(pos_);
      return begin;
    }

    mlc::_end
    operator=(mlc::_end)
    {
      pos_ = win_.card();
      return end;
    }

    bool
    operator==(mlc::_end) const
    {
      return pos_ == win_.card();
    }

    bool
    operator!=(mlc::_end) const
    {
      return pos_ != win_.card();
    }

    winneighb&
    operator++()
    {
      precondition(*this != end);
      ++pos_;
      if (*this != end)
	cur_ = anchor_ + win_.dp(pos_);
      return *this;
    }

    winneighb
    operator++(int)
    {
      precondition(*this != end);
      winneighb i = *this;
      ++(*this);
      return i;
    }

    operator point() const
    {
      precondition(*this != end);
      return cur_;
    }

    // it's convenient to type `it.cur()' instead of `(point)it' when
    // necessary.
    point cur() const
    {
      return *this;
    }

    static std::string name()
    {
      return std::string("winneighb<") + Win::name() + ">";
    }

  private:
    const Win &win_;
    unsigned pos_;
    const point &anchor_;
    point cur_;
  };

# define Neighb(Neighbable)			\
typename Neighbable::neighb_type


} // end of oln

#endif // OLENA_CORE_WINNEIGHB_HH
