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

#ifndef OLENA_CORE_BKD_ITER3D_HH
# define OLENA_CORE_BKD_ITER3D_HH

# include <oln/core/internal/iter3d.hh>


namespace oln {

  template<class Exact = type::final>
  class bkd_iter3d : public internal::_iter3d<typename type::exact_vt<bkd_iter3d<Exact>, Exact>::ret>, 
                     public bkd_iter<typename type::exact_vt<bkd_iter3d<Exact>, Exact>::ret>
  {
  public:
    typedef internal::_iter3d<typename type::exact_vt<bkd_iter3d<Exact>, Exact>::ret> super;
    typedef bkd_iter<typename type::exact_vt<bkd_iter3d<Exact>, Exact>::ret> super2;

    enum { dim = 3 };
    typedef point3d point;

    template<class Image>
    bkd_iter3d(const Image& ima) :
      super(ima.size()), super2()
    {
    }

    template<class U> U operator=(U u) { return super::operator=(u); }

    void _goto_begin()
    {
      _p.slice() = _nslices - 1;
      _p.row() = _nrows - 1;
      _p.col() = _ncols - 1;
    }

    void _goto_end()
    {
      _p.slice() = -1;
    }

    bool _is_at_end() const
    {
      return _p.slice() == -1;
    }

    void _goto_next()
    {
      --_p.col();
      if (_p.col() >= 0)
	return;
      _p.col() = _ncols - 1;
      --_p.row();
      if (_p.row() >= 0)
	_p.row() = _nrows - 1;
      --_p.slice();
    }

    static std::string name() { return std::string("bkd_iter3d<") + Exact::name() + ">"; }

  };

} // end of oln


#endif // ! OLENA_CORE_BKD_ITER3D_HH
