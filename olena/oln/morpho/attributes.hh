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

#ifndef ATTRIBUTES_HH
# define ATTRIBUTES_HH
# include <mlc/type.hh>
# include <vector>

// some usefull macros

// those macros should be moved into mlc
// # define mlc_exact_vt_type(T, Exact)	typename mlc::exact_vt<T, Exact>::ret
// # define oln_2_exact_vt_type(self, T, Exact)	typename mlc::exact_vt<self<T, Exact>, Exact>::ret
// # define dispatch(Fun)			return exact().Fun##_impl

// attribute dedicated macros
# define attr_lambda_type(T)	typename attr_traits<T>::lambda_type
# define attr_env_type(T)	typename attr_traits<T>::env_type
# define attr_value_type(T)	typename attr_traits<T>::value_type

# define attr_type_decl(slef_type) \
      typedef mlc_exact_vt_type(self_type, Exact)	exact_type; \
      typedef attr_value_type(exact_type)		value_type; \
      typedef attr_env_type(exact_type)			env_type; \
      typedef attr_lambda_type(exact_type)		lambda_type

namespace oln {
  namespace morpho {
    namespace tarjan {
      namespace tools {
	// should be moved elsewhere
	template <class T>
	T diffabs(const T &v1, const T &v2)
	{
	  return v1 > v2 ? v1 - v2 : v2 - v1;
	}

      }
      // environment herarchy
      // not used yet
      template <class Exact>
      struct env: public mlc_hierarchy::any<Exact>
      {
      };

      // this env is an empty one
      struct NullEnv: public env<NullEnv>
      {
      };


      // the traits fwd declaration
      template <class T>
      struct attr_traits;


      // the attributes hierarchy

      /*------------*
	| attribute |
	*-----------*/
      template <class Exact>
      class attribute: public mlc_hierarchy::any<Exact>
      {
      public:
 	typedef attribute<Exact>	self_type;
	attr_type_decl(self_type);

	void operator+=(const exact_type &rhs)
	{
	  mlc_dispatch(pe)(rhs);
	};

	bool operator>=(const lambda_type &lambda) const
	{
	  mlc_dispatch(ge)(lambda);
	};

	bool operator<(const lambda_type &lambda) const
	{
	  mlc_dispatch(less)(lambda);
	};

	bool operator!=(const lambda_type &lambda) const
	{
	  mlc_dispatch(ne)(lambda);
	};

	// impl
	bool ge_impl(const lambda_type &lambda) const
	{
	  return !(*this < lambda);
	};

      protected:
	attribute() {};

      };

      /*-----------*
	|   area    |
	*-----------*/

      template <class T = unsigned, class Exact = mlc::final>
      class area_type:
	public attribute<mlc_2_exact_vt_type(area_type, T, Exact)>
      {
      public:
	typedef area_type<T, Exact>			self_type;
	attr_type_decl(self_type);

	area_type()
	  {
	  };

	area_type(const lambda_type &lambda): value_(lambda)
	  {
	  };

	template <class I>
	  area_type(const abstract::image<I>&,
		    const oln_point_type(I)&,
		    const env_type&):
	  value_(1)
	  {
	  };

	void pe_impl(const area_type &rhs)
	  {
	    value_ += rhs.value_;
	  };

	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type value_;
	//    const client_data_type &client_data;
	// FIXME: client_data is usefull when attribute is computed from other data
      };


      /*-----------*
	|  volume   |
	*-----------*/

      template <class T = unsigned, class Exact = mlc::final>
      class volume_type:
	public attribute<mlc_2_exact_vt_type(volume_type, T, Exact)>
      {
      public:
	typedef volume_type<T, Exact>			self_type;
	attr_type_decl(self_type);

	// Ctors
	volume_type()
	  {
	  };


	volume_type(const lambda_type &lambda): value_(lambda)
	  {
	  };

	template <class I>
	  volume_type(const abstract::image<I> &input,
		      const oln_point_type(I) &p,
		      const env_type &) :
	  reflevel_(input[p]),
	  area_(1),
	  value_(1)
	  {
	  };
	// interface part
	const value_type &getValue() const
	  {
	    mlc_dispatch(getValue)();
	  };

	const value_type &getReflevel() const
	  {
	    mlc_dispatch(getReflevel)();
	  };

	const value_type &getArea() const
	  {
	    mlc_dispatch(getArea)();
	  };

	// impl part
	const value_type &getValue_impl() const
	  {
	    return value_;
	  };

	const value_type &getReflevel_impl() const
	  {
	    return reflevel_;
	  };

	const value_type &getArea_impl() const
	  {
	    return area_;
	  };

	void pe_impl(const volume_type &rhs)
	  {
	    value_ += rhs.getValue() + area_ * tools::diffabs(reflevel_, rhs.getReflevel());
	    area_ += rhs.getArea();
	  };

	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type reflevel_;
	value_type area_;
	value_type value_;
      };

