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

#ifndef OLENA_CORE_FWD_ITER1D_HH
# define OLENA_CORE_FWD_ITER1D_HH

# include <oln/core/internal/iter1d.hh>

namespace oln {

  template<class Exact = type::final>
  class fwd_iter1d : public internal::_iter1d<typename type::exact_vt<fwd_iter1d<Exact>, Exact>::ret>, 
                     public fwd_iter<typename type::exact_vt<fwd_iter1d<Exact>, Exact>::ret>
  {
  public:

    typedef internal::_iter1d< typename type::exact_vt<fwd_iter1d<Exact>, Exact>::ret> super;
    typedef fwd_iter<typename type::exact_vt<fwd_iter1d<Exact>, Exact>::ret> super2;

    enum { dim = 1 };
    typedef point1d point;

    template<class Image>
    fwd_iter1d(const Image& ima) :
      super(ima.size()), super2()
    {
    }

    template<class U> U operator=(U u) { return super::operator=(u); }

    void _goto_begin()
    {
      _p.col() = 0;
    }

    void _goto_end()
    {
      _p.col() = _ncols;
    }

    bool _is_at_end() const
    {
      return _p.col() == _ncols;
    }

    void _goto_next()
    {
      ++_p.col();
    }

    static std::string name() { return "fwd_iter1d<" + Exact::name() + ">"; }
  };

} // end of oln

#endif // ! OLENA_CORE_FWD_ITER1D_HH
