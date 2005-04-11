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

#ifndef OLENA_CORE_ABSTRACT_REGULAR_NITER_HH
# define OLENA_CORE_ABSTRACT_REGULAR_NITER_HH

# include <vector>

# include <mlc/any.hh>
# include <mlc/types.hh>
# include <mlc/contract.hh>

# include <oln/core/abstract/niter.hh>
# include <oln/core/gen/image_with_nbh.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/dpoint.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/neighborhood.hh>
# include <oln/core/abstract/image_neighbness.hh>


namespace oln {

  // fwd decl
  namespace abstract {
    template <typename E> struct regular_niter;
  }

  // super type
  template <typename E>
  struct set_super_type < abstract::regular_niter<E> > { typedef abstract::niter<E> ret; };


  namespace abstract {

    template <typename E>
    struct regular_niter : public niter<E>
    {
      typedef oln_type_of(E, point)  point_type;
      typedef oln_type_of(E, dpoint) dpoint_type;

      void impl_start()
      {
	dp_cur_ = dp_.begin();
      }

      void impl_next()
      {
	++dp_cur_;
      }

      void impl_center_at(const point_type& pt)
      {
	this->p_ = pt;
	dp_.clear();
	this->exact().impl_determine_neighb();
      }

      bool impl_is_valid() const
      {
	return dp_cur_ != dp_.end();
      }

      const point_type impl_cast_point() const
      {
	return *dp_cur_ + this->p_;
      }

      void impl_invalidate()
      {
	dp_cur_ = dp_.end();
	postcondition(! this->is_valid());
      }

    protected:

      void impl_determine_neighb()
      {
	for (unsigned i = 0; i < this->nbh_.card(); ++i)
	  dp_.push_back(this->nbh_[i]);
      }

      typedef niter<E> super_type;

      template <typename I>
      regular_niter(const abstract::image_with_nbh<I>& ima) :
	super_type(ima)
      {
      }

      std::vector<dpoint_type> dp_;
      typename std::vector<dpoint_type>::const_iterator dp_cur_;
    };

  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_REGULAR_NITER_HH
