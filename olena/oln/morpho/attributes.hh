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

#ifndef OLN_MORPHO_ATTRIBUTES_HH
# define OLN_MORPHO_ATTRIBUTES_HH
# include <oln/basics.hh>
# include <mlc/type.hh>
# include <vector>
# include <oln/morpho/environments.hh>

// attribute dedicated macros
# define attr_lambda_type(T)	typename oln::morpho::attr::attr_traits<T>::lambda_type
# define attr_env_type(T)	typename oln::morpho::attr::attr_traits<T>::env_type
# define attr_value_type(T)	typename oln::morpho::attr::attr_traits<T>::value_type

# define attr_lambda_type_(T)	 oln::morpho::attr::attr_traits<T>::lambda_type
# define attr_env_type_(T)	 oln::morpho::attr::attr_traits<T>::env_type
# define attr_value_type_(T)	 oln::morpho::attr::attr_traits<T>::value_type


# define attr_type_decl(self_type) \
      typedef mlc_exact_vt_type(self_type, Exact)	exact_type; \
      typedef attr_value_type(exact_type)		value_type; \
      typedef attr_env_type(exact_type)			env_type; \
      typedef attr_lambda_type(exact_type)		lambda_type

namespace oln {
  namespace morpho {
    /*! \brief Implementation of attributes.
     */
    namespace attr {
      /*! \brief Useful tools for morphological math.
       */
      namespace tools {

	/*!
	** \brief Absolute value of difference between v1 and v2.
	** \warning Should be moved elsewhere.
	*/
	template <class T>
	T diffabs(const T &v1, const T &v2)
	{
	  return v1 > v2 ? v1 - v2 : v2 - v1;
	}
      } // !tools


      // the traits fwd declaration
      /*!
      ** Traits  for attributes information.
      ** \param T Exact type of the attribute.
      */
      template <class T>
      struct attr_traits;

      /*!
      ** \brief Change the exact type of an attribute.
      **
      ** BE CAREFUL,  when you use specialization of  this traits, you
      ** say change_exact<In, Exact>::ret IS A In.
      ** Check this to avoid modeling problems.
      */
      template <class In, class Exact>
      struct change_exact;


      // the attributes hierarchy

      /*------------*
	| attribute |
	*-----------*/
      /*!
      ** \brief Attribute abstract class
      **
      ** Top of the attribute hierarchy.
      */
      template <class Exact>
      class attribute: public mlc_hierarchy::any<Exact>
      {
      public:
	typedef attribute<Exact>	self_type; /*< Self type of the class.*/
	attr_type_decl(self_type);

	/*!
	** \brief += operator
	**
	** This is a static dispatcher for the += operator.
	** This method is abstract.
	*/
	void operator+=(const exact_type &rhs)
	{
	  mlc_dispatch(pe)(rhs);
	};

	/*!
	** \brief >= operator
	**
	** This is a static dispatcher for the >= operator.
	*/
	bool operator>=(const lambda_type &lambda) const
	{
	  mlc_dispatch(ge)(lambda);
	};

	/*!
	** \brief "<" operator
	**
	** This is a static dispatcher for the "<" operator.
	** This method is abstract.
	*/
	bool operator<(const lambda_type &lambda) const
	{
	  mlc_dispatch(less)(lambda);
	};

	/*!
	** \brief != operator
	**
	** This is a static dispatcher for the != operator.
	** This method is abstract.
	*/
	bool operator!=(const lambda_type &lambda) const
	{
	  mlc_dispatch(ne)(lambda);
	};

	/*!
	** \brief >= operator implementation.
	**
	** This is an implementation  of the >= operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T be called.
	*/
	bool ge_impl(const lambda_type &lambda) const
	{
	  return !(*this < lambda);
	};

      protected:
	attribute() {};

      };

      /*-----------*
	|   card    |
	*-----------*/

      /*!
      ** \brief Cardinal attribute.
      **
      ** It is equivalent to an area in 2d, and a volume in 3D.
      */
      template <class T = unsigned, class Exact = mlc::final>
      class card_type:
	public attribute<mlc_2_exact_vt_type(card_type, T, Exact)>
      {
      public:
	typedef card_type<T, Exact>			self_type; /*!< Self type of the class. */
	attr_type_decl(self_type);

	/*!
	** \brief Basic Ctor.
	**
	** \warning  After this  call, the  object is  only instantiated
	** (not initialized).
	*/
	card_type()
	  {
	  };

	/*!
	** \brief Ctor from a lambda_type value.
	*/
	card_type(const lambda_type &lambda): value_(lambda)
	  {
	  };

	/*!
	** \brief Ctor from a point and an image.
	**
	** Every parameters are useless.
	*/
	template <class I>
	  card_type(const abstract::image<I>&,
		    const oln_point_type(I)&,
		    const env_type&):
	  value_(ntg_unit_val(value_type))
	  {
	  };


	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T be called.
	*/
	void pe_impl(const self_type &rhs)
	  {
	    value_ += rhs.value_;
	  };

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the "<" operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the != operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type value_; /*!< Value used inside the class. */
      };


