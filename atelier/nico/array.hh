// Copyright (C) 2007 EPITA Research and Development Laboratory.
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef ARRAY_HH_
# define ARRAY_HH_


# include <cassert>

namespace oln_point
{
  template <typename value_t,
	    typename coord_t>
  class array2d
  {
  public:
    array2d(coord_t imin, coord_t jmin, coord_t imax, coord_t jmax);
    array2d(const array2d& to_cpy);
    ~array2d();


    bool has(coord_t i, coord_t j) const ;
    value_t operator() (coord_t i, coord_t j) const;
    value_t &operator() (coord_t i, coord_t j);
  private:
    void allocate_();
    void desallocate_();

    coord_t   imin_, imax_, jmin_, jmax_;
    coord_t   ilen_, jlen_;

    value_t*  buffer_;
    value_t** data_;
  };


  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::array2d(coord_t imin, coord_t jmin, coord_t imax, coord_t jmax) :
    imin_(imin), imax_(imax), jmin_(jmin), jmax_(jmax)
  {
    assert(imin < imax && jmin < jmax);

    ilen_ = imax_ - imin_ + 1;
    jlen_ = jmax_ - jmin_ + 1;
    allocate_();
  }

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::~array2d()
  {
    desallocate_();
  }

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::array2d(const array2d& to_cpy)
  {
    if (&to_cpy == this)
      return;

    desallocate_;
    imin_ = to_cpy.imin_;
    imax_ = to_cpy.imax_;
    jmin_ = to_cpy.imin_;
    jmax_ = to_cpy.ymax_;
    allocate_();

    for (coord_t i = imin_; i < imax_; ++i)
      for (coord_t j = jmin_; j < jmax_; ++j)
	data_(i, j) = to_cpy(i, j);
  }



  template <typename value_t, typename coord_t>
  bool array2d<value_t, coord_t>::has(coord_t i, coord_t j) const
  {
    return i >= imin_ && i <= imax_ && j >= jmin_ && j <= jmax_;
  }

  template <typename value_t, typename coord_t>
  value_t array2d<value_t, coord_t>::operator() (coord_t i, coord_t j) const
  {
    assert(has(i, j));

    return data_[i][j];
  }

  template <typename value_t, typename coord_t>
  value_t &array2d<value_t, coord_t>::operator() (coord_t i, coord_t j)
  {
    assert(has(i, j));

    return data_[i][j];
  }


  template <typename value_t, typename coord_t>
  void array2d<value_t, coord_t>::allocate_()
  {
    value_t* buf = 0;

    buffer_ = new value_t [size_t(ilen_) * size_t(jlen_)];
    data_ = new value_t* [size_t(ilen_)];

    buf = buffer_ - jmin_;

    for (coord_t i = 0; i < ilen_; ++i)
    {
      data_[i] = buf;
      buf += jlen_;
    }

    data_ -= imin_;
  }

  template <typename value_t, typename coord_t>
  void array2d<value_t, coord_t>::desallocate_()
  {
    assert(buffer_ != 0 and data_ != 0);

    delete[] buffer_;
    buffer_ = 0;
    data_ += imin_;
    delete[] data_;
    data_ = 0;
  }
}

#endif /* !ARRAY_HH_ */
