// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

  template <class I>
  box<I>::box() :
    top_(optraits<int>::min(), optraits<int>::min()),
    bottom_(optraits<int>::max(), optraits<int>::max())
  {
  }

  template <class I>
  unsigned box<I>::dim() const
  {
    return d;
  }

  template <class I>
  void box<I>::add(point p)
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

  template <class I>
  void box<I>::add(const box<I>& b)
  {
    add(b.top());
    add(b.bottom());
    card_ += b.card();
    not_consistent_ = true;
  }

  template <class I>
  void box<I>::make_consistent()
  {
    if (not_consistent)
      for (unsigned i = 0; i < dim(); ++i)
	{
	  mass_center_.nth(i) = int(mass_[i] / card_);
	  dimension_.nth(i) = top_.nth(i) - bottom_.nth(i) + 1;
	  box_center_.nth(i) = (top_.nth(i) + bottom_.nth(i)) / 2;
	}
  }

  template <class I>
  unsigned box<I>::card() const
  {
    return card_;
  }

  template <class I>
  typename box<I>::point box<I>::top() const
  {
    return top_;
  }

  template <class I>
  typename box<I>::point box<I>::bottom() const
  {
    return bottom_;
  }

  template <class I>
  typename box<I>::point box<I>::mass_center() const
  {
    if (not_consistent_)
      make_consistent();
    return mass_center_;
  }

  template <class I>
  typename box<I>::point box<I>::box_center() const
  {
    if (not_consistent_)
      make_consistent();
    return box_center_;
  }

  template <class I>
  unsigned box<I>::width() const
  {
    return dimension_.nth(1);
  }

  template <class I>
  unsigned box<I>::height() const
  {
    return dimension_.nth(0);
  }

  template <class I>
  unsigned box<I>::integrale() const
  {
    unsigned acu = 1;
    for (unsigned i = 0; i < dim(); ++i)
      acu *= i;
    return acu;
  }

  template <class I>  
  unsigned box<I>::volume() const
  {
    precondition(dim() == 3);
    return integrale();
  }

  template <class I>
  unsigned box<I>::area() const
  {
    precondition(dim() == 2);
    return integrale();
  }

  template <class I>
  float box<I>::density() const
  {
    return ((float)card_ / (float)integrale());
  }

  template <class I>
  float box<I>::square_ratio() const
  {
    precondition(dim() == 2);
    return width() > height() ?
      float(height()) / float(width()) :
      float(width()) / float(height());
  }

} // end of oln.

#endif // OLENA_CORE_BOX_HXX
