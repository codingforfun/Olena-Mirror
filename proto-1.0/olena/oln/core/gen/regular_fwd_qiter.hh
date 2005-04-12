// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_REGULAR_FWD_QITER_HH
# define OLENA_CORE_REGULAR_FWD_QITER_HH

# include <oln/core/abstract/grid.hh>
# include <oln/core/abstract/qiter.hh>
# include <oln/core/gen/regular_window.hh>


namespace oln {

  // fwd decl
  template <typename G>
  struct regular_fwd_qiter;

  // category
  template <typename G>
  struct set_super_type< regular_fwd_qiter<G> > { typedef abstract::qiter< regular_fwd_qiter<G> > ret; };

  // props
  template <typename G>
  struct set_props < category::qiter, regular_fwd_qiter<G> >
  {
    typedef regular_window<G> window_type;
  };


  template <typename G>
  struct regular_fwd_qiter : public abstract::qiter< regular_fwd_qiter<G> >
  {
    typedef regular_fwd_qiter<G> self_type;
    typedef abstract::qiter<self_type> super_type;

    typedef oln_grd_type_of(G, coord)  coord_type;
    typedef oln_grd_type_of(G, dpoint) dpoint_type;

    regular_fwd_qiter(const regular_window<G>& win) :
      super_type(win)
    {
      this->invalidate();
    }

    const coord_type nth(unsigned i) const
    {
      return this->win_[this->pos_].nth(i);
    }

    coord_type& nth(unsigned i)
    {
      return this->win_[this->pos_].nth(i);
    }

    operator dpoint_type() const
    {
      precondition(this->is_valid());
      return this->win_[this->pos_];
    }

    friend class abstract::iter<self_type>;
    friend class abstract::qiter<self_type>;

  protected:

    void impl_start()
    {
      this->pos_ = 0;
    }

    void impl_next()
    {
      ++(this->pos_);
    }

    bool impl_is_valid() const
    {
      return this->pos_ != this->win_.card();
    }

    void impl_invalidate()
    {
      this->pos_ = this->win_.card();
    }

    unsigned pos_;

  };

} // end of namespace oln


#endif // OLENA_CORE_REGULAR_FWD_QITER_HH
