// Copyright (C) 2001, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_IMPL_IMAGE_ARRAY1D_HH
# define OLENA_CORE_IMPL_IMAGE_ARRAY1D_HH

# include <oln/core/impl/image_array.hh>

namespace oln {

  template<class T>
  void pretreat_1d_data_(T*& buffer, T*& buffer_, const image1d_size& s)
  {
    precondition(s.ncols() > 0 && s.border() >= 0);
    buffer_ = buffer + s.border(); 
  }

  namespace impl {

    template<class T, class ExactI>
    class image_array1d :
      public image_array<T, ExactI, image_array1d<T, ExactI> >
    {
    public:
      typedef typename image_traits<ExactI>::point_type point_type;
      typedef typename image_traits<ExactI>::iter_type iter_type;
      typedef typename image_traits<ExactI>::fwd_iter_type fwd_iter_type;
      typedef typename image_traits<ExactI>::bkd_iter_type bkd_iter_type;
      typedef typename image_traits<ExactI>::value_type value_type;
      typedef typename image_traits<ExactI>::size_type size_type;

      typedef image_array<T, ExactI, image_array1d<T, ExactI> > super_type;
      typedef image_array1d<T, ExactI> self_type;

      image_array1d(const size_type& s): super_type(s) 
      {
	pretreat_1d_data_(buffer_, buffer__, s);
      }

      bool hold_(const oln::point1d& p) const
      {
	return
	  p.col() >= 0 &&
	  p.col() < size_.ncols();
      }

      bool hold_large_(const oln::point1d& p) const
      {
	return
	  p.col() >= - size_.border() &&
	  p.col() < size_.ncols() + size_.border();
      }


      value_type& at_(const point_type& p)
      {
	return at_(p.col());
      }

      value_type& at_(coord col)
      {
	invariant(buffer_ != 0);
	precondition_hold_large(point_type(col));
	return buffer__[col];
      }

      size_t len_(const oln::image1d_size& s) const
      {
	coord ncols_eff = s.ncols() + 2 * s.border();
	return size_t(ncols_eff);
      }

      // borders
      void border_reallocate_and_copy_(coord new_border, bool
				      copy_border) 
      {
	T* buffer = 0;
	// first allocate
	allocate_data_(buffer, len_(image1d_size(size_.ncols(), new_border)));
	// move data
	coord border = size_.border();
	if (border > new_border)
	  border = new_border;
	coord src_min_col = copy_border ? -border : 0;
	coord src_ncols = size_.ncols() + (copy_border ? (border * 2) : 0);
	memcpy(buffer + new_border, &at_(src_min_col),
	       src_ncols * sizeof(T));

	// then replace
	size_.border() = new_border;
	pretreat_1d_data_(buffer, buffer__,
			  image1d_size(size_.ncols(), new_border));
	desallocate_data_(buffer_);
	buffer_ = buffer;
	
      }
            
      void border_replicate_(void) 
      {
	for (coord j = - size_.border(); j; ++j)
	  {
	    at_(j) = at_(0);
	    at_(size_.ncols() - j - 1) = at_(size_.ncols() - 1);
	  }
      }

      void border_mirror_(void)
      {
	for (coord j = - size_.border(); j; ++j)
	  {
	    at_(j) = at_(-j);
	    at_(size_.ncols() - j - 1) = at_(size_.ncols() + j - 1);
	  }
      }

      void border_assign_(value_type val) 
      {
	for (coord j = - size_.border(); j; ++j)
          {
            at_(j) = val;
            at_(size_.ncols() - j - 1) = val;
          }
      }
    protected:

    private:
      T* buffer__;
    };

  } // end of namespace impl

} // end of namespace oln

#endif // ! OLENA_CORE_IMPL_IMAGE_ARRAY1D_HH
