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

#ifndef OLENA_CORE_1D_ARRAY1D_HH
# define OLENA_CORE_1D_ARRAY1D_HH

# include <oln/core/abstract/data_storage.hh>
# include <oln/core/1d/size1d.hh>
# include <oln/core/1d/point1d.hh>


namespace oln {

  // fwd decl
  template <typename T> struct array1d;

  // super type
  template <typename T>
  struct set_super_type < array1d<T> > { typedef abstract::data_storage< array1d<T> > ret; };

  // props
  template <typename T>
  struct set_props < category::data_storage, array1d<T> >
  {
    typedef size1d  size_type;
    typedef point1d point_type;
    typedef T       data_type;
  };


  template<class T>
  void alloc_and_init(T*& buffer, const size1d& s)
  {
    size_t nelts_eff = s.nindices() + 2 * s.border();
    buffer = new T[nelts_eff];
    buffer += s.border();
  }


  template <typename T>
  class array1d : public abstract::data_storage< array1d<T> >
  {

  public:

    array1d() :
      buffer_(0),
      size_()
    {
      invariant_();
    }

    // w/o impl
    array1d(const array1d&);
    void operator=(const array1d&);


    array1d(const size1d& s) :
      buffer_(0),
      size_()
    {
      this->resize(s);
    }

    ~array1d()
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
	  buffer_ -= size_.border();
	  delete[] buffer_;
	  buffer_ = 0;
	  // size
	  size_ = size1d();
	}
      invariant_();
    }

    const size1d& impl_size() const
    {
      return size_;
    }

    void impl_resize(const size1d& s)
    {
      precondition(s.nindices() > 0 &&
		   s.border() >= 0);
      invariant_();
      this->clear_data();
      size_ = s;

      alloc_and_init(buffer_, s);
      invariant_();
    }

    void impl_resize_border(size_t new_border, bool copy_border)
    {
      invariant_();
      T* new_buffer_;
      size1d new_size_(this->size_.nindices(), new_border);

      alloc_and_init(new_buffer_, new_size_);

      if (buffer_ != 0)
	{
	  size_t border = this->size_.border();

	  if (border > new_border)
	    border = new_border;

	  coord_t indice_min = copy_border ? -border : 0;
	  coord_t indice_max = int(this->size_.nindices())
	    + (copy_border ? border : 0);
	  size_t nindices = int(this->size_.nindices()) +
	    (copy_border ? (border * 2) : 0);

	  memcpy(new_buffer_ + indice_min, this->buffer_ + indice_min,
		 nindices * sizeof (T));

	  this->clear_data();
	}

      buffer_ = new_buffer_;
      size_ = new_size_;

      invariant_();
    }

    unsigned long impl_npoints() const
    {
      return size_.npoints();
    }

    bool impl_hold(const point1d& p) const
    {
      return
	p.index() >= 0 &&
	p.index() < size_.nindices();
    }

    bool impl_hold_large(const point1d& p) const
    {
      return
	p.index() >= - size_.border() &&
	p.index() < size_.nindices() + size_.border();
    }

    const T impl_get(const point1d& p) const
    {
      invariant_();
      return buffer_[p.index()];
    }

    void impl_set(const point1d& p, const T& v)
    {
      invariant_();
      buffer_[p.index()] = v;
    }

    void impl_set_data(const T& v)
    {
      invariant_();
      const size_t len = size_.nindices() + 2 * size_.border();
      T* p = buffer_;
      for (size_t i = 0; i < len; ++i)
	*p++ = v;
    }

  private:

    T* buffer_;
    size1d size_;

    void invariant_() const
    {
      invariant((buffer_ != 0 &&
		 size_.nindices() > 0 &&
		 size_.border() >= 0)
		||
		(buffer_ == 0 &&
		 size_.nindices().is_undefined() &&
		 size_.border().is_undefined()));
    }

  };

} // end of namespace oln


#endif // ! OLENA_CORE_1D_ARRAY1D_HH
