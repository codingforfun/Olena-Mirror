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

#ifndef OLENA_CORE_IMPL_IMAGE_ARRAY3D_HH
# define OLENA_CORE_IMPL_IMAGE_ARRAY3D_HH

# include <oln/core/impl/image_array.hh>

namespace oln {

  template<class T>
  void pretreat_3d_data_(T*& buffer, T**& array2, T***& array,
			 const image3d_size& s)
  {
    precondition(s.nslices() > 0
		 && s.nrows() > 0
		 && s.ncols() > 0
		 && s.border() >= 0);
    coord nslices_eff = s.nslices() + 2 * s.border();
    coord nrows_eff = s.nrows() + 2 * s.border();
    coord ncols_eff = s.ncols() + 2 * s.border();
    array = new T**[size_t(nslices_eff)];
    array2 = new T*[size_t(nslices_eff)
			* size_t(nrows_eff)];
    T* buf = buffer + s.border();
    
    for (coord slice = 0; slice < nslices_eff; ++slice)
      {
	T** a2 = array2 + slice * nrows_eff;
	array[slice] = a2 + s.border();
	for (coord row = 0; row < nrows_eff; ++row)
	  {
	    a2[row] = buf;
	    buf += ncols_eff;
	  }
      }
    array += s.border();
  }

  template<class T>
  void desallocate_3d_data_(T**& array2, T***& array, const
			    image3d_size& s)
  {
    delete[] array2;
    array2 = 0;  // security
    array -= s.border();
    delete[] array;
    array = 0;  // security
  }

  namespace impl
  {
    
