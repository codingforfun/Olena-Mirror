// Copyright (C) 2004  EPITA Research and Development Laboratory
//
// This  file is  part of  the Olena  Library.  This  library  is free
// software; you can redistribute it  and/or modify it under the terms
// of the  GNU General  Public License version  2 as published  by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with  this library; see the  file COPYING.  If  not, write to
// the Free Software Foundation, 59  Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a  special exception, you  may use this  file as part of  a free
// software library without restriction.  Specifically, if other files
// instantiate templates  or use macros or inline  functions from this
// file, or  you compile  this file  and link it  with other  files to
// produce  an executable,  this file  does  not by  itself cause  the
// resulting  executable  to be  covered  by  the  GNU General  Public
// License.   This exception  does  not however  invalidate any  other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_OLN_UTILS_BUFFER_HXX
# define OLENA_OLN_UTILS_BUFFER_HXX

// Ctor implementation
inline
buffer::buffer(): current_bit_(0),
		  data_(),
		  len1_(ntg_zero_val(value_type)),
		  len2_(ntg_zero_val(value_type)),
		  padding_appended_(false),
		  length_appended_(false)
{
  data_.reserve(length_);
  data_.push_back(ntg_zero_val(value_type));
}

// add implementation
template <class E>
inline
void buffer::add(const E &e, bool count)
{
  ntg_storage_type(E)	mask = 1 << (ntg_nbits(E) - 1);
  ntg_storage_type(E) e_cast = e;

  while (mask)
    {
      push_back(e_cast & mask, !count);
      mask >>= 1;
    }
}

// push_back implementation
inline
void buffer::push_back(bool bit, bool is_padding)
{
  precondition(current_bit_ < 32);

  current_bit_ += 1;
  data_.back() *= 2;
  if (bit)
    data_.back() += ntg_unit_val(value_type);
  if (current_bit_ == 32)
    {
      if (data_.size() == data_.capacity())
	data_.reserve(data_.capacity() + length_);
      current_bit_ = 0;
      data_.push_back(ntg_zero_val(value_type));
    }
  if (!is_padding)
    {
      if (len1_ == ntg_max_val(value_type))
	{
	  if (len2_ == ntg_max_val(value_type))
	    len2_ = ntg_unit_val(value_type);
	  else
	    len2_ += ntg_unit_val(value_type);
	}
      len1_ += ntg_unit_val(value_type);
    }
}

// reorder implementation
inline
buffer::value_type buffer::reorder(value_type x) const
{
  ntg_storage_type_(value_type)	r = 0;
  ntg_storage_type_(value_type)	mask = 0xFF;

  for (unsigned i = 0; i < (sizeof(ntg_storage_type_(value_type))); ++i)
    {
      r <<= 8;
      r |= (mask & ntg_storage_type_(value_type)(x)) >> (i * 8);
      mask <<= 8;
    }
  return r;
}

// [] operator implementation
inline
ntg::int_u32 buffer::operator[] (unsigned n) const
{
  precondition(n < data_.size());

  return reorder(data_[n]);
}

// append padding implementation
inline
void buffer::append_padding()
{
  push_back(true, true);
  while (((data_.size() * 32) % 512) != 448)
    push_back(false, true);
  padding_appended_ = true;
}

// append_length implementation
inline
void buffer::append_length()
{
  precondition(padding_appended_);

  add(reorder(len2_), false);
  add(reorder(len1_), false);
  length_appended_ = true;
}

// size implementation.
inline
unsigned buffer::size() const
{
  return data_.size();
}

// bit len implementation.
inline
unsigned buffer::bit_len() const
{
  return data_.size() * 32;
}

// is_append_padding implementation.
inline
bool buffer::is_append_padding() const
{
  return padding_appended_;
}

// is append_length implementation.
inline
bool buffer::is_append_length() const
{
  return length_appended_;
}

#endif // !OLENA_OLN_UTILS_BUFFER_HXX
