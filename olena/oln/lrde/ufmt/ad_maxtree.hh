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

#ifndef OLENA_LRDE_UFMT_AD_MAXTREE_HH
# define OLENA_LRDE_UFMT_AD_MAXTREE_HH

# include <oln/lrde/ufmt/utils.hh>


# define oln_lrde_ufmt_import_ad_maxtree_typedefs	\
	typedef typename super::image image;		\
	typedef typename super::point point;		\
	typedef typename super::dpoint dpoint;		\
	typedef typename super::dparent_t dparent_t;	\
	typedef typename super::dpar_t dpar_t;		\
	typedef typename super::value value


namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {


      template <class T>
      point2d point_at(const image2d<T>& ima, int i)
      {
	int len = ima.ncols() + 2 * ima.border();
	i += (len + 1) * ima.border();
	return point2d((i / len) - ima.border(),
		       (i % len) - ima.border());
      }


      // FIXME: doc.

      template <class I>
      struct ad_maxtree
      {
	typedef I image;
	typedef int point;
	typedef int dpoint;
	typedef oln_value_type(I) value;

	typedef oln_point_type(I) point_type;

	template <class II>
	struct indexed_image
	{
	  indexed_image()
	    : ima_(0),
	      buffer(0)
	  {
	  }

	  void init(const II& ima)
	  {
	    ima_ = const_cast<II*>(&ima);
	    buffer = &((*ima_)[point_type()]);
	  }

	  void memset_0()
	  {
	    T* ptr = ima_->impl()->buffer();
	    memset(ptr, 0, ima_->impl()->len() * sizeof(int));
	  }

	  typedef oln_value_type(II) T;

	  T& operator[](int i)
	  {
	    assert(buffer != 0);
	    return *(buffer + i);
	  }

	  T operator[](int i) const
	  {
	    assert(buffer != 0);
	    return *(buffer + i);
	  }

	  int index_of(const point_type& p) const
	  {
	    assert(buffer != 0);
	    return int(&((*ima_)[p]) - buffer);
	  }

	  bool hold(const oln_point_type(I)& p) const
	  {
	    return this->ima_->hold(p);
	  }

	  bool hold(int i) const
	  {
	    point_type p = point_at(*ima_, i);
	    if (index_of(p) != i)
	      std::cout << p << ' ' << i << std::endl;
	    assert(index_of(p) == i);
	    return this->ima_->hold(p);
	  }

	  size_t npoints() const
	  {
	    return this->ima_.npoints();
	  }

	  const II& real() const { return *ima_; }

	  II* ima_;
	  T* buffer;
	};



	// aux data
	typedef typename mute<I, int>::ret dparent_t;
	typedef indexed_image<dparent_t> dpar_t;

	dparent_t dpar_; // proxied
	dpar_t dpar;     // proxy
	// input
	indexed_image<I> f;


	// ctor

	ad_maxtree(const abstract::image<I>& f_)
	  :
	  dpar_(f_.size())
	{
	  f.init(f_.exact());
	  dpar.init(dpar_);
	  dpar.memset_0();
	}

	void make_set(const point& x)
	{
	  dpar[x] = 0;
	}

	bool is_root(const point& x) const
	{
	  return dpar[x] == 0;
	}

	bool is_level_root(const point& x) const
	{
	  return is_root(x) or f[x + dpar[x]] < f[x];
	}

	point find_level_root(const point& x)
	{
	  if (is_level_root(x))
	    return x;
	  else
	    {
	      point lr = find_level_root(x + dpar[x]);
	      dpar[x] = lr - x;
	      return lr;
	    }
	}

	point anc(point x, value h)
	{
	  while (not is_root(x) and h <= f[x + dpar[x]])
	    x = find_level_root(x + dpar[x]);
	  return x;
	}

	void swap(point& x, point& y)
	{
	  point memo = x;
	  x = y;
	  y = memo;
	}


	// uniformized interface

	const point_type& parent_(const point_type& p) const {
	  int i = f.index_of(p);
	  return point_at(i + dpar[i]);
	}
	const value& f_(const point_type& p) const {
	  return f.real()[p];
	}
	const I& f_() const {
	  return f.real();
	}
	bool is_root_(const point_type& p) const {
	  int i = f.index_of(p);
	  assert(i == dpar.index_of(p));
	  return is_root(i);
	}
	bool is_level_root_(const point_type& p) const {
	  int i = f.index_of(p);
	  assert(i == dpar.index_of(p));
	  return is_level_root(i);
	}

	// end of uniformized interface



      }; // end of class ad_maxtree



    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_AD_MAXTREE_HH
