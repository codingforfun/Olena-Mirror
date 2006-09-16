// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_LRDE_UFMT_ATTRIBUTES_HH
# define OLENA_LRDE_UFMT_ATTRIBUTES_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/point2d.hh>


namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {


      // FIXME: doc.

      template <class A>
      struct node_
      {
	typedef node_<A> self_t;

	A attr;  // attribute(s)
	unsigned par; // parent label

	// ctors

	node_()
	{}

	template <class I, class P>
	void init(const abstract::image<I>& f, const P& p)
	{
	  attr.init(f, p);
	}

	template <class I, class P>
	void insert(const abstract::image<I>& f, const P& p)
	{
	  // for the max-tree:
	  //   p belongs to the same node than init.p
	  //   so f(p) == f(init.p)
	  attr.insert(f, p);
	}

	void embrace(const self_t& rhs)
	{
	  // for the max-tree:
	  //   this node is the parent node of rhs
	  //   so h(this node) < h(rhs)
	  attr.embrace(rhs.attr);
	}
      };

      template <>
      struct node_ <void>
      {
	unsigned par; // parent label
      };


      struct card_t // is area for 2D images
      {
	typedef card_t self_t;

	template <class I, class P>
	void init(const I&, const P&)
	{
	  value = 1;
	}
	template <class I, class P>
	void insert(const I&, const P&)
	{
	  ++value;
	}
	void embrace(const self_t& rhs)
	{
	  value += rhs.value;
	}
	operator unsigned() const
	{
	  return value;
	}

      private:
	unsigned value;
      };


      struct mass_t // is volume for 2D images
      {
	typedef mass_t self_t;
	
	template <class I, class P>
	void init(const I& f, const P& p)
	{
	  h = f[p];
	  card = 1;
	  value = 1;
	}
	template <class I, class P>
	void insert(const I& f, const P& p)
	{
	  assert(f[p] == h);
	  ++card;
	  ++value;
	}
	void embrace(const self_t& rhs)
	{
	  assert(rhs.h > h);
	  value += (rhs.h - h) * rhs.card;
	  card += rhs.card;
	}
	operator unsigned() const
	{
	  return value;
	}

	unsigned card;

      private:
	unsigned h, value;
      };


      struct bbox2d_t
      {
	typedef bbox2d_t self_t;
	point2d min, max;

	template <class I>
	void init(const I&, const point2d& p)
	{
	  min = p;
	  max = p;
	}
	template <class I>
	void insert(const I&, const point2d& p)
	{
	  // row
	  if (p.row() > max.row())
	    max.row() = p.row();
	  else if (p.row() < min.row())
	    min.row() = p.row();
	  // col
	  if (p.col() > max.col())
	    max.col() = p.col();
	  else if (p.col() < min.col())
	    min.col() = p.col();
	}
	void embrace(const self_t& rhs)
	{
	  // min
	  if (rhs.min.row() < min.row())
	    min.row() = rhs.min.row();
	  if (rhs.min.col() < min.col())
	    min.col() = rhs.min.col();
	  // max
	  if (rhs.max.row() > max.row())
	    max.row() = rhs.max.row();
	  if (rhs.max.col() > max.col())
	    max.col() = rhs.max.col();
	}
	unsigned width() const
	{
	  return max.col() - min.col();
	}
	unsigned height() const
	{
	  return max.row() - min.row();
	}
      };



      struct hdome_t // delta level to the highest point
      {
	typedef hdome_t self_t;

	template <class I, class P>
	void init(const I& f, const P& p)
	{
	  h = f[p];
	  value = 0;
	}
	template <class I, class P>
	void insert(const I& f, const P& p)
	{
	  assert(f[p] == h);
	}
	void embrace(const self_t& rhs)
	{
	  assert(rhs.h > h);
	  if (rhs.value + (rhs.h - h) > value)
	    value = rhs.value + (rhs.h - h);
	}
	operator unsigned() const
	{
	  return value;
	}

	unsigned h;
      private:
	unsigned value;
      };


//       struct montparnasse_t // FIXME: explain...
//       {
// 	typedef montparnasse_t self_t;

// 	template <class I, class P>
// 	void init(const I& f, const P& p)
// 	{
// 	  hdome.init(f, p);
// 	  mass.init(f, p);
// 	}
// 	template <class I, class P>
// 	void insert(const I& f, const P& p)
// 	{
// 	  assert(f[p] == hdome.h);
// 	  hdome.insert(f, p);
// 	  mass.insert(f, p);
// 	}
// 	void embrace(const self_t& rhs)
// 	{
// 	  assert(rhs.h > hdome.h);
// 	  hdome.embrace(f, p);
// 	  mass.embrace(f, p);
// 	}
// 	operator float() const
// 	{
// 	  static const float epsilon = 1e-7; // FIXME
// 	  float tmp = float(mass) / (mass.card * (hdome + 1));
// 	  assert(tmp > - epsilon and tmp < 1.f + epsilon);
// 	}

//       private:
// 	hdome_t hdome;
// 	mass_t mass;
//       };


    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_ATTRIBUTES_HH
