// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_1D_FWD_PITER1D_HH
# define OLENA_CORE_1D_FWD_PITER1D_HH

# include <mlc/contract.hh>

# include <oln/core/abstract/piter.hh>
# include <oln/core/1d/point1d.hh>
# include <oln/core/1d/size1d.hh>
# include <oln/core/props.hh>


namespace oln {

  struct fwd_piter1d;

  template <> struct category_type< fwd_piter1d > { typedef cat::piter ret; };

  template <>
  struct props < cat::piter, fwd_piter1d > : public default_props< cat::piter >
  {
    typedef point1d point_type;
    typedef size1d  size_type;
  };

  struct fwd_piter1d : public abstract::piter< fwd_piter1d >
  {

    typedef abstract::piter<fwd_piter1d> super_type;

    fwd_piter1d(const size1d& size) :
      super_type(size)
    {
      this->exact_ptr = this;
      this->invalidate();
    }

# if defined __GNUC__ && __GNUC__ >= 3
    friend class abstract::piter< fwd_piter1d >;
  protected:
# endif

    void impl_start()
    {
      this->p_.index() = 0;
      postcondition(this->p_.index().is_defined());
    }

    bool impl_is_valid() const
    {
      precondition(this->p_.index().is_defined());
      return this->p_.index() < this->s_.nindices();
    }

    void impl_next()
    {
      precondition(this->p_.index().is_defined());
      precondition(this->p_.index() >= 0 and this->p_.index() <= this->s_.nindices());
      if (this->p_.index() == this->s_.nindices())
	return;
      ++this->p_.index();
      postcondition(this->p_.index().is_defined());
    }

    void impl_invalidate()
    {
      this->p_.index() = this->s_.nindices();
      postcondition(this->p_.index().is_defined());
    }

  };
}


#endif // ! OLENA_CORE_1D_FWD_PITER1D_HH
