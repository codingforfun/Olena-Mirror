// Copyright (C) 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_SNAKES_SNAKES_BASE_HH
# define OLENA_SNAKES_SNAKES_BASE_HH

#include <oln/snakes/segment.hh>

namespace oln {

  /*! \brief oln::snakes::snake implementation.
  */
  namespace snakes {
    /*! Snake algorithm.
    **
    ** \todo FIXME: Do not work due to the function node::energy.
    ** \todo FIXME: Add doc & test.
    */
    template <class algorithm>
    class snake
    {
    public:
      typedef typename algorithm::image_type	image_type;
      typedef typename image_type::point_type	point_type;

    public:
      snake(const image_type&		image,
	    std::list<point_type>	initial_contour,
	    ntg::float_s		alpha,
	    ntg::float_s		beta,
	    ntg::float_s		gamma,
	    ntg::float_s		khi);

    public:
      std::list<point_type>
      contour(void) const;
      ///< Return the points of the snake.

      ntg::float_s
      energy(void) const;
      ///< Return the snake energy. This is not algorithm-dependant.
      ///< \todo FIXME: Do not work due to the function node::energy

    public:
      int
      update_snake(void);
      ///< Calling this method causes the snake to execute one
      ///< step. If the method is not iterative, it should fail
      ///< to compile.

      void
      converge(void);
      ///< Calling this method causes the snake to converge. It
      ///< does so by delegating the method to the algorithm.

    private:
      segment<image_type> s;
      ///< The current point list, I mean the one that is returned
      ///< when nodes() is called and the one on which energy() computes
      ///< the global energy. Implementations that use several segments
      ///< should just do so in the algorithm class.

      algorithm a;
      ///< This one is just the instance that holds algorithm-specific
      //< information, that is to say nearly every information.

    private:
      ///< Image gradient.
      image_type gradient;

    private:
      ///< Each energy is weighted by a its own coefficient.
      ntg::float_s alpha; ///< Weight of the continuity energy.
      ntg::float_s beta;  ///< Weight of the curvature energy.
      ntg::float_s gamma; ///< Weight of the image energy.
      ntg::float_s khi;   ///< Weight of the image energy.

    private:
      friend
      int
      algorithm::update_snake(const typename algorithm::image_type&, snake&);

      friend
      void
      algorithm::converge(const typename algorithm::image_type&, snake&);

      friend
      std::ostream&
      ::operator<< <>(std::ostream&, const snake&);
    };

  } // end snakes

} // end oln

template <class algorithm>
std::ostream& operator<<(std::ostream& os,
			 const oln::snakes::snake<algorithm>& s)
{
  os << "Snake:" << std::endl;
  os << s.s;
  return os;
}

#include <oln/snakes/snakes_base.hxx>

#endif // !OLENA_SNAKES_SNAKES_BASE_HH
