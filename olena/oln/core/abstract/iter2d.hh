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

#ifndef OLENA_CORE_ABSTRACT_ITER2D_HH
# define OLENA_CORE_ABSTRACT_ITER2D_HH

# include <oln/core/coord.hh>
# include <oln/core/point2d.hh>
# include <oln/core/dpoint2d.hh>
# include <oln/core/image2d_size.hh>
# include <oln/core/abstract/iter.hh>

# include <mlc/contract.hh>
# include <mlc/type.hh>
# include <mlc/objs.hh>

namespace oln {

  namespace abstract {
    template<class Exact>
    class iter2d; // fwd_decl
  } // end of abstract

  template<class Exact>
  struct iter_traits<abstract::iter2d<Exact> >: public
  iter_traits<abstract::iter<Exact> >
  {
    enum { dim = 2 };
    typedef point2d point_type;
    typedef dpoint2d dpoint_type;
  };


  namespace abstract {


    template<class Exact>
    class iter2d : public iter< Exact >
    {
    public:

      typedef iter<Exact> super_type;
      friend class super_type;

      coord row() const
      {
	return this->p_.row();
      }

      coord col() const
      {
	return this->p_.col();
      }

      static std::string name() { return std::string("_iter2d<") + Exact::name() + ">"; }

    protected:

      const coord nrows_;
      const coord ncols_;

      point2d to_point() const
      {
	precondition(*this != end);
	invariant(this->p_.row() >= 0 &&
		  (this->p_.row() < nrows_ || this->p_.row() == nrows_) &&
		  this->p_.col() >= 0 &&
		  this->p_.col() < ncols_);
	return this->p_;
      }

      iter2d() : super_type(), nrows_(0), ncols_(0) {}

      iter2d(const image2d_size& size) :
	super_type(), 
	nrows_(size.nrows()),
	ncols_(size.ncols())
      {
	precondition(size.nrows() > 0 &&
		     size.ncols() > 0);
	this->exact().goto_begin_();
      }
    };


  } // end of abstract

} // end of oln


#endif // ! OLENA_CORE_ABSTRACT_ITER2D_HH