      /*-----------*
	|  height   |
	*-----------*/

      template <class T = unsigned, class Exact = mlc::final>
      class height_type:
	public attribute<mlc_2_exact_vt_type(height_type, T, Exact)>
      {
      public:
	typedef height_type<T, Exact>			self_type;
	attr_type_decl(self_type);

	height_type()
	  {
	  };

	height_type(const lambda_type &lambda): value_(lambda)
	  {
	  };

	template <class I>
	  height_type(const abstract::image<I> &input,
		      const oln_point_type(I) &p,
		      const env_type&):
	  reflevel_(input[p]),
	  value_(ntg_zero_val(value_type))
	  {
	  };

	const value_type &getReflevel() const
	  {
	    mlc_dispatch(getReflevel)();
	  };

	// impl part
	const value_type &getReflevel_impl() const
	  {
	    return reflevel_;
	  };

	void pe_impl(const height_type &rhs)
	  {
	    value_ = tools::diffabs(reflevel_, rhs.getReflevel());
	  };

	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type reflevel_;
	value_type value_;
    };


    /*-----------*
      | maxvalue  |
      *-----------*/

    template <class T = unsigned, class Exact = mlc::final>
    class maxvalue_type:
      public attribute<mlc_2_exact_vt_type(maxvalue_type, T, Exact)>
    {
    public:
      typedef maxvalue_type<T, Exact>			self_type;
      attr_type_decl(self_type);

      maxvalue_type()
	{
	};

      maxvalue_type(const lambda_type &lambda): value_(lambda)
	{
	};

      template <class I>
	maxvalue_type(const abstract::image<I> &input,
		      const oln_point_type(I) &p,
		      const env_type &):
	value_(input[p])
	{
	};

      const value_type &getValue() const
	{
	  mlc_dispatch(getValue)();
	};

      const value_type &getValue_impl() const
	{
	  return value_;
	};

      void pe_impl(const maxvalue_type &rhs)
	{
	  value_ = ntg::max(value_, rhs.getValue());
	};

      bool less_impl(const lambda_type &lambda) const
	{
	  return value_ < lambda;
	};

      bool ne_impl(const lambda_type &lambda) const
	{
	  return lambda != value_;
	};

    protected:
      value_type value_;
    };


    /*-----------*
      | minvalue  |
      *-----------*/

    template <class T = unsigned, class Exact = mlc::final>
    class minvalue_type:
      public attribute<mlc_2_exact_vt_type(minvalue_type, T, Exact)>
    {
    public:
      typedef minvalue_type<T, Exact>			self_type;
      attr_type_decl(self_type);

      minvalue_type()
	{
	};

      minvalue_type(const lambda_type &lambda): value_(lambda)
	{
	};

      template <class I>
	minvalue_type(const abstract::image<I> &input,
		      const oln_point_type(I) &p,
		      const env_type &) :
	value_(input[p])
	{
	};

      const value_type &getValue() const
	{
	  mlc_dispatch(getValue)();
	};

      const value_type &getValue_impl() const
	{
	  return value_;
	};

      void pe_impl(const minvalue_type &rhs)
	{
	  value_ = ntg::min(value_, rhs.getValue());
	};

      bool less_impl(const lambda_type &lambda) const
	{
	  return value_ < lambda;
	};

      bool ne_impl(const lambda_type &lambda) const
	{
	  return lambda != value_;
	};

    protected:
      value_type value_;
    };


      /*-----------*
	|   disk    |
	*-----------*/

      template <class I, class Exact = mlc::final>
      class disk_type:
	public attribute<mlc_2_exact_vt_type(disk_type, I, Exact)>
      {
      public:
	typedef disk_type<I, Exact>			self_type;
	attr_type_decl(self_type);
	typedef abstract::image<mlc_exact_type(I)>	im_type;
	typedef oln_point_type(im_type)			point_type;
	typedef oln_dpoint_type(im_type)		dpoint_type;
	typedef std::vector<point_type>			pts_type;
	typedef typename pts_type::const_iterator	cst_iter_type;

	disk_type()
	{
	};

	disk_type(const lambda_type &lambda): value_(lambda), pts_()
	  {
	  };

	disk_type(const im_type&, const point_type &p, const env_type &) :
	  value_(1), pts_()
	{
	  pts_.push_back(p);
	};

	const pts_type &getPts() const
	  {
	    mlc_dispatch(getPts)();
	  };

	// impl

	const pts_type &getPts_impl() const
	  {
	    return pts_;
	  };

	void pe_impl(const disk_type &rhs)
	{
	  std::copy(rhs.getPts().begin(), rhs.getPts().end(), std::back_inserter(pts_));
	  value_ = ntg_zero_val(value_type);
	  for (cst_iter_type p1 = pts_.begin(); p1 != pts_.end(); ++p1)
	    for (cst_iter_type p2 = pts_.begin(); p2 != pts_.end(); ++p2)
	      {
		unsigned d = 0;
		dpoint_type	p = *p1 - *p2;
		for (int i = 0; i < point_traits<point_type>::dim; ++i)
		  d += p.nth(i) * p.nth(i);
		if (d > value_)
		  value_ = d;
	      }
	  value_ /= 2;
	};

	bool less_impl(const lambda_type &lambda) const
	{
	  return value_ < lambda;
	};

	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type value_;
	pts_type pts_;
      };