      /*-----------*
	|  integral   |
	*-----------*/

      /*!
      ** \brief Integral attribute.
      **
      ** It is equivalent to volume in 2D, and weight in 3D.
      */
      template <class T = unsigned, class Exact = mlc::final>
      class integral_type:
	public attribute<mlc_2_exact_vt_type(integral_type, T, Exact)>
      {
      public:
	typedef integral_type<T, Exact>			self_type; /*!< Self type of the class. */
	attr_type_decl(self_type);

	/*!
	** \brief Basic Ctor.
	**
	** \warning  After this  call, the  object is  only instantiated
	** (not initialized).
	*/
	integral_type()
	  {
	  };

	/*!
	** \brief Ctor from a lambda_type value.
	*/
	integral_type(const lambda_type &lambda): value_(lambda)
	  {
	  };

	/*!
	** \brief Ctor from a point and an image.
	*/
	template <class I>
	  integral_type(const abstract::image<I> &input,
			const oln_point_type(I) &p,
			const env_type &) :
	  reflevel_(input[p]),
	  area_(ntg_unit_val(value_type)),
	  value_(ntg_unit_val(value_type))
	  {
	  };

	// interface part
	/*!
	** \brief Accessor to value_.
	**
	** Virtual method.
	** \see getValue_impl()
	*/
	const value_type &getValue() const
	  {
	    mlc_dispatch(getValue)();
	  };

	/*!
	** \brief Accessor to the reference level.
	**
	** \see getReflevel_impl()
	*/
	const value_type &getReflevel() const
	  {
	    mlc_dispatch(getReflevel)();
	  };

	/*!
	** \brief Accessor to the current area.
	**
	** \see getArea_impl()
	*/
	const value_type &getArea() const
	  {
	    mlc_dispatch(getArea)();
	  };

	// impl part
	/*!
	** \brief Implementation of getValue().
	**
	** Override this  method in  order to provide  a new  version of
	** getValue().
	**
	** \warning Do not call this method, use getValue() instead.
	*/
	const value_type &getValue_impl() const
	  {
	    return value_;
	  };

	/*!
	** \brief Implementation of getReflevel().
	**
	** Override this  method in  order to provide  a new  version of
	** getReflevel().
	**
	** \warning Do not call this method, use getReflevel() instead.
	*/
	const value_type &getReflevel_impl() const
	  {
	    return reflevel_;
	  };

	/*!
	** \brief Implementation of getArea().
	**
	** Override this  method in  order to provide  a new  version of
	** getArea().
	**
	** \warning Do not call this method, use getArea() instead.
	*/
	const value_type &getArea_impl() const
	  {
	    return area_;
	  };

	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	void pe_impl(const self_type &rhs)
	  {
	    value_ += rhs.getValue() + area_ * tools::diffabs(reflevel_, rhs.getReflevel());
	    area_ += rhs.getArea();
	  };

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type reflevel_; ///< Reference level.
	value_type area_; ///< Current area.
	value_type value_; ///< Current value (deduced from area and level).
      };

      /*-------------------*
	| other image_type |
	------------------*/
      /*!
      ** \class other_image
      **
      ** \brief Metaclass used to change attribute behavior.
      **
      ** This class do the same job  that its Dad parameter, but force it
      ** to work on other data.
      */
      template <class Dad, class I, class Exact = mlc::final>
      class other_image:
	public change_exact<Dad, typename mlc::exact_vt<other_image<Dad, I, Exact>, Exact>::ret >::ret
      {
      public:
	typedef other_image<Dad, I,  Exact>						self_type; /*< Self type of the class.*/
	typedef typename abstract::image<mlc_exact_type(I)>				im_type; /*< Type of substituted image.*/
	attr_type_decl(self_type);
	typedef typename change_exact<Dad,
				      typename mlc::exact_vt<other_image<Dad, I, Exact>,
							     Exact>::ret >::ret		super_type; /*< Mother class type.*/

	/*!
	** \brief Constructor.
	**
	** Dispatch to Dad constructor.
	*/
	other_image(): super_type()
	{
	};

	/*!
	** \brief lambda_type Constructor.
	**
	** Dispatch to Dad constructor.
	*/
	other_image(const lambda_type &lambda): super_type(lambda)
	{
	};

	/*!
	** \brief Image Constructor.
	**
	** Dispatch to Dad constructor but substitute image argument with
	** the image contained in the environment.
	*/
	template <typename IM>
	other_image(const abstract::image<IM> &,
		    const oln_point_type(I) &p,
		    const env_type & e): super_type(e.getImage(), p, e)
	{
	};
      };

