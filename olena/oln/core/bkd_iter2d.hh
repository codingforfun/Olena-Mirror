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

#ifndef OLENA_CORE_BKD_ITER2D_HH
# define OLENA_CORE_BKD_ITER2D_HH

# include <oln/core/abstract/iter2d.hh>


namespace oln {

  template<class Exact = mlc::final>
  class bkd_iter2d; // fwd_decl

  template<class Exact>
  struct iter_traits<bkd_iter2d<Exact> >: public
  iter_traits<abstract::iter2d<typename
  mlc::exact_vt<bkd_iter2d<Exact>, Exact>::ret> >
  {

  };


  template<class Exact>
  class bkd_iter2d : public abstract::iter2d<typename mlc::exact_vt<bkd_iter2d<Exact>, Exact>::ret>
  {
  public:

    typedef typename mlc::exact_vt<bkd_iter2d<Exact>, Exact>::ret exact_type;

    typedef abstract::iter2d<exact_type> super_type;
    
    enum { dim = iter_traits<exact_type>::dim };
    typedef typename iter_traits<exact_type>::point_type point_type;

    template<class Image>
    bkd_iter2d(const Image& ima) :
      super_type(ima.size())
    {
    }

    template<class U> U operator=(U u) { return super_type::operator=(u); }

    void goto_begin_()
    {
      p_.row() = nrows_ - 1;
      p_.col() = ncols_ - 1;
    }

    void goto_end_()
    {
      p_.row() = -1;
    }

    bool is_at_end_() const
    {
      return p_.row() == -1;
    }

    void goto_next_()
    {
      --p_.col();
      if (p_.col() >= 0)
	return;
      p_.col() = ncols_ - 1;
      --p_.row();
    }

    static std::string name() { return std::string("bkd_iter2d<") + Exact::name() + ">"; }

  };


} // end of oln


#endif // ! OLENA_CORE_BKD_ITER2D_HH
