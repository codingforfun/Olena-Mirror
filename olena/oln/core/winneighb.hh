// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

namespace oln {

  template< class Win >
  struct winneighb {
    typedef Point(Win) point;
    typedef DPoint(Win) dpoint;

    template< class T >
    winneighb(const Win& win, const T& anchor) :
      _win(win), _pos(0), _anchor(anchor.point_ref()) {}

    const point& point_ref() const
    {
      return _cur;
    }

    internal::_begin
    operator=(internal::_begin)
    {
      _pos = 0;
      _cur = _anchor + _win.dp(_pos);
      return begin;
    }

    internal::_end
    operator=(internal::_end)
    {
      _pos = _win.card();
      return end;
    }

    bool
    operator==(internal::_end) const
    {
      return _pos == _win.card();
    }

    bool
    operator!=(internal::_end) const
    {
      return _pos != _win.card();
    }

    winneighb&
    operator++()
    {
      precondition(*this != end);
      ++_pos;
      if (*this != end)
	_cur = _anchor + _win.dp(_pos);
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
      return _cur;
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
    const Win &_win;
    unsigned _pos;
    const point &_anchor;
    point _cur;
  };

# define Neighb(Neighbable)			\
typename Neighbable::neighb


} // oln

#endif // OLENA_CORE_WINNEIGHB_HH