      /*----------------------*
	| ball_parent_change |
	*--------------------*/
      /*!
      ** \class other_point
      **
      ** \brief Metaclass used to change attribute behavior.
      **
      ** This class do the same job  that its Dad parameter, but force it
      ** to work on other data.
      */
      template <class I, class Exact = mlc::final>
      class ball_parent_change:
	public attribute<mlc_2_exact_vt_type(ball_parent_change, I, Exact)>
      {
      public:
	typedef ball_parent_change<I,  Exact>			self_type; /*< Self type of the class.*/
	typedef typename abstract::image<mlc_exact_type(I)>	im_type; /*< Type of substituted image.*/
	attr_type_decl(self_type);
	typedef oln_value_type(I)				pts_type; ///< should be list <?>
	typedef typename pts_type::const_iterator		cst_iter_type; ///< const iterator on Point vector.
	typedef typename pts_type::value_type			point_type; ///< Point type associated to im_type.
	typedef oln_dpoint_type(point_type)	dpoint_type; ///< Dpoint type associated to im_type.

	/*!
	** \brief Constructor.
	**
	** Dispatch to Dad constructor.
	*/
	ball_parent_change(): value_(ntg_zero_val(value_type)), points_()
	{
	};

	/*!
	** \brief lambda_type Constructor.
	**
	** Dispatch to Dad constructor.
	*/
	ball_parent_change(const lambda_type &lambda): value_(lambda), points_()
	{
	};

	/*!
	** \brief Image Constructor.
	**
	** Dispatch to Dad constructor but substitute image argument with
	** the image contained in the environment.
	*/
	template <typename IM>
	ball_parent_change(const abstract::image<IM> &,
			   const oln_point_type(IM) &p,
			   const env_type &e): points_()//: super_type(/*e.getImage(), */e.getPoint(p), e)
	{
	  std::copy(e.getParent()[p].begin(),
		    e.getParent()[p].end(),
		    std::back_inserter(points_));
	};

	/*!
	** \brief Accessor to value_.
	**
	** Virtual method.
	** \see getValue_impl()
	*/
	const value_type &getValue() const
	  {
	    mlc_dispatch(getValue)();
	  };


	/*!
	** \brief Accessor to pts_.
	**
	** Virtual method.
	** \see getPts_impl()
	*/
	const pts_type &getPts() const
	  {
	    mlc_dispatch(getPts)();
	  };

	// impl
	/*!
	** \brief Implementation of getValue().
	**
	** Override this  method in  order to provide  a new  version of
	** getValue().
	**
	** \warning Do not call this method, use getValue() instead.
	*/
	const value_type &getValue_impl() const
	  {
	    return value_;
	  };

	/*!
	** \brief Implementation of getValue().
	**
	** Override this  method in  order to provide  a new  version of
	** getPts().
	**
	** \warning Do not call this method, use getPts() instead.
	*/
	const pts_type &getPts_impl() const
	  {
	    return points_;
	  };

	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	void pe_impl(const self_type &rhs)
	  {
	    precondition(points_.size() < (128 * 128 * 128 + 1));
	    std::copy(rhs.getPts().begin(),
		      rhs.getPts().end(),
		      std::back_inserter(points_));
	    compute_value();
	    value_ = ntg::max(value_, rhs.getValue());
	  };

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	void compute_value()
	  {
	    value_type	last = value_;
	    value_ = ntg_zero_val(value_type);
	    for (cst_iter_type p1 = points_.begin(); p1 != points_.end(); ++p1)
	      for (cst_iter_type p2 = points_.begin(); p2 != points_.end(); ++p2)
		{
		  unsigned d = 0;
		  dpoint_type	p = *p1 - *p2;
		  for (int i = 0; i < point_traits<point_type>::dim; ++i)
		    d += p.nth(i) * p.nth(i);
		  if (d > value_)
		    value_ = d;
		}
	    value_ /= 2;
	    value_ = ntg::max(value_, last);
	  }

	value_type	value_;
	pts_type	points_;
      };

