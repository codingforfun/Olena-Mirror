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

#ifndef OLENA_CORE_REGULAR_BKD_QITER_HH
# define OLENA_CORE_REGULAR_BKD_QITER_HH

# include <oln/core/gen/regular_qiter.hh>


namespace oln {

  // bkd decl
  template <typename G>
  struct regular_bkd_qiter;

  // category
  template <typename G>
  struct set_super_type< regular_bkd_qiter<G> > { typedef abstract::regular_qiter< G, regular_bkd_qiter<G> > ret; };


  template <typename G>
  struct regular_bkd_qiter : public abstract::regular_qiter< G, regular_bkd_qiter<G> >
  {
    typedef regular_bkd_qiter<G> self_type;
    typedef abstract::regular_qiter<G, self_type> super_type;

    regular_bkd_qiter(const regular_window<G>& win) :
      super_type(win)
    {
      this->invalidate();
    }

    friend class abstract::iter<self_type>;

  protected:

    void impl_start_()
    {
      this->pos_ = this->win_.card() - 1;
    }

    void impl_next()
    {
      --(this->pos_);
    }

    bool impl_is_valid() const
    {
      return this->pos_ != -1;
    }

    void impl_invalidate()
    {
      this->pos_ = -1;
    }

  };

} // end of namespace oln


#endif // OLENA_CORE_REGULAR_BKD_QITER_HH
