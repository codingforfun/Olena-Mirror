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

#ifndef PROTO_OLN_CORE_2D_FWD_PITER2D_HH
# define PROTO_OLN_CORE_2D_FWD_PITER2D_HH

# include <mlc/contract.hh>

# include <oln/core/abstract/piter.hh>
# include <oln/core/2d/point2d.hh>
# include <oln/core/2d/size2d.hh>
# include <oln/core/props.hh>


namespace oln {

  struct fwd_piter2d;

  template <> struct category_type<fwd_piter2d> { typedef cat::piter ret; };

  template <>
  struct props < cat::piter, fwd_piter2d > : public default_props<cat::piter>
  {
    typedef point2d point_type;
    typedef size2d  size_type;
  };

  struct fwd_piter2d : public abstract::piter< fwd_piter2d >
  {

    typedef abstract::piter<fwd_piter2d> super_type;

    fwd_piter2d(const size2d& size) :
      super_type(size)
    {
      this->exact_ptr = this;
      this->invalidate();
    }

# if defined __GNUC__ && __GNUC__ >= 3
    friend class abstract::piter< fwd_piter2d >;
  protected:
# endif

    void impl_start()
    {
      this->p_.row() = 0;
      this->p_.col() = 0;
      postcondition(this->p_.row().is_defined() && this->p_.col().is_defined());
    }

    bool impl_is_valid() const
    {
      precondition(this->p_.row().is_defined() && this->p_.col().is_defined());
      return this->p_.row() < this->s_.nrows();
    }

    void impl_next()
    {
      precondition(this->p_.row().is_defined() && this->p_.col().is_defined());
      precondition(this->p_.row() >= 0 && this->p_.row() <= this->s_.nrows()
		   &&
		   this->p_.col() >= 0 && this->p_.col() <= this->s_.ncols());
      ++this->p_.col();
      if (this->p_.col() != this->s_.ncols())
	return;
      this->p_.col() = 0;
      precondition(this->p_.row() != this->s_.nrows());
      ++this->p_.row();
      postcondition(this->p_.row().is_defined() && this->p_.col().is_defined());
    }

    void impl_invalidate()
    {
      this->p_.row() = this->s_.nrows();
      this->p_.col() = this->s_.ncols();
      postcondition(this->p_.row().is_defined() && this->p_.col().is_defined());
    }

  };
}


#endif // ndef PROTO_OLN_CORE_2D_FWD_PITER2D_HH
