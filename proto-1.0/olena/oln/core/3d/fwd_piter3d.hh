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

#ifndef OLENA_CORE_3D_FWD_PITER3D_HH
# define OLENA_CORE_3D_FWD_PITER3D_HH

# include <mlc/contract.hh>

# include <oln/core/abstract/piter.hh>
# include <oln/core/3d/point3d.hh>
# include <oln/core/3d/size3d.hh>


namespace oln {

  // fwd decl
  struct fwd_piter3d;

  // category
  template <>
  struct set_category<fwd_piter3d> { typedef category::piter ret; };


  // props
  template <>
  struct set_props < category::piter, fwd_piter3d > : public props_of<category::piter>
  {
    typedef point3d point_type;
    typedef size3d  size_type;
  };



  struct fwd_piter3d : public abstract::piter< fwd_piter3d >
  {

    typedef abstract::piter<fwd_piter3d> super_type;

    fwd_piter3d(const size3d& size) :
      super_type(size)
    {
      this->exact_ptr = this;
      this->invalidate();
    }

# if defined __GNUC__ && __GNUC__ >= 3
    friend class abstract::piter< fwd_piter3d >;
  protected:
# endif

    void impl_start()
    {
      this->p_.row() = 0;
      this->p_.col() = 0;
      this->p_.slice() = 0;
      postcondition(this->p_.row().is_defined() &&
		    this->p_.col().is_defined() &&
		    this->p_.slice().is_defined());
    }

    bool impl_is_valid() const
    {
      precondition(this->p_.row().is_defined() &&
		   this->p_.col().is_defined() &&
		   this->p_.slice().is_defined());
      return this->p_.slice() < this->s_.nslices();
    }

    void impl_next()
    {
      precondition(this->p_.row().is_defined() &&
		   this->p_.col().is_defined() &&
		   this->p_.slice().is_defined());
      precondition(this->p_.row() >= 0 && this->p_.row() < this->s_.nrows() &&
		   this->p_.col() >= 0 && this->p_.col() < this->s_.ncols() &&
		   this->p_.slice() >= 0 && this->p_.slice() < this->s_.nslices());
      ++this->p_.col();
      if (this->p_.col() != this->s_.ncols())
	return;
      this->p_.col() = 0;
      ++this->p_.row();
      if (this->p_.row() != this->s_.nrows())
	return;
      this->p_.row() = 0;
      ++this->p_.slice();
      postcondition(this->p_.row().is_defined() &&
		    this->p_.col().is_defined() &&
		    this->p_.slice().is_defined());
    }

    void impl_invalidate()
    {
      this->p_.row() = this->s_.nrows();
      this->p_.col() = this->s_.ncols();
      this->p_.slice() = this->s_.nslices();
      postcondition(this->p_.row().is_defined() &&
		    this->p_.col().is_defined() &&
		    this->p_.slice().is_defined());
    }

  };
}


#endif // ! OLENA_CORE_3D_FWD_PITER3D_HH
