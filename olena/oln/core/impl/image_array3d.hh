// Copyright (C) 2001, 2003, 2004  EPITA Research and Development Laboratory
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
# include <oln/core/image3d_size.hh>
# include <oln/core/point3d.hh>

namespace oln {

   /// Build an image data array with the real data and the border.
  
  template<class T>
  void 
  pretreat_3d_data_(T*& buffer, T**& array2, T***& array,
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

  /// Free the image3d data array.
  
  template<class T>
  void 
  desallocate_3d_data_(T**& array2, T***& array, const
		       image3d_size& s)
  {
    delete[] array2;
    array2 = 0;  // security
    array -= s.border();
    delete[] array;
    array = 0;  // security
  }


  namespace impl {
    template<class T>
    class image_array3d;
  } // end of impl

  /*! \class impl_traits<impl::image_array3d<T> >
  **
  ** Specialized version for impl::image_array3d<T>. Retrieve
  ** associated types.
  */
  
  template<class T>
  struct impl_traits<impl::image_array3d<T> >: public impl_traits<impl::image_array<T, impl::image_array3d<T> > >
  {
    enum { dim = 3 };
    typedef point3d point_type;
    typedef image3d_size size_type;
    typedef T value_type;
  };

  namespace impl
  {

    /*! \class image_array3d
    **
    ** Data array implementation for image3d
    */ 
    
    template<class T>
    class image_array3d :
      public image_array<T, image_array3d<T> >
    {

    public:

      typedef image_array3d<T> self_type;
      typedef image_array3d<T> exact_type;

      typedef typename impl_traits<exact_type>::point_type point_type;
      typedef typename impl_traits<exact_type>::value_type value_type;
      typedef typename impl_traits<exact_type>::size_type size_type;

      typedef image_array<T, image_array3d<T> > super_type;
      

      friend class image_impl<image_array3d<T> >;
      friend class image_array<T, image_array3d<T> >;

      image_array3d(const size_type& s): super_type(s) 
      {
	pretreat_3d_data_(this->buffer_, array2_, array_, s);
      }

      ~image_array3d()
      {
	desallocate_3d_data_(array2_, array_, this->size_);
      }

    protected:
      
      /// Return true if \a p belongs to the image.
      
      bool 
      hold_(const point_type& p) const
      {
	return (p.slice() >= 0
		&& p.slice() < this->size_.nslices()
		&& p.row() >= 0
		&& p.row() < this->size_.nrows()
		&& p.col() >= 0
		&& p.col() < this->size_.ncols());
      }

      /// Return true if \a p belongs to the image or the image border

      bool 
      hold_large_(const point_type& p) const
      {
	return (p.slice() >= -this->size_.border()
		&& p.slice() < this->size_.nslices() + this->size_.border()
		&& p.row() >= - this->size_.border()
		&& p.row() < this->size_.nrows() + this->size_.border()
		&& p.col() >= - this->size_.border()
		&& p.col() < this->size_.ncols() + this->size_.border());
      }

      /// Return a reference to the value stored at \a p. 

      value_type& 
      at_(const point_type& p)
      {
	
	return at_(p.slice(), p.row(), p.col());
      }
      
      /// Return a reference to the value stored at \a slice, \a row and \a col.

      value_type& 
      at_(coord slice, coord row, coord col)
      {
	invariant(this->buffer_ != 0);
	precondition_hold_large(point_type(slice, row, col));
	return array_[slice][row][col];
      }
      
      /// Return the total size of the data array.
      
      size_t 
      len_(const size_type& s) const
      {
	coord nslices_eff = s.nslices() + 2 * s.border();
	coord ncols_eff = s.ncols() + 2 * s.border();
	coord nrows_eff = s.nrows() + 2 * s.border();
	return size_t(nslices_eff * nrows_eff * ncols_eff);
      }

      // borders

      /*! \brief Reallocate the border regarding to the value of \a
      ** new_border.
      */
      
      void 
      border_reallocate_and_copy_(coord new_border, bool
				  copy_border) 
      {
	T* buffer = 0;
	T** array2 = 0;
	T*** array = 0;
	// first allocate

	allocate_data_(buffer, len_(size_type(this->size_.nslices(), this->size_.nrows(),
					      this->size_.ncols(), new_border)));
	pretreat_3d_data_(buffer, array2, array, size_type(this->size_.nslices(), this->size_.nrows(),
							     this->size_.ncols(), new_border));
	// move data
	coord border = this->size_.border();
	if (border > new_border)
	  border = new_border;
	coord src_min_slice = copy_border ? -border : 0;
	coord src_max_slice = this->size_.nslices() + (copy_border ? border : 0);
	coord src_min_row = copy_border ? -border : 0;
	coord src_max_row = this->size_.nrows() + (copy_border ? border : 0);
	coord src_min_col = copy_border ? -border : 0;
	coord src_ncols = this->size_.ncols() + (copy_border ? (border * 2) : 0);
	for (coord slice = src_min_slice; slice < src_max_slice; ++slice)
	  for (coord row = src_min_row; row < src_max_row; ++row)
	    memcpy(array[slice][row],
		   &at_(slice, row, src_min_col),
		   src_ncols * sizeof(T));

	// then replace
	desallocate_data_(this->buffer_);
	desallocate_3d_data_(array2_, array_, this->size_);
	this->size_.border() = new_border;
	this->buffer_ = buffer;
	array2_ = array2;
	array_ = array;
      }
      
      /*! \brief The border points are all set to 
      ** the value of the closest image point.
      */
      
      void 
      border_replicate_(void) 
      {
	const coord imax = this->size_.nslices() - 1;
	const coord jmax = this->size_.nrows() - 1;
	const coord kmax = this->size_.ncols() - 1;
	// front & rear
	for (coord i = - this->size_.border(); i; ++i)
	  for (coord j = 0; j <= jmax; ++j)
	    for (coord k = 0; k <= kmax; ++k)
	      {
		at_(i, j, k)        = at_(0, j, k);
		at_(imax - i, j, k) = at_(imax, j, k);
	      }
	// top & bottom
	for (coord i = - this->size_.border(); i <= imax + this->size_.border(); ++i)
	  for (coord j = - this->size_.border(); j; ++j)
	    for (coord k = 0; k <= kmax; ++k)
	      {
		at_(i, j, k)        = at_(i, 0, k);
		at_(i, jmax - j, k) = at_(i, jmax, k);
	      }
	// left & right
	for (coord i = - this->size_.border(); i <= imax + this->size_.border(); ++i)
	  for (coord j = - this->size_.border(); j <= jmax + this->size_.border(); ++j)
	    for (coord k = - this->size_.border(); k; ++k)
	      {
		at_(i, j, k)        = at_(i, j, 0);
		at_(i, j, kmax - k) = at_(i, j, kmax);
	      }
      }

      /*! \brief The border points are set by mirroring
      ** the image edges.
      */
      
      void 
      border_mirror_(void) 
      {
	const coord imax = this->size_.nslices() - 1;
	const coord jmax = this->size_.nrows() - 1;
	const coord kmax = this->size_.ncols() - 1;
	// front & rear
	for (coord i = - this->size_.border(); i; ++i)
	  for (coord j = 0; j <= jmax; ++j)
	    for (coord k = 0; k <= kmax; ++k)
	      {
		at_(i, j, k)        = at_(- i, j, k);
		at_(imax - i, j, k) = at_(imax + i, j, k);
	      }
	// top & bottom
	for (coord i = - this->size_.border(); i <= imax + this->size_.border(); ++i)
	  for (coord j = - this->size_.border(); j; ++j)
	    for (coord k = 0; k <= kmax; ++k)
	      {
		at_(i, j, k)        = at_(i, - j, k);
		at_(i, jmax - j, k) = at_(i, jmax + j, k);
	      }
	// left & right
	for (coord i = - this->size_.border(); i <= imax + this->size_.border(); ++i)
	  for (coord j = - this->size_.border(); j <= jmax + this->size_.border(); ++j)
	    for (coord k = - this->size_.border(); k; ++k)
	      {
		at_(i, j, k)        = at_(i, j, - k);
		at_(i, j, kmax - k) = at_(i, j, kmax + k);
	      }
      }

      /// The border points are set to \a val.

      void 
      border_assign_(value_type val) 
      {
	const coord imax = this->size_.nslices() - 1;
        const coord jmax = this->size_.nrows() - 1;
        const coord kmax = this->size_.ncols() - 1;
        // front & rear
        for (coord i = - this->size_.border(); i; ++i)
          for (coord j = 0; j <= jmax; ++j)
            for (coord k = 0; k <= kmax; ++k)
              {
                at_(i, j, k)        = val;
                at_(imax - i, j, k) = val;
              }
        // top & bottom
        for (coord i = - this->size_.border(); i <= imax + this->size_.border(); ++i)
          for (coord j = - this->size_.border(); j; ++j)
            for (coord k = 0; k <= kmax; ++k)
              {
                at_(i, j, k)        = val;
                at_(i, jmax - j, k) = val;
              }
        // left & right
        for (coord i = - this->size_.border(); i <= imax + this->size_.border(); ++i)
          for (coord j = - this->size_.border(); j <= jmax + this->size_.border(); ++j)
            for (coord k = - this->size_.border(); k; ++k)
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
