// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
///

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_BBOX2D_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_BBOX2D_HH

# include <mln/core/alias/point2d.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;

	class bbox2d
	{
	public:
	  bbox2d();

	  bbox2d(const unsigned n,
		 const point2d& pmin,
		 const point2d& pmax);

	  point2d& pmin();

	  const point2d& pmin() const;

	  point2d& pmax();

	  const point2d& pmax() const;

	  point2d pcenter() const;

	  unsigned bbox_number() const;

	  void set_bbox_number(const unsigned id);

	private:
	  unsigned n_;
	  point2d pmin_;
	  point2d pmax_;
	};


# ifndef MLN_INCLUDE_ONLY

	//---------------------------
	// Bounding box 2D
	//---------------------------

	inline
	bbox2d::bbox2d()
	  : n_(0), pmin_(point2d(0, 0)), pmax_(point2d(0, 0))
	{
	}

	inline
	bbox2d::bbox2d(const unsigned n,
		       const point2d& pmin,
		       const point2d& pmax)
	  : n_(n), pmin_(pmin), pmax_(pmax)
	{
	}

	inline
	point2d&
	bbox2d::pmin()
	{
	  return pmin_;
	}

	inline
	const point2d&
	bbox2d::pmin() const
	{
	  return pmin_;
	}

	inline
	point2d&
	bbox2d::pmax()
	{
	  return pmax_;
	}

	inline
	const point2d&
	bbox2d::pmax() const
	{
	  return pmax_;
	}

	inline
	point2d
	bbox2d::pcenter() const
	{
	  point2d p(pmin_.row() + (pmax_.row() - pmin_.row()) / 2,
		    pmin_.col() + (pmax_.col() - pmin_.col()) / 2);

	  return p;
	}

	inline
	unsigned
	bbox2d::bbox_number() const
	{
	  return n_;
	}

	inline
	void
	bbox2d::set_bbox_number(const unsigned id)
	{
	  n_ = id;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_BBOX2D_HH