    template<class T, class ExactI>
    class image_array3d :
      public image_array<T, ExactI, image_array3d<T, ExactI> >
    {
    public:
      typedef typename image_traits<ExactI>::point_type point_type;
      typedef typename image_traits<ExactI>::iter_type iter_type;
      typedef typename image_traits<ExactI>::fwd_iter_type fwd_iter_type;
      typedef typename image_traits<ExactI>::bkd_iter_type bkd_iter_type;
      typedef typename image_traits<ExactI>::value_type value_type;
      typedef typename image_traits<ExactI>::size_type size_type;

      typedef image_array<T, ExactI, image_array3d<T, ExactI> > super_type;
      typedef image_array3d<T, ExactI> self_type;

      image_array3d(const size_type& s): super_type(s) 
      {
	pretreat_3d_data_(buffer_, array2_, array_, s);
      }

      bool hold_(const oln::point3d& p) const
      {
	return (p.slice >= 0
		&& p.slice < size_.nslices()
		&& p.row() >= 0
		&& p.row() < size_.nrows()
		&& p.col() >= 0
		&& p.col() < size_.ncols());
      }

      bool hold_large_(const oln::point3d& p) const
      {
	return (p.slice() >= -size_.border()
		&& p.slice() < size_.nslices() + size_.border()
		&& p.row() >= - size_.border()
		&& p.row() < size_.nrows() + size_.border()
		&& p.col() >= - size_.border()
		&& p.col() < size_.ncols() + size_.border());
      }

      value_type& at_(const oln::point3d& p)
      {
	
	return at_(p.slice(), p.row(), p.col());
      }

      value_type& at_(coord slice, coord row, coord col)
      {
	invariant(buffer_ != 0);
	precondition_hold_large(point_type(slice, row, col));
	return array_[slice][row][col];
      }
      
      size_t len_(const oln::image3d_size& s) const
      {
	coord nslices_eff = s.nslices() + 2 * s.border();
	coord ncols_eff = s.ncols() + 2 * s.border();
	coord nrows_eff = s.nrows() + 2 * s.border();
	return size_t(nslices_eff * nrows_eff * ncols_eff);
      }

    protected:
      ~image_array3d()
      {
	desallocate_3d_data_(array2_, array_, size_);
      }

    public:
      // borders

      void border_reallocate_and_copy_(coord new_border, bool
				      copy_border)
      {
	T* buffer = 0;
	T** array2 = 0;
	T*** array = 0;
	// first allocate

	allocate_data_(buffer, len_(image3d_size(size_.nslices(), size_.nrows(),
					 size_.ncols(), new_border)));
	pretreat_3d_data_(buffer, array2, array, image3d_size(size_.nslices(), size_.nrows(),
							     size_.ncols(), new_border));
	// move data
	coord border = size_.border();
	if (border > new_border)
	  border = new_border;
	coord src_min_slice = copy_border ? -border : 0;
	coord src_max_slice = size_.nslices() + (copy_border ? border : 0);
	coord src_min_row = copy_border ? -border : 0;
	coord src_max_row = size_.nrows() + (copy_border ? border : 0);
	coord src_min_col = copy_border ? -border : 0;
	coord src_ncols = size_.ncols() + (copy_border ? (border * 2) : 0);
	for (coord slice = src_min_slice; slice < src_max_slice; ++slice)
	  for (coord row = src_min_row; row < src_max_row; ++row)
	    memcpy(array[slice][row],
		   &at_(slice, row, src_min_col),
		   src_ncols * sizeof(T));

	// then replace
	desallocate_data_(buffer_);
	desallocate_3d_data_(array2_, array_, size_);
	size_.border() = new_border;
	buffer_ = buffer;
	array2_ = array2;
	array_ = array;
      }
            
      void border_replicate_(void)
      {
	const coord imax = size_.nslices() - 1;
	const coord jmax = size_.nrows() - 1;
	const coord kmax = size_.ncols() - 1;
	// front & rear
	for (coord i = - size_.border(); i; ++i)
	  for (coord j = 0; j <= jmax; ++j)
	    for (coord k = 0; k <= kmax; ++k)
	      {
		at_(i, j, k)        = at_(0, j, k);
		at_(imax - i, j, k) = at_(imax, j, k);
	      }
	// top & bottom
	for (coord i = - size_.border(); i <= imax + size_.border(); ++i)
	  for (coord j = - size_.border(); j; ++j)
	    for (coord k = 0; k <= kmax; ++k)
	      {
		at_(i, j, k)        = at_(i, 0, k);
		at_(i, jmax - j, k) = at_(i, jmax, k);
	      }
	// left & right
	for (coord i = - size_.border(); i <= imax + size_.border(); ++i)
	  for (coord j = - size_.border(); j <= jmax + size_.border(); ++j)
	    for (coord k = - size_.border(); k; ++k)
	      {
		at_(i, j, k)        = at_(i, j, 0);
		at_(i, j, kmax - k) = at_(i, j, kmax);
	      }
      }

      void border_mirror_(void)
      {
	const coord imax = size_.nslices() - 1;
	const coord jmax = size_.nrows() - 1;
	const coord kmax = size_.ncols() - 1;
	// front & rear
	for (coord i = - size_.border(); i; ++i)
	  for (coord j = 0; j <= jmax; ++j)
	    for (coord k = 0; k <= kmax; ++k)
	      {
		at_(i, j, k)        = at_(- i, j, k);
		at_(imax - i, j, k) = at_(imax + i, j, k);
	      }
	// top & bottom
	for (coord i = - size_.border(); i <= imax + size_.border(); ++i)
	  for (coord j = - size_.border(); j; ++j)
	    for (coord k = 0; k <= kmax; ++k)
	      {
		at_(i, j, k)        = at_(i, - j, k);
		at_(i, jmax - j, k) = at_(i, jmax + j, k);
	      }
	// left & right
	for (coord i = - size_.border(); i <= imax + size_.border(); ++i)
	  for (coord j = - size_.border(); j <= jmax + size_.border(); ++j)
	    for (coord k = - size_.border(); k; ++k)
	      {
		at_(i, j, k)        = at_(i, j, - k);
		at_(i, j, kmax - k) = at_(i, j, kmax + k);
	      }
      }

      void border_assign_(value_type val)
      {
	const coord imax = size_.nslices() - 1;
        const coord jmax = size_.nrows() - 1;
        const coord kmax = size_.ncols() - 1;
        // front & rear
        for (coord i = - size_.border(); i; ++i)
          for (coord j = 0; j <= jmax; ++j)
            for (coord k = 0; k <= kmax; ++k)
              {
                at_(i, j, k)        = val;
                at_(imax - i, j, k) = val;
              }
        // top & bottom
        for (coord i = - size_.border(); i <= imax + size_.border(); ++i)
          for (coord j = - size_.border(); j; ++j)
            for (coord k = 0; k <= kmax; ++k)
              {
                at_(i, j, k)        = val;
                at_(i, jmax - j, k) = val;
              }
        // left & right
        for (coord i = - size_.border(); i <= imax + size_.border(); ++i)
          for (coord j = - size_.border(); j <= jmax + size_.border(); ++j)
            for (coord k = - size_.border(); k; ++k)
              {
                at_(i, j, k)        = val;
                at_(i, j, kmax - k) = val;
              }
      }



    private:
      T** array2_;
      T*** array_;
    };

  } // end of namespace impl

} // end of namespace oln

#endif // ! OLENA_CORE_IMPL_IMAGE_ARRAY3D_HH
