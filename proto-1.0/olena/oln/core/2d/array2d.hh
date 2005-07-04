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

#ifndef OLENA_CORE_2D_ARRAY2D_HH
# define OLENA_CORE_2D_ARRAY2D_HH


# include <oln/core/abstract/data_storage.hh>
# include <oln/core/2d/size2d.hh>
# include <oln/core/2d/point2d.hh>

namespace oln {

  // fwd decl
  template <typename T> struct array2d;

  // props
  template <typename T>
  struct set_props < category::data_storage, array2d<T> >
  {
    typedef size2d  size_type;
    typedef point2d point_type;
    typedef T       data_type;
  };


  template<class T>
  void alloc_and_init(T*& buffer, T**& array, const size2d& s)
  {
    size_t nrows_eff = s.nrows() + 2 * s.border();
    size_t ncols_eff = s.ncols() + 2 * s.border();
    size_t nelts_eff = nrows_eff * ncols_eff;

    buffer = new T[nelts_eff];
    array = new T*[nrows_eff];

    T* buf = buffer + s.border();
    for (size_t row = 0; row < nrows_eff; ++row)
      {
	array[row] = buf;
	buf += ncols_eff;
      }
    array += s.border();
  }

  template <typename T>
  class array2d : public abstract::data_storage< array2d<T> >
  {

  public:

    array2d() :
      buffer_(0),
      array_(0),
      size_()
    {
      invariant_();
    }

    // w/o impl
    array2d(const array2d&);
    void operator=(const array2d&);


    array2d(const size2d& s) :
      buffer_(0),
      array_(0),
      size_()
    {
      this->resize(s);
    }

    ~array2d()
    {
      this->clear_data();
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
	  // array
	  array_ -= size_.border();
	  delete[] array_;
	  array_ = 0;
	  // size
	  size_ = size2d();
	}
      invariant_();
    }

    const size2d& impl_size() const
    {
      return size_;
    }

    void impl_resize(const size2d& s)
    {
      precondition(s.nrows() > 0 &&
		   s.ncols() > 0 &&
		   s.border() >= 0);
      invariant_();
      this->clear_data();
      size_ = s;

      alloc_and_init(buffer_, array_, s);
      invariant_();
    }

    void impl_resize_border(size_t new_border, bool copy_border)
    {
      invariant_();
      T* new_buffer_;
      T** new_array_;
      size2d new_size_(this->size_.nrows(), this->size_.ncols(), new_border);

      alloc_and_init(new_buffer_, new_array_, new_size_);

      if (buffer_ != 0)
	{
	  size_t border = this->size_.border();

	  if (border > new_border)
	    border = new_border;

	  coord_t row_min = copy_border ? -border : 0;
	  coord_t row_max = int(this->size_.nrows())
	    + (copy_border ? border : 0);
	  coord_t col_min = copy_border ? -border : 0;
	  size_t ncols = int(this->size_.ncols()) +
	    (copy_border ? (border * 2) : 0);

	  for (coord_t row = row_min; row < row_max; ++row)
	    memcpy(new_array_[row] + col_min, this->array_[row] + col_min,
		   ncols * sizeof (T));

	  this->clear_data();
	}

      buffer_ = new_buffer_;
      array_ = new_array_;
      size_ = new_size_;

      invariant_();
    }

    unsigned long impl_npoints() const
    {
      return size_.npoints();
    }

    bool impl_hold(const point2d& p) const
    {
      return
	p.row() >= 0 &&
	p.row() < size_.nrows() &&
	p.col() >= 0 &&
	p.col() < size_.ncols();
    }

    bool impl_hold_large(const point2d& p) const
    {
      return
	p.row() >= - size_.border() &&
	p.row() < size_.nrows() + size_.border() &&
	p.col() >= - size_.border() &&
	p.col() < size_.ncols() + size_.border();
    }

    const T impl_get(const point2d& p) const // FIXME: remove (obsolete)
    {
      invariant_();
      return array_[p.row()][p.col()];
    }

    void impl_set(const point2d& p, const T& v) // FIXME: remove (obsolete)
    {
      invariant_();
      array_[p.row()][p.col()] = v;
    }

    const T& impl_at(const point2d& p) const
    {
      invariant_();
      return array_[p.row()][p.col()];
    }

    T& impl_at(const point2d& p)
    {
      invariant_();
      return array_[p.row()][p.col()];
    }

    // FIXME: should be impl_ here; and call should be defined in storage_type
    template <typename T2, typename A, typename V>
    void call(const point2d& p,
	      void (T2::*method)(A),
	      V arg)
    {
      T& value = array_[p.row()][p.col()];
      (value.*method)(arg);
    }

    void impl_set_data(const T& v)
    {
      invariant_();
      const size_t nrows_eff = size_.nrows() + 2 * size_.border();
      const size_t ncols_eff = size_.ncols() + 2 * size_.border();
      const size_t len = nrows_eff * ncols_eff;
      T* p = buffer_;
      for (size_t i = 0; i < len; ++i)
	*p++ = v;
    }

  private:

    T* buffer_;
    T** array_;
    size2d size_;

    void invariant_() const
    {
      invariant((buffer_ != 0 &&
		 array_ != 0 &&
		 size_.nrows() > 0 &&
		 size_.ncols() > 0 &&
		 size_.border() >= 0)
		||
		(buffer_ == 0 &&
		 array_ == 0 &&
		 size_.nrows().is_undefined() &&
		 size_.ncols().is_undefined() &&
		 size_.border().is_undefined()));
    }



  };

} // end of namespace oln


#endif // ! OLENA_CORE_2D_ARRAY2D_HH
