// Copyright (C) 2001, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_3D_ARRAY3D_HH
# define OLENA_CORE_3D_ARRAY3D_HH


# include <oln/core/abstract/data_storage.hh>
# include <oln/core/3d/size3d.hh>
# include <oln/core/3d/point3d.hh>

namespace oln {


  template <typename T> struct array3d;
  template <typename T> 
  struct category_type< array3d<T> > { typedef cat::data_storage ret; };


  template <typename T>
  struct props < cat::data_storage, array3d<T> > // FIXME: add inheritance
  {
    typedef size3d  size_type;
    typedef point3d point_type;
    typedef T       data_type;
  };


  template <typename T>
  class array3d : public abstract::data_storage< array3d<T> >
  {

  public:

    array3d() :
      buffer_(0),
      array_(0),
      array2_(0),
      size_()
    {
      this->exact_ptr = this;
      invariant_();
    }

    // w/o impl
    array3d(const array3d&);
    void operator=(const array3d&);


    array3d(const size3d& s) :
      buffer_(0),
      array_(0),
      array2_(0),
      size_()
    {
      this->exact_ptr = this;
      this->resize(s);
    }

    ~array3d()
    {
      this->impl_clear_data();
    }

    bool impl_has_data() const
    {
      invariant_();
      return buffer_ != 0;
    }

    void impl_clear_data()
    {
      invariant_();
      if (this->has_data())
	{
	  // buffer
	  delete[] buffer_;
	  buffer_ = 0;
          // array2
          delete[] array2_;
          array2_ = 0;
	  // array
	  array_ -= size_.border();
	  delete[] array_;
	  array_ = 0;
	  // size
	  size_ = size3d();
	}
      invariant_();
    }

    const size3d& size() const
    {
      return size_;
    }
 
    void impl_resize(const size3d& s)
    {
      precondition(s.nslices() > 0 and
                   s.nrows() > 0 and
		   s.ncols() > 0 and
		   s.border() >= 0);
      invariant_();
      this->clear_data();
      size_ = s;

      size_t nslices_eff = size_.nslices() + 2 * size_.border();
      size_t nrows_eff = size_.nrows() + 2 * size_.border();
      size_t ncols_eff = size_.ncols() + 2 * size_.border();
      size_t nelts_eff = nslices_eff * nrows_eff * ncols_eff;

      buffer_ = new T[nelts_eff];
      array_ = new T**[nslices_eff];
      array2_ = new T*[nslices_eff * nrows_eff];

      T* buf = buffer_ + size_.border();
      for (size_t slice = 0; slice < nslices_eff; ++slice)
	{
          T** a2 = array2_ + slice * nrows_eff;
          array_[slice] = a2 + size_.border();
   	  for (size_t row = 0; row < nrows_eff; ++row)
	    {
	      a2[row] = buf;
	      buf += ncols_eff;
	    } 
	}
      array_ += size_.border();
      invariant_();
    }

    unsigned long impl_npoints() const
    {
      return size_.npoints();
    }

    bool impl_hold(const point3d& p) const
    {
      return
	p.slice() >= 0 and
	p.slice() < size_.nslices() and
	p.row() >= 0 and
	p.row() < size_.nrows() and
	p.col() >= 0 and
	p.col() < size_.ncols();
    }

    bool impl_hold_large(const point3d& p) const
    {
      return
	p.slice() >= - size_.border() and
	p.slice() < size_.nslices() + size_.border() and
	p.row() >= - size_.border() and
	p.row() < size_.nrows() + size_.border() and
	p.col() >= - size_.border() and
	p.col() < size_.ncols() + size_.border();
    }

    const T impl_get(const point3d& p) const
    {
      invariant_();
      return array_[p.slice()][p.row()][p.col()];
    }

    void impl_set(const point3d& p, const T& v)
    {
      invariant_();
      array_[p.slice()][p.row()][p.col()] = v;
    }

    void impl_set_data(const T& v)
    {
      invariant_();
      const size_t nslices_eff = size_.nslices() + 2 * size_.border();
      const size_t nrows_eff = size_.nrows() + 2 * size_.border();
      const size_t ncols_eff = size_.ncols() + 2 * size_.border();
      const size_t len = nslices_eff * nrows_eff * ncols_eff;
      T* p = buffer_;
      for (size_t i = 0; i < len; ++i)
	*p++ = v;
    }

  private:

    T* buffer_;
    T*** array_;
    T** array2_;
    size3d size_;

    void invariant_() const
    {
      invariant((buffer_ != 0 and
		 array_ != 0 and
		 size_.nslices() > 0 and
		 size_.nrows() > 0 and
		 size_.ncols() > 0 and
		 size_.border() >= 0)
		or
		(buffer_ == 0 and
		 array_ == 0 and
		 size_.nslices() == 0 and
		 size_.nrows() == 0 and
		 size_.ncols() == 0 and
		 size_.border() == 0));
    }

  };

} // end of namespace oln


#endif // ! OLENA_CORE_3D_ARRAY3D_HH
