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

#ifndef OLENA_CORE_GEN_REGULAR_BKD_NITER_HH
# define OLENA_CORE_GEN_REGULAR_BKD_NITER_HH

# include <oln/core/gen/image_with_nbh.hh>
# include <oln/core/gen/regular_niter.hh>


namespace oln {

  // fwd decl
  template <typename G> struct regular_bkd_niter;

  // super type
  template <typename G>
  struct set_super_type < regular_bkd_niter<G> > { typedef abstract::regular_niter< G, regular_bkd_niter<G> > ret; };


  template <typename G>
  struct regular_bkd_niter : public abstract::regular_niter< G, regular_bkd_niter<G> >
  {
    typedef regular_bkd_niter<G> self_type;
    typedef abstract::regular_niter<G, self_type> super_type;

    template <typename I>
    regular_bkd_niter(const abstract::image_with_nbh<I>& image) :
      super_type(image.nbh_get())
    {
    }

    friend class abstract::iter<self_type>;
    friend class abstract::niter<self_type>;

  protected:

    void impl_start_()
    {
      this->pos_ = this->nbh_.card() - 1;
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


#endif // ! OLENA_CORE_GEN_REGULAR_BKD_NITER_HH
