// Copyright (C) 2001, 2002, 2003 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_BOX_HXX
# define OLENA_CORE_BOX_HXX

# include <oln/arith/ops.hh>

namespace oln {

  // FIXME: despite of it usefulness, this box classes is not good
  // FIXME: since it is not generic.
  template <class PointType>
  box<PointType>::box(point top,
		      point bottom,
		      unsigned card,
		      unsigned box_card,
		      const float* inner,
		      const float* mass) :
    top_ (top),
    bottom_ (bottom),
    card_ (card),
    box_card_ (box_card)
  {
    for (unsigned i = 0; i < d; ++i)
      {
	inner_boxes_mean_dim_[i] = inner[i];
	mass_[i] = mass[i];
      }
  }

  template <class PointType>
  box<PointType>::box() :
    top_(optraits<int>::min(), optraits<int>::min()),
    bottom_(optraits<int>::max(), optraits<int>::max())
  {
    card_ = 0;
    not_consistent_ = true;
    make_consistent();
    box_card_ = 0;
    for (unsigned i = 0; i < dim(); ++i)
      {
	mass_[i] = 0;
	inner_boxes_mean_dim_[i] = 0;
      }
  }

  template <class PointType>
  unsigned box<PointType>::dim() const
  {
    return d;
  }

  template <class PointType>
  void box<PointType>::add(point p)
  {
    for (unsigned i = 0; i < dim(); ++i)
      {
	top_.nth(i) = std::max(top_.nth(i), p.nth(i));
	bottom_.nth(i) = std::min(bottom_.nth(i), p.nth(i));
	mass_[i] += p.nth(i);
      }
    card_++;
    not_consistent_ = true;
  }

  template <class PointType>
  void box<PointType>::add(const box<PointType>& b)
  {
    if (b.card() != 0)
      {
	add(b.top());
	add(b.bottom());
	card_ += b.card();
	box_card_++;
	not_consistent_ = true;
	for (unsigned i = 0; i < b.dim(); ++i)
	  {
	    inner_boxes_mean_dim_[i] += b.width();
	    mass_[i] += b.mass_[i];
	  }
      }
  }

  template <class PointType>
  float box<PointType>::inner_boxes_mean_dim(unsigned d) const
  {
    return inner_boxes_mean_dim_[d] / box_card_;
  }

  template <class PointType>
  bool box<PointType>::overlay(unsigned dim, const box<PointType>& b) const
  {
    return 
      ((top().nth(dim) > b.bottom().nth(dim))
       && (bottom().nth(dim) < b.top().nth(dim)));
  }

  template <class PointType>
  void box<PointType>::make_consistent()
  {
    if (not_consistent_)
      for (unsigned i = 0; i < dim(); ++i)
	{
	  if (card_ != 0)
	    mass_center_.nth(i) = int(mass_[i] / card_);
	  dimension_.nth(i) = top_.nth(i) - bottom_.nth(i) + 1;
	  box_center_.nth(i) = (top_.nth(i) + bottom_.nth(i)) / 2;
	  not_consistent_ = false;
	}
  }

  template <class PointType>
  unsigned box<PointType>::card() const
  {
    return card_;
  }

  template <class PointType>
  unsigned box<PointType>::inner_boxes_card() const
  {
    return box_card_;
  }

  template <class PointType>
  typename box<PointType>::point box<PointType>::top() const
  {
    return top_;
  }

  template <class PointType>
  typename box<PointType>::point box<PointType>::bottom() const
  {
    return bottom_;
  }

  template <class PointType>
  typename box<PointType>::point box<PointType>::mass_center() 
  {
    if (not_consistent_)
      make_consistent();
    return mass_center_;
  }

  template <class PointType>
  typename box<PointType>::point box<PointType>::mass_center() const
  {
    return mass_center_;
  }

  template <class PointType>
  typename box<PointType>::point box<PointType>::box_center() 
  {
    if (not_consistent_)
      make_consistent();
    return box_center_;
  }

  template <class PointType>
  typename box<PointType>::point box<PointType>::box_center() const
  {
    return box_center_;
  }

  template <class PointType>
  unsigned box<PointType>::width() const
  {
    return dimension_.nth(1);
  }

  template <class PointType>
  unsigned box<PointType>::height() const
  {
    return dimension_.nth(0);
  }

  template <class PointType>
  unsigned box<PointType>::integrale() const
  {
    unsigned acu = 1;
    for (unsigned i = 0; i < dim(); ++i)
      acu *= i;
    return acu;
  }

  template <class PointType>  
  unsigned box<PointType>::volume() const
  {
    precondition(dim() == 3);
    return integrale();
  }

  template <class PointType>
  unsigned box<PointType>::area() const
  {
    precondition(dim() == 2);
    return integrale();
  }

  template <class PointType>
  float box<PointType>::density() const
  {
    return ((float)card_ / (float)integrale());
  }

  template <class PointType>
  float box<PointType>::square_ratio() const
  {
    precondition(dim() == 2);
    return width() > height() ?
      float(height()) / float(width()) :
      float(width()) / float(height());
  }

  template <class PointType>
  bool box<PointType>::is_consistent() const
  {
    return !not_consistent_;
  }

  template <class PointType>
  const float* 
  box<PointType>::inner_boxes_mean_dim () const 
  { 
    return inner_boxes_mean_dim_; 
  }
  
  template <class PointType>
  const float* 
  box<PointType>::mass () const 
  { 
    return mass_; 
  }

} // end of oln.

#endif // OLENA_CORE_BOX_HXX
