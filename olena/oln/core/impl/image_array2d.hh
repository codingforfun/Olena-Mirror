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

#ifndef OLENA_CORE_IMPL_IMAGE_ARRAY2D_HH
# define OLENA_CORE_IMPL_IMAGE_ARRAY2D_HH

# include <oln/core/impl/image_array.hh>
# include <oln/core/image2d_size.hh>
# include <oln/core/point2d.hh>

namespace oln {

  template<class T>
  void pretreat_2d_data_(T*& buffer, T**& array, const image2d_size& s)
  { 
    precondition(s.nrows() > 0 &&
		 s.ncols() > 0 &&
		 s.border() >= 0);
    coord nrows_eff = s.nrows() + 2 * s.border();
    coord ncols_eff = s.ncols() + 2 * s.border();
    array = new T*[size_t(nrows_eff)];
    T* buf = buffer + s.border();
    for (coord row = 0; row < nrows_eff; ++row)
      {
	array[row] = buf;
	buf += ncols_eff;
      }
    array += s.border();
  }

  template<class T>
  void desallocate_2d_data_(T**& array, const image2d_size& s)
  {
    array -= s.border();
    delete[] array;
    array = 0;  // security
  }

  namespace impl {
    template<class T>
    class image_array2d;
  } // end of impl

  template<class T>
  struct impl_traits<impl::image_array2d<T> >: public impl_traits<impl::image_array<T, impl::image_array2d<T> > >
  {
    enum { dim = 2 };
    typedef point2d point_type;
    typedef image2d_size size_type;
    typedef T value_type;
  };


  namespace impl 
  {
    
    template<class T>
    class image_array2d : 
      public image_array<T, image_array2d<T> >
    {
    public:
      typedef image_array2d<T> self_type;
      typedef image_array2d<T> exact_type;

      typedef typename impl_traits<exact_type>::point_type point_type;
      typedef typename impl_traits<exact_type>::value_type value_type;
      typedef typename impl_traits<exact_type>::size_type size_type;

      typedef image_array<T, image_array2d<T> > super_type;
 

      friend class image_impl<image_array2d<T> >;
      friend class super_type;

      image_array2d(const size_type& s): super_type(s) 
      {
	pretreat_2d_data_(this->buffer_, array_, s);
      }

      ~image_array2d() 
      {
	desallocate_2d_data_(array_, this->size_); 
      }

    protected:

      bool hold_(const point_type& p) const
      {
	return
	  p.row() >= 0 &&
	  p.row() < this->size_.nrows() &&
	  p.col() >= 0 &&
	  p.col() < this->size_.ncols();
      }

      bool hold_large_(const point_type& p) const
      {
	return
	  p.row() >= - this->size_.border() &&
	  p.row() < this->size_.nrows() + this->size_.border() &&
	  p.col() >= - this->size_.border() &&
	  p.col() < this->size_.ncols() + this->size_.border();
      }

      value_type& at_(const point_type& p)
      {
	return at_(p.row(), p.col());
      }

      value_type& at_(coord row, coord col)
      {
	invariant(this->buffer_ != 0);
	precondition_hold_large(point_type(row, col));
	return array_[row][col];
      }

      size_t len_(const size_type& s) const 
      {
	coord ncols_eff = s.ncols() + 2 * s.border();
	coord nrows_eff = s.nrows() + 2 * s.border();
	return size_t(nrows_eff * ncols_eff);
      }

      // borders

      void border_reallocate_and_copy_(coord new_border, bool
				      copy_border) 
      {
	T* buffer = 0;
	T** array = 0;
	// first allocate
	allocate_data_(buffer, len_(size_type(this->size_.nrows(), this->size_.ncols(), new_border)));
	
	pretreat_2d_data_(buffer, array, size_type(this->size_.nrows(),
						   this->size_.ncols(), new_border));
	// move data
	coord border = this->size_.border();
	if (border > new_border)
	  border = new_border;
	coord src_min_row = copy_border ? -border : 0;
	coord src_max_row = this->size_.nrows() + (copy_border ? border : 0);
	coord src_min_col = copy_border ? -border : 0;
	coord src_ncols = this->size_.ncols() + (copy_border ? (border * 2) : 0);
	for (coord row = src_min_row; row < src_max_row; ++row)
	  memcpy(array[row],
		 &at_(row, src_min_col),
		 src_ncols * sizeof(T));

	// then replace
	desallocate_data_(this->buffer_);
	desallocate_2d_data_(array_, this->size_); 
	this->size_.border() = new_border;
	this->buffer_ = buffer;
	array_ = array;
      }
            
      void border_replicate_(void)
      {
	const coord imax = this->size_.nrows() - 1;
	const coord jmax = this->size_.ncols() - 1;
	// top & bottom
	for (coord i = - this->size_.border(); i; ++i)
	  for (coord j = 0; j <= jmax; ++j)
	    {
	      at_(i, j) = at_(0, j);
	      at_(imax - i, j) = at_(imax, j);
	    }
	// left & right
	for (coord i = - this->size_.border(); i <= imax + this->size_.border(); ++i)
	  for (coord j = - this->size_.border(); j; ++j)
	    {
	      at_(i, j) = at_(i, 0); 
	      at_(i, jmax - j) = at_(i, jmax );
	    }
      }

      void border_mirror_(void) 
      {
	// top & bottom
	const coord imax = this->size_.nrows() - 1;
	const coord jmax = this->size_.ncols() - 1;

	for (coord i = - this->size_.border(); i; ++i)
	  for (coord j = 0; j <= jmax; ++j)
	    {
	      at_(i, j) = at_(-i, j);
	      at_(imax - i, j) = at_(imax + i, j);
	    }
	// left & right
	for (coord i = - this->size_.border(); i <= imax + this->size_.border(); ++i)
	  for (coord j = - this->size_.border(); j; ++j)
	    {
	      at_(i, j) = at_(i, -j);
	      at_(i, jmax - j) = at_(i, jmax + j);
	    }
      }

      void border_assign_(value_type val) 
      {
	// top & bottom
        const coord imax = this->size_.nrows() - 1;
        const coord jmax = this->size_.ncols() - 1;

        for (coord i = - this->size_.border(); i; ++i)
          for (coord j = 0; j <= jmax; ++j)
            {
              at_(i, j) = val;
              at_(imax - i, j) = val;
            }
        // left & right
        for (coord i = - this->size_.border(); i <= imax + this->size_.border(); ++i)
          for (coord j = - this->size_.border(); j; ++j)
            {
              at_(i, j) = val;
              at_(i, jmax - j) = val;
            }
      }

    private:
      T** array_;
    };

  } // end of namespace impl

} // end of namespace oln

#endif // ! OLENA_CORE_IMPL_IMAGE_ARRAY2D_HH