      /*-----------*
	|   dist    |
	*-----------*/

      template <class I, class Exact = mlc::final>
      class dist_type:
	public attribute<mlc_2_exact_vt_type(dist_type, I, Exact)>
      {
      public:
	typedef dist_type<I, Exact>			self_type;
	attr_type_decl(self_type);
	typedef abstract::image<mlc_exact_type(I)>	im_type;
	typedef oln_point_type(im_type)			point_type;
	typedef oln_dpoint_type(im_type)		dpoint_type;

	dist_type()
	{
	};

	dist_type(const im_type&,
		  const point_type &p,
		  const env_type &) :
	  value_(ntg_zero_val(value_type)),
	  center_(p)
	{
	};

	dist_type(const lambda_type lambda): value_(lambda)
	  {
	  };

	const point_type &getCenter() const
	  {
	    mlc_dispatch(getCenter)();
	  };

	// impl
	const point_type &getCenter_impl() const
	  {
	    return center_;
	  };

	void pe_impl(const dist_type &rhs)
	{
	  dpoint_type	p = center_ - rhs.getCenter();

	  value_ = ntg_zero_val(value_type);
	  for (int i = 0; i < point_traits<point_type>::dim; ++i)
	    value_ += p.nth(i) * p.nth(i);
	  value_ = sqrt(value_);
	};

	bool less_impl(const lambda_type &lambda) const
	{
	  return value_ < lambda;
	};

	bool ne_impl(const lambda_type &lambda) const
	  {
	    return value_ != lambda;
	  };

      protected:
	value_type value_;
	point_type center_;

      };



      /*-----------*
	|  square   |
	*-----------*/

      template <class I, class Exact = mlc::final>
      class square_type:
	public attribute<mlc_2_exact_vt_type(square_type, I, Exact)>
      {
      public:
	typedef square_type<I, Exact>			self_type;
	attr_type_decl(self_type);
	typedef abstract::image<mlc_exact_type(I)>	im_type;
	typedef oln_point_type(im_type)			point_type;
	typedef oln_dpoint_type(im_type)		dpoint_type;

	enum {dim = point_traits<point_type>::dim};

	square_type()
	  {
	  }

	square_type(const lambda_type &lambda):
	  mins_(dim),
	  maxs_(dim),
	  value_(lambda)
	  {
	    for (int i = 0; i < point_traits<point_type>::dim; ++i)
	      {
		maxs_[i] = lambda;
		mins_[i] = ntg_zero_val(coord);
	      }
	  };

	square_type(const im_type&,
		    const point_type &p,
		    const env_type &):
	  mins_(dim), maxs_(dim), value_(ntg_zero_val(value_type))
	{
	  for (int i = 0; i < dim; ++i)
	    mins_[i] = maxs_[i] = p.nth(i);
	}

	int getMin(int i) const
	  {
	    mlc_dispatch(getMin)(i);
	  };

	int getMax(int i) const
	  {
	    mlc_dispatch(getMax)(i);
	  };

	// impl
	int getMin_impl(int i) const
	  {
	    precondition(i < dim);
	    return mins_[i];
	  };

	int getMax_impl(int i) const
	  {
	    precondition(i < dim);
	    return maxs_[i];
	  };

	void pe_impl(const square_type &rhs)
	{
	  for (int i = 0; i < dim; ++i)
	    {
	      mins_[i] = ntg::min(mins_[i], rhs.getMin(i));
	      maxs_[i] = ntg::max(maxs_[i], rhs.getMax(i));
	    }
	  value_ = maxs_[0] - mins_[0];
	  for (int i = 1; i < dim; ++i)
	    if (value_ < value_type(maxs_[i] - mins_[i]))
	      value_ = maxs_[i] - mins_[i];
	}

	bool less_impl(const lambda_type &lambda) const
	{
	  return value_ < lambda;
	}

	bool ne_impl(const lambda_type &lambda) const
	  {
	    return value_ != lambda;
	  };

      protected:
	std::vector<coord>	mins_;
	std::vector<coord>	maxs_;
	value_type		value_;
      };



