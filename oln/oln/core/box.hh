// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_BOX_HH
# define OLENA_CORE_BOX_HH

namespace oln {

  // FIXME: despite of it usefulness, this box classes is not good
  // FIXME: since it is not generic.

  template <class PointType>
  class box 
  {
  public:
    typedef typename PointType::dpoint  dpoint;
    typedef PointType			point;

    enum { d = point::dim };
    
    box();
    box(point		top,
	point		bottom,
	unsigned	card,
	unsigned	box_card,
	const float*	inner,
	const float*	mass);
    unsigned    dim() const;
    void	add(point p);
    void	add(const box<PointType>& p);
    bool	overlay(unsigned dim, const box<PointType>& p) const;
    void	make_consistent();
    point	mass_center();
    point	box_center();
    point	mass_center() const;
    point	box_center() const;
    point	top() const;
    unsigned	card() const;
    unsigned	inner_boxes_card() const;
    point	bottom() const;
    unsigned	width() const;
    unsigned	height() const;
    unsigned	integrale() const;
    float	inner_boxes_mean_dim(unsigned i) const;
    unsigned	volume() const;
    unsigned	area() const;
    float	density() const;
    float	square_ratio() const;
    bool	is_consistent() const;
    const float* inner_boxes_mean_dim () const;
    const float* mass () const;

  private:
    bool	not_consistent_;
    point	top_;
    point	bottom_;
    point	mass_center_;
    point	box_center_;
    float	inner_boxes_mean_dim_[point::dim];
    unsigned	card_;
    unsigned	box_card_;
    dpoint	dimension_;
    float	mass_[point::dim];
  };

} // end of oln.

# include <oln/core/box.hxx>

#endif // OLENA_CORE_BOX_HH