      /*-----------*
	|  height   |
	*-----------*/
      /*!
      ** \brief Attribute working on height between components.
      */
      template <class T = unsigned, class Exact = mlc::final>
      class height_type:
	public attribute<mlc_2_exact_vt_type(height_type, T, Exact)>
      {
      public:
	typedef height_type<T, Exact>			self_type;
	attr_type_decl(self_type);

	/*!
	** \brief Basic Ctor.
	**
	** \warning  After this  call, the  object is  only instantiated
	** (not initialized).
	*/
	height_type()
	  {
	  };

	/*!
	** \brief Ctor from a lambda_type value.
	*/
	height_type(const lambda_type &lambda):
	  value_(lambda),
	  min_(ntg_zero_val(value_type)),
	  max_(lambda)
	  {
	  };

	/*!
	** \brief Ctor from a point and an image.
	*/
	template <class I>
	  height_type(const abstract::image<I> &input,
		      const oln_point_type(I) &p,
		      const env_type&):
	  value_(ntg_zero_val(value_type)),
	  min_(input[p]),
	  max_(input[p])
	  {
	  };

	/*!
	** \brief Accessor to min value.
	**
	** Virtual method.
	** \see getMin_impl()
	*/
	const value_type &getMin() const
	  {
	    mlc_dispatch(getMin)();
	  };

	/*!
	** \brief Accessor to max value.
	**
	** Virtual method.
	** \see getMax_impl()
	*/
	const value_type &getMax() const
	  {
	    mlc_dispatch(getMax)();
	  };

	// impl part
	/*!
	** \brief Implementation of getMin().
	**
	** Override this  method in  order to provide  a new  version of
	** getMin().
	**
	** \warning Do not call this method, use getMin() instead.
	*/
	const value_type &getMin_impl() const
	  {
	    return min_;
	  };

	/*!
	** \brief Implementation of getMax().
	**
	** Override this  method in  order to provide  a new  version of
	** getMax().
	**
	** \warning Do not call this method, use getMax() instead.
	*/
	const value_type &getMax_impl() const
	  {
	    return max_;
	  };

	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	void pe_impl(const height_type &rhs)
	  {
	    min_ = ntg::min(min_, rhs.getMin());
	    max_ = ntg::max(max_, rhs.getMax());
	    value_ = max_ - min_;
	  };

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type	value_; ///< Current value.
	value_type	min_; ///< Current minimum.
	value_type	max_; ///< Current maximum.
      };


      /*-----------*
	| maxvalue  |
	*-----------*/
      /*!
      ** \brief Max value attribute.
      ** \param T Data type.
      ** \param Exact The exact type.
      */
      template <class T = unsigned, class Exact = mlc::final>
      class maxvalue_type:
	public attribute<mlc_2_exact_vt_type(maxvalue_type, T, Exact)>
      {
      public:
	typedef maxvalue_type<T, Exact>			self_type; ///< Self type of the class.
	attr_type_decl(self_type);

	/*!
	** \brief Basic Ctor.
	**
	** \warning  After this  call, the  object is  only instantiated
	** (not initialized).
	*/
	maxvalue_type()
	  {
	  };

	/*!
	** \brief Ctor from a lambda_type value.
	**
	** \arg lambda Value of the attribute.
	*/
	maxvalue_type(const lambda_type &lambda): value_(lambda)
	  {
	  };

	/*!
	** \brief Ctor from a point and an image.
	**
	** \param I Image exact type.
	**
	** \arg input Input image.
	** \arg p Point to consider in the image.
	*/
	template <class I>
	  maxvalue_type(const abstract::image<I> &input,
			const oln_point_type(I) &p,
			const env_type &):
	  value_(input[p])
	  {
	  };

	/*!
	** \brief Accessor to value_.
	**
	** Virtual method.
	** \see getValue_impl()
	*/
	const value_type &getValue() const
	  {
	    mlc_dispatch(getValue)();
	  };

	/*!
	** \brief Implementation of getValue().
	**
	** Override this  method in  order to provide  a new  version of
	** getValue().
	**
	** \warning Do not call this method, use getValue() instead.
	*/
	const value_type &getValue_impl() const
	  {
	    return value_;
	  };

	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	void pe_impl(const maxvalue_type &rhs)
	  {
	    value_ = ntg::max(value_, rhs.getValue());
	  };

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type value_; ///< Value of the attribute.
      };


