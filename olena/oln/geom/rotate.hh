// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_GEOM_ROTATE_HH
# define OLENA_GEOM_ROTATE_HH

# include <cmath>
# include <iostream>

# include <oln/core/image2d.hh>
# include <oln/core/fpoint2d.hh>

namespace oln {

  namespace geom {

    template <class T, class Access>
    struct ftor_rotate
    {
    private:
      fpoint2d_access<T, Access> interp;
      point2d center;
      double cos_angle, sin_angle, a, b;

    public:
      ftor_rotate(const point2d& center_,
		   const fpoint2d_access<T, Access>& interp_,
		   const double rad_angle_) :
	interp(interp_),
	center(center_)
      {
	cos_angle = cos(-rad_angle_);
	sin_angle = sin(-rad_angle_);

	std::cout << sin_angle << " " << cos_angle << std::endl;

	a = double(center.row());
	b = double(center.col());

      }

      ftor_rotate() {}

      ftor_rotate(const ftor_rotate<T, Access>& rhs)
      {
	interp = rhs.interp;
	center = rhs.center;
	cos_angle = rhs.cos_angle;
	sin_angle = rhs.sin_angle;
	a = rhs.a;
	b = rhs.b;
      }

      const T
      operator()(const image2d<T>& ima,
		 const point2d p) const
      {
	double x, y;
	double c, d;

	c = a + ima.size().nrows() / 2;
	d = b + ima.size().ncols() / 2;

	x = cos_angle * double(p.row()) - sin_angle * double(p.col()) -
	  c * cos_angle + d * sin_angle + c;
	y = sin_angle * double(p.row()) + cos_angle * double(p.col()) -
	  c * sin_angle - d * cos_angle + d;

	return interp(ima, fpoint2d<double>(x, y));
      }
    };

    template <class T, class Access>
    struct rotate
    {
      typedef image2d<T> im_type;

      im_type
      operator()(const im_type& ima,
		 const point2d center,
		 double rad_angle,
		 fpoint2d_access<T, Access> interp)
      {
	im_type res(ima.size());
	ftor_rotate<T, Access> f(center, interp, rad_angle);
	oln_iter_type(im_type) it(res);

	for_all(it)
	{
	  res[it] = f(ima, point2d(it.row(), it.col()));
	}
	return res;
      }
    };


    template <class T, class Access>
    struct ftor_rotate_enlarge
    {
    private:
      fpoint2d_access<T, Access> interp;
      point2d center;
      double cos_angle;
      double sin_angle;
      double a;
      double b;

    public:
      ftor_rotate_enlarge(const point2d& center_,
			  const fpoint2d_access<T, Access>& interp_,
			  const double rad_angle_) :
	interp(interp_),
	center(center_)
      {
	cos_angle = cos(-rad_angle_);
	sin_angle = sin(-rad_angle_);
	a = double(center.row());
	b = double(center.col());
      }

      ftor_rotate_enlarge()
      {}

      ftor_rotate_enlarge(const ftor_rotate_enlarge& rhs) :
	interp(rhs.interp),
	center(rhs.center)
      {
	cos_angle = rhs.cos_angle;
	sin_angle = rhs.sin_angle;
	a = rhs.a;
	b = rhs.b;
      }

      T
      operator()(const image2d<T>& ima,
		 const point2d p)
      {
	double x, y;
	double c, d;
	double rrows = ima.size().nrows() * cos_angle +
	  ima.size().ncols() * -sin_angle;
	double rcols = ima.size().nrows() * -sin_angle +
	  ima.size().ncols() * cos_angle;
	double vt = (rrows - ima.size().nrows()) / 2;
	double ht = (rcols - ima.size().ncols()) / 2;

	c = a + ima.size().nrows() / 2;
	d = b + ima.size().ncols() / 2;

	x = cos_angle * double(p.row() - vt) - sin_angle * double(p.col() - ht) -
	  c * cos_angle + d * sin_angle + c;
	y = sin_angle * double(p.row() - vt) + cos_angle * double(p.col() - ht) -
	  c * sin_angle - d * cos_angle + d;

	return interp(ima, fpoint2d<double>(x, y));
      }
    };




    template <class T, class Access>
    struct rotate_enlarge
    {

      typedef image2d<T> im_type;

      im_type
      operator()(const im_type& ima,
		 const point2d center,
		 double rad_angle,
		 fpoint2d_access<T, Access> interp)
      {
	im_type res(image2d_size(int(ima.size().nrows() * cos(rad_angle) +
					ima.size().ncols() * sin(rad_angle)),
				    int(ima.size().nrows() * sin(rad_angle) +
					ima.size().ncols() * cos(rad_angle)),
				    0));

	oln_iter_type(im_type) it(res);
	ftor_rotate_enlarge<T, Access> f(center, interp, rad_angle);

	for_all(it)
	  {
	    res[it] = f(ima, point2d(it.row(), it.col()));
	  }
	return res;
      }
    };

  }
}


#endif // !OLENA_CORE_FPOINT2D_HH
