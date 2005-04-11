// Copyright (C) 2001, 2003, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_FWD_QITER2D_HH
# define OLENA_CORE_FWD_QITER2D_HH

# include <string>

# include <oln/core/abstract/qiter.hh>
# include <oln/core/2d/dpoint2d.hh>
# include <oln/core/2d/window2d.hh>


namespace oln {

  // fwd decl
  struct fwd_qiter2d;

  // category
  template <>
  struct set_super_type< fwd_qiter2d > { typedef abstract::qiter< fwd_qiter2d > ret; };

  // props
  template <>
  struct set_props < category::qiter, fwd_qiter2d >
  {
    typedef dpoint2d dpoint_type; // FIXME: !!!
    typedef window2d window_type; // FIXME: !!!
  };


  struct fwd_qiter2d : public abstract::qiter< fwd_qiter2d >
  {
    typedef abstract::qiter<fwd_qiter2d> super_type;

    fwd_qiter2d(const window2d& se) :
      se_(se),
      pos_(0)
    {
      this->invalidate();
    }

    coord_t impl_nth(unsigned i)
    {
      return se_[pos_].nth(i);
    }

    dpoint2d impl_cast_dpoint() const
    {
      precondition(this->is_valid());
      return se_[pos_];
    }

    void impl_start()
    {
      pos_ = 0;
    }

    void impl_next()
    {
      ++pos_;
    }

    bool impl_is_valid() const
    {
      return pos_ != se_.card();
    }

    void impl_invalidate()
    {
      pos_ = se_.card();
    }

    const window2d& se_;
    unsigned pos_;

  };

} // end of namespace oln

#endif // OLENA_CORE_FWD_QITER2D_HH