      /*-----------*
	| rectangle |
	*-----------*/
      template <class I, class Exact = mlc::final>
      class rectangle_type:
	public attribute<mlc_2_exact_vt_type(rectangle_type, I, Exact)>
      {
      public:
	typedef rectangle_type<I, Exact>		self_type;
	attr_type_decl(self_type);
	typedef abstract::image<mlc_exact_type(I)>	im_type;
	typedef oln_point_type(im_type)			point_type;
	typedef oln_dpoint_type(im_type)		dpoint_type;
	enum {dim = point_traits<point_type>::dim };

	rectangle_type(const lambda_type &lambda): maxs_(dim), mins_(dim)
	  {
	    for (int i = 0; i < dim; ++i)
	      {
		mins_[i] = ntg_zero_val(value_type);
		maxs_[i] = lambda[i];
	      }
	  };

	rectangle_type()
	{
	}

	rectangle_type(const im_type&, const point_type &p, const env_type &): maxs_(dim), mins_(dim)
	{
	  for (int i = 0; i < dim; ++i)
	    mins_[i] = maxs_[i] = p.nth(i);
	}

	value_type getMin(int i) const
	  {
	    mlc_dispatch(getMin)(i);
	  };

	value_type getMax(int i) const
	  {
	    mlc_dispatch(getMax)(i);
	  };

	// impl
	value_type getMin_impl(int i) const
	  {
	    precondition(i < point_traits<point_type>::dim);
	    return mins_[i];
	  };

	value_type getMax_impl(int i) const
	  {
	    precondition(i < point_traits<point_type>::dim);
	    return maxs_[i];
	  };

	void pe_impl(const rectangle_type &rhs)
	{
	  for (int i = 0; i < dim; ++i)
	    {
	      mins_[i] = ntg::min(mins_[i], rhs.getMin(i));
	      maxs_[i] = ntg::max(maxs_[i], rhs.getMax(i));
	    }
	}

	bool less_impl(const lambda_type &lambda) const
	{
	  for (int i = 0; i < dim; ++i)
	    if ((maxs_[i] - mins_[i]) >= lambda[i])
	      return false;
	  return true;
	}

	bool ne_impl(const lambda_type &lambda) const
	  {
	    for (int i = 0; i < dim; ++i)
	      if ((maxs_[i] - mins_[i]) == lambda[i])
		return false;
	    return true;
	  };

      protected:
	std::vector<value_type>	maxs_;
	std::vector<value_type>	mins_;
      };

      /*-------------------------
	| traits specialisations |
	\------------------------*/


      // volume traits
      template <class T, class Exact>
      struct attr_traits<volume_type<T, Exact> >
      {
	typedef T		value_type;
	typedef value_type	lambda_type;
	typedef NullEnv		env_type;
      };

      // volume traits
      template <class T, class Exact>
      struct attr_traits<height_type<T, Exact> >
      {
	typedef T		value_type;
	typedef value_type	lambda_type;
	typedef NullEnv		env_type;
      };

      // area traits
      template <class T, class Exact>
      struct attr_traits<area_type<T, Exact> >
      {
	typedef T		value_type;
	typedef value_type	lambda_type;
	typedef NullEnv		env_type;
      };

      // maxvalue traits
      template <class T, class Exact>
      struct attr_traits<maxvalue_type<T, Exact> >
      {
	typedef T		value_type;
	typedef value_type	lambda_type;
	typedef NullEnv		env_type;
      };

      // minvalue traits
      template <class T, class Exact>
      struct attr_traits<minvalue_type<T, Exact> >
      {
	typedef T		value_type;
	typedef value_type	lambda_type;
	typedef NullEnv		env_type;
      };

      // disk traits
      template <class I, class Exact>
      struct attr_traits<disk_type<I, Exact> >
      {
	typedef unsigned	value_type;
	typedef value_type	lambda_type;
	typedef NullEnv		env_type;
      };

      // dist traits
      template <class I, class Exact>
      struct attr_traits<dist_type<I, Exact> >
      {
	typedef float		value_type;
	typedef value_type	lambda_type;
	typedef NullEnv		env_type;
      };

      // square traits
      template <class I, class Exact>
      struct attr_traits<square_type<I, Exact> >
      {
	typedef unsigned	value_type;
	typedef value_type	lambda_type;
	typedef NullEnv		env_type;
      };

      // rectangle traits
      template <class I, class Exact>
      struct attr_traits<rectangle_type<I, Exact> >
      {
	typedef unsigned					value_type;
	typedef ntg::vec<I::dim, value_type, mlc::final>	lambda_type;
	typedef NullEnv						env_type;
      };
    }
  }
}

/*-----------*
  |  diamond  |
  *-----------*/

// FIXME: to be written...


#endif // ndef ATTRIBUTES_HH