      /*-----------*
	| minvalue  |
	*-----------*/
      /*!
      ** \brief Min value attribute.
      ** \param T Data type.
      ** \param Exact The exact type.
      */
      template <class T = unsigned, class Exact = mlc::final>
      class minvalue_type:
	public attribute<mlc_2_exact_vt_type(minvalue_type, T, Exact)>
      {
      public:
	typedef minvalue_type<T, Exact>			self_type;
	attr_type_decl(self_type);

	/*!
	** \brief Basic Ctor.
	**
	** \warning  After this  call, the  object is  only instantiated
	** (not initialized).
	*/
	minvalue_type()
	  {
	  };

	/*!
	** \brief Ctor from a lambda_type value.
	**
	** \arg lambda Value of the attribute.
	*/
	minvalue_type(const lambda_type &lambda): value_(lambda)
	  {
	  };

	/*!
	** \brief Ctor from a point and an image.
	**
	** \param I Image exact type.
	**
	** \arg input Input image.
	** \arg p Point to consider in the image.
	*/
	template <class I>
	  minvalue_type(const abstract::image<I> &input,
			const oln_point_type(I) &p,
			const env_type &) :
	  value_(input[p])
	  {
	  };

	/*!
	** \brief Accessor to value_.
	**
	** Virtual method.
	** \see getValue_impl()
	*/
	const value_type &getValue() const
	  {
	    mlc_dispatch(getValue)();
	  };

	/*!
	** \brief Implementation of getValue().
	**
	** Override this  method in  order to provide  a new  version of
	** getValue().
	**
	** \warning Do not call this method, use getValue() instead.
	*/
	const value_type &getValue_impl() const
	  {
	    return value_;
	  };

	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	void pe_impl(const minvalue_type &rhs)
	  {
	    value_ = ntg::min(value_, rhs.getValue());
	  };

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ > lambda;
	  };

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type value_; ///< Value of the attribute.
      };


      /*-----------*
	|   ball    |
	*-----------*/
      /*!
      ** \brief Ball attribute.
      ** \param I Exact type of images to process.
      ** \param Exact The exact type.
      */
      template <class I, class Exact = mlc::final>
      class ball_type:
	public attribute<mlc_2_exact_vt_type(ball_type, I, Exact)>
      {
      public:
	typedef ball_type<I, Exact>			self_type; ///< Self type of the class.
	attr_type_decl(self_type);
	typedef abstract::image<mlc_exact_type(I)>	im_type; ///< Image type.
	typedef oln_point_type(im_type)			point_type; ///< Point type associated to im_type.
	typedef oln_dpoint_type(im_type)			dpoint_type; ///< Dpoint type associated to im_type.
	typedef std::vector<point_type>			pts_type; ///< Point vector type.
	typedef typename pts_type::const_iterator		cst_iter_type; ///< const iterator on Point vector.

	/*!
	** \brief Basic Ctor.
	**
	** \warning  After this  call, the  object is  only instantiated
	** (not initialized).
	*/
	ball_type()
	  {
	  };

	/*!
	** \brief Ctor from a lambda_type value.
	**
	** \arg lambda Value of the attribute.
	*/
	ball_type(const lambda_type &lambda): value_(lambda), pts_()
	  {
	  };


	/*!
	** \brief Ctor from a point and an image.
	**
	** \arg p Point to consider in the image.
	*/
	ball_type(const im_type&, const point_type &p, const env_type &) :
	  value_(ntg_zero_val(value_type)), pts_()

	  {
	    pts_.push_back(p);
	  };

	/*!
	** \brief Accessor to value_.
	**
	** Virtual method.
	** \see getValue_impl()
	*/
	const value_type &getValue() const
	  {
	    mlc_dispatch(getValue)();
	  };


	/*!
	** \brief Accessor to pts_.
	**
	** Virtual method.
	** \see getPts_impl()
	*/
	const pts_type &getPts() const
	  {
	    mlc_dispatch(getPts)();
	  };

	// impl
	/*!
	** \brief Implementation of getValue().
	**
	** Override this  method in  order to provide  a new  version of
	** getValue().
	**
	** \warning Do not call this method, use getValue() instead.
	*/
	const value_type &getValue_impl() const
	  {
	    return value_;
	  };

	/*!
	** \brief Implementation of getValue().
	**
	** Override this  method in  order to provide  a new  version of
	** getPts().
	**
	** \warning Do not call this method, use getPts() instead.
	*/
	const pts_type &getPts_impl() const
	  {
	    return pts_;
	  };

	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	void pe_impl(const ball_type &rhs)
	  {
	    value_type	last = value_;
	    std::copy(rhs.getPts().begin(),
		      rhs.getPts().end(),
		      std::back_inserter(pts_));
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
	    value_ = ntg::max(value_, last);
	    value_ = ntg::max(value_, rhs.getValue());
	  };

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    return lambda != value_;
	  };

      protected:
	value_type	value_; ///< Value of the attribute.
	pts_type		pts_; ///< List of point in the ball.
      };



      /*-----------*
	|   dist    |
	*-----------*/
      /*!
      ** \brief Dist attribute.
      ** \param I Exact type of images to process.
      ** \param Exact The exact type.
      */
      template <class I, class Exact = mlc::final>
      class dist_type:
	public attribute<mlc_2_exact_vt_type(dist_type, I, Exact)>
      {
      public:
	typedef dist_type<I, Exact>			self_type; ///< Self type of the class.
	attr_type_decl(self_type);
	typedef abstract::image<mlc_exact_type(I)>	im_type; ///< Image type.
	typedef oln_point_type(im_type)			point_type; ///< Point type associated to im_type.
	typedef oln_dpoint_type(im_type)			dpoint_type; ///< Dpoint type associated to im_type.

	/*!
	** \brief Basic Ctor.
	**
	** \warning  After this  call, the  object is  only instantiated
	** (not initialized).
	*/
	dist_type()
	  {
	  };

	/*!
	** \brief Ctor from a point and an image.
	**
	** \arg p Point to consider in the image.
	*/
	dist_type(const im_type&,
		  const point_type &p,
		  const env_type &):
	  value_(ntg_zero_val(value_type)),
	  center_(p)
	  {
	  };

	/*!
	** \brief Accessor to value_.
	**
	** Virtual method.
	** \see getValue_impl()
	*/
	const value_type &getValue() const
	  {
	    mlc_dispatch(getValue)();
	  };

	/*!
	** \brief Ctor from a lambda_type value.
	**
	** \arg lambda Value of the attribute.
	*/
	dist_type(const lambda_type lambda): value_(lambda)
	  {
	  };

	/*!
	** \brief Accessor to center_.
	**
	** Virtual method.
	** \see getCenter_impl()
	*/
	const point_type &getCenter() const
	  {
	    mlc_dispatch(getCenter)();
	  };

	// impl
	/*!
	** \brief Implementation of getCenter().
	**
	** Override this  method in  order to provide  a new  version of
	** getCenter().
	**
	** \warning Do not call this method, use getCenter() instead.
	*/
	const point_type &getCenter_impl() const
	  {
	    return center_;
	  };

	/*!
	** \brief Implementation of getValue().
	**
	** Override this  method in  order to provide  a new  version of
	** getValue().
	**
	** \warning Do not call this method, use getValue() instead.
	*/
	const value_type &getValue_impl() const
	  {
	    return value_;
	  };

	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	void pe_impl(const dist_type &rhs)
	  {
	    value_type	last = value_;
	    dpoint_type	p = center_ - rhs.getCenter();

	    value_ = ntg_zero_val(value_type);
	    for (int i = 0; i < point_traits<point_type>::dim; ++i)
	      value_ += p.nth(i) * p.nth(i);
	    value_ = sqrt(value_);
	    value_ = ntg::max(value_, last);
	    value_ = ntg::max(value_, rhs.getValue());
	  };

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  };

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    return value_ != lambda;
	  };

      protected:
	value_type value_; ///< Current value of the attribute.
	point_type center_; ///< Center of the attribute.

      };



      /*-----------*
	|  cube   |
	*-----------*/
      /*!
      ** \brief Cube attribute.
      ** \param I Exact type of images to process.
      ** \param Exact The exact type.
      */
      template <class I, class Exact = mlc::final>
      class cube_type:
	public attribute<mlc_2_exact_vt_type(cube_type, I, Exact)>
      {
      public:
	typedef cube_type<I, Exact>			self_type; ///< Self type of the class.
	attr_type_decl(self_type);
	typedef abstract::image<mlc_exact_type(I)>	im_type; ///< Image type.
	typedef oln_point_type(im_type)			point_type; ///< Point type associated to im_type.
	typedef oln_dpoint_type(im_type)			dpoint_type; ///< Dpoint type associated to im_type.

	enum {dim = point_traits<point_type>::dim};

	/*!
	** \brief Basic Ctor.
	**
	** \warning  After this  call, the  object is  only instantiated
	** (not initialized).
	*/
	cube_type()
	  {
	  }

	/*!
	** \brief Ctor from a lambda_type value.
	**
	** \arg lambda Value of the attribute.
	*/
	cube_type(const lambda_type &lambda):
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

	/*!
	** \brief Ctor from a point and an image.
	**
	** \arg p Point to consider in the image.
	*/
	cube_type(const im_type&,
		  const point_type &p,
		  const env_type &):
	  mins_(dim), maxs_(dim), value_(ntg_zero_val(value_type))
	  {
	    for (int i = 0; i < dim; ++i)
	      mins_[i] = maxs_[i] = p.nth(i);
	  }

	/*!
	** \brief Accessor to minimums.
	**
	** Virtual method.
	** \arg i Index of the minimum wanted.
	** \return the i th minimum.
	** \see getMin_impl()
	*/
	int getMin(int i) const
	  {
	    mlc_dispatch(getMin)(i);
	  };

	/*!
	** \brief Accessor to maximums.
	**
	** Virtual method.
	** \arg i Index of the minimum wanted.
	** \return the i th maximum.
	** \see getMax_impl()
	*/
	int getMax(int i) const
	  {
	    mlc_dispatch(getMax)(i);
	  };

	// impl
	/*!
	** \brief Implementation of getMin(int i).
	**
	** Override this  method in  order to provide  a new  version of
	** getMin(int i).
	**
	** \warning Do not call this method, use getMin() instead.
	*/
	int getMin_impl(int i) const
	  {
	    precondition(i < dim);
	    return mins_[i];
	  };

	/*!
	** \brief Implementation of getMax(int i).
	**
	** Override this  method in  order to provide  a new  version of
	** getMax(int i).
	**
	** \warning Do not call this method, use getMax() instead.
	*/
	int getMax_impl(int i) const
	  {
	    precondition(i < dim);
	    return maxs_[i];
	  };

	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	void pe_impl(const cube_type &rhs)
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

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    return value_ < lambda;
	  }

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    return value_ != lambda;
	  };

      protected:
	std::vector<coord>	mins_;
	std::vector<coord>	maxs_;
	value_type		value_;
      };



      /*------*
	| box |
	*-----*/
      /*!
      ** \brief Box attribute.
      ** \param I Exact type of images to process.
      ** \param Exact The exact type.
      */
      template <class I, class Exact = mlc::final>
      class box_type:
	public attribute<mlc_2_exact_vt_type(box_type, I, Exact)>
      {
      public:
	typedef box_type<I, Exact>			self_type; ///< Self type of the class.
	attr_type_decl(self_type);
	typedef abstract::image<mlc_exact_type(I)>	im_type; ///< Image type.
	typedef oln_point_type(im_type)			point_type; ///< Point type associated to im_type.
	typedef oln_dpoint_type(im_type)			dpoint_type; ///< Dpoint type associated to im_type.
	enum {dim = point_traits<point_type>::dim };

	/*!
	** \brief Ctor from a lambda_type value.
	**
	** \arg lambda Value of the attribute.
	*/
	box_type(const lambda_type &lambda): maxs_(dim), mins_(dim)
	  {
	    for (int i = 0; i < dim; ++i)
	      {
		mins_[i] = ntg_zero_val(value_type);
		maxs_[i] = lambda[i];
	      }
	  };

	/*!
	** \brief Basic Ctor.
	**
	** \warning  After this  call, the  object is  only instantiated
	** (not initialized).
	*/
	box_type()
	  {
	  };

	/*!
	** \brief Ctor from a point and an image.
	**
	** \arg p Point to consider in the image.
	*/
	box_type(const im_type&, const point_type &p, const env_type &): maxs_(dim), mins_(dim)
	  {
	    for (int i = 0; i < dim; ++i)
	      mins_[i] = maxs_[i] = p.nth(i);
	  };

	/*!
	** \brief Accessor to minimums.
	**
	** Virtual method.
	** \arg i Index of the minimum wanted.
	** \return the i th minimum.
	** \see getMin_impl()
	*/
	value_type getMin(int i) const
	  {
	    mlc_dispatch(getMin)(i);
	  };


	/*!
	** \brief Accessor to maximums.
	**
	** Virtual method.
	** \arg i Index of the minimum wanted.
	** \return the i th maximum.
	** \see getMax_impl()
	*/
	value_type getMax(int i) const
	  {
	    mlc_dispatch(getMax)(i);
	  };

	// impl
	/*!
	** \brief Accessor to minimums.
	**
	** Virtual method.
	** \arg i Index of the minimum wanted.
	** \return the i th minimum.
	** \see getMin_impl()
	*/
	value_type getMin_impl(int i) const
	  {
	    precondition(i < point_traits<point_type>::dim);
	    return mins_[i];
	  };

	/*!
	** \brief Accessor to maximums.
	**
	** Virtual method.
	** \arg i Index of the minimum wanted.
	** \return the i th maximum.
	** \see getMax_impl()
	*/
	value_type getMax_impl(int i) const
	  {
	    precondition(i < point_traits<point_type>::dim);
	    return maxs_[i];
	  };

	/*!
	** \brief += operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	void pe_impl(const box_type &rhs)
	  {
	    for (int i = 0; i < dim; ++i)
	      {
		mins_[i] = ntg::min(mins_[i], rhs.getMin(i));
		maxs_[i] = ntg::max(maxs_[i], rhs.getMax(i));
	      }
	  }

	/*!
	** \brief "<" operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool less_impl(const lambda_type &lambda) const
	  {
	    for (int i = 0; i < dim; ++i)
	      if ((maxs_[i] - mins_[i]) >= lambda[i])
		return false;
	    return true;
	  }

	/*!
	** \brief != operator implementation.
	**
	** This is an implementation  of the += operator.  Override this
	** method to provide a new implementation of this operator.
	** \warning This method SHOULDN'T directly be called.
	*/
	bool ne_impl(const lambda_type &lambda) const
	  {
	    for (int i = 0; i < dim; ++i)
	      if ((maxs_[i] - mins_[i]) == lambda[i])
		return false;
	    return true;
	  };

      protected:
	std::vector<value_type>	maxs_; ///< List of minimums.
	std::vector<value_type>	mins_; ///< List of maximums.
      };

      /*-------------------------
	| traits specializations |
	\------------------------*/


      /*!
      ** \brief Trait specialization for the integral attribute.
      */
      template <class T, class Exact>
      struct attr_traits<integral_type<T, Exact> >
      {
	typedef T		value_type; ///< Type of data.
	typedef value_type	lambda_type; ///< Type of lambda.
	typedef env::NullEnv	env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for the height attribute.
      */
      template <class T, class Exact>
      struct attr_traits<height_type<T, Exact> >
      {
	typedef T		value_type; ///< Type of data.
	typedef value_type	lambda_type; ///< Type of lambda.
	typedef env::NullEnv	env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for card attribute.
      */
      template <class T, class Exact>
      struct attr_traits<card_type<T, Exact> >
      {
	typedef T		value_type; ///< Type of data.
	typedef value_type	lambda_type; ///< Type of lambda.
	typedef env::NullEnv	env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for the maxvalue attribute.
      */
      template <class T, class Exact>
      struct attr_traits<maxvalue_type<T, Exact> >
      {
	typedef T		value_type; ///< Type of data.
	typedef value_type	lambda_type; ///< Type of lambda.
	typedef env::NullEnv	env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for the minvalue attribute.
      */
      template <class T, class Exact>
      struct attr_traits<minvalue_type<T, Exact> >
      {
	typedef T		value_type; ///< Type of data.
	typedef value_type	lambda_type; ///< Type of lambda.
	typedef env::NullEnv	env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for the ball attribute.
      */
      template <class I, class Exact>
      struct attr_traits<ball_type<I, Exact> >
      {
	typedef unsigned			value_type; ///< Type of data.
	typedef value_type			lambda_type; ///< Type of lambda.
	typedef oln::morpho::env::NullEnv	env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for the dist attribute.
      */
      template <class I, class Exact>
      struct attr_traits<dist_type<I, Exact> >
      {
	typedef float				value_type; ///< Type of data.
	typedef value_type			lambda_type; ///< Type of lambda.
	typedef oln::morpho::env::NullEnv	env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for the cube attribute.
      */
      template <class I, class Exact>
      struct attr_traits<cube_type<I, Exact> >
      {
	typedef unsigned			value_type; ///< Type of data.
	typedef value_type			lambda_type; ///< Type of lambda.
	typedef oln::morpho::env::NullEnv	env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for the box attribute.
      */
      template <class I, class Exact>
      struct attr_traits<box_type<I, Exact> >
      {
	typedef unsigned					value_type; ///< Type of data.
	typedef ntg::vec<I::dim, value_type, mlc::final>	lambda_type; ///< Type of lambda.
	typedef oln::morpho::env::NullEnv			env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for the other_image attribute.
      */
      template <class Dad, class I, class Exact>
      struct attr_traits<other_image<Dad, I, Exact> >
      {
	//typedef typename change_exact<Dad, other_image<Dad, I, Exact> >::ret	super_type;
	typedef typename change_exact<Dad,
				      typename mlc::exact_vt<other_image<Dad, I, Exact>,
							     Exact>::ret>::ret		super_type; ///< Parent class type.
	typedef attr_value_type(super_type)						value_type; ///< Type of data.
	typedef attr_lambda_type(super_type)						lambda_type; ///< Type of lambda.
	typedef oln::morpho::env::OtherImageEnv<I>					env_type; ///< Type of environment.
      };

      /*!
      ** \brief Trait specialization for the ball_parent_change attribute.
      */
      template <class I, class Exact>
      struct attr_traits<ball_parent_change<I, Exact> >
      {
	typedef unsigned		value_type; ///< Type of data.
	typedef value_type		lambda_type; ///< Type of lambda.
	typedef env::ParentEnv<I>	env_type; ///< Type of environment.
      };

      // traits for other_image derivation

      /*!
      ** \brief Change the exact type of an attribute.
      **
      ** Traits to change integral_type exact type.
      */
      template <class NewExact, class OldExact, class T>
      struct change_exact<integral_type<T, OldExact>, NewExact>
      {
	typedef integral_type<T, NewExact>	ret;
      };

    }// !attr
  } // !morpho
} //!oln

  /*-----------*
    |  diamond  |
    *-----------*/

  // FIXME: to be written...


#endif // !OLN_MORPHO_ATTRIBUTES


