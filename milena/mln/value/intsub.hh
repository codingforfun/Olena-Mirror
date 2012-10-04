// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_INTSUB_HH
# define MLN_VALUE_INTSUB_HH

/// \file
///
/// Define a subdivided integer value class.

# include <cstdlib>
# include <iostream>
# include <sstream>
# include <algorithm>
# include <mln/core/routine/ops.hh>
# include <mln/value/ops.hh>
# include <mln/value/builtin/ops.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/concept/floating.hh>
# include <mln/value/iota.hh>
# include <mln/value/prev.hh>
# include <mln/value/succ.hh>


namespace mln
{

  // Forward declaration
  namespace value {
    template <unsigned n> class intsub;
  }

  namespace trait
  {

    template <unsigned n>
    struct set_precise_unary_< op::uminus, mln::value::intsub<n> >
    {
      typedef mln::value::intsub<n> ret;
    };


    template <unsigned n>
    struct value_< mln::value::intsub<n> >
    {
    private:
      typedef mln::value::intsub<n> self_;
      typedef int enc_;
    public:

      enum constants_ {
	dim = 1,
	nbits = 32,
	card  = mln_value_card_from_(32) / n
      };

      typedef trait::value::nature::integer nature;
      typedef trait::value::kind::data      kind;
      typedef mln_value_quant_from_(card)   quant;

      static const self_ max() { return mln::value::internal::limits<int>::max() / n; }
      static const self_ min() { return - max(); }
      static const self_ epsilon() { return 0; }

      typedef mln::value::intsub<n> comp;

      typedef mln::value::intsub<n> sum;

      static const char* name()
      {
	static std::string
	  s = mln::value::internal::make_generic_name("intsub", n);
	return s.c_str();
      }

    };

  } // end of namespace mln::trait



  namespace value
  {

    template <unsigned n>
    class intsub :
      public value::Floating< intsub<n> >,
      public value::internal::value_like_< float,    // Equivalent.
					   int, // Enc.
					   float,         // Interoperation.
					   intsub<n> >   // Exact.
    {
    public:
      intsub();
      intsub(const intsub<n>& rhs);
      /// Construct an intsub with value : \p int_part + 1 / \p denominator.
      intsub(int int_part, unsigned denominator);
      intsub(int i);
      intsub(float i);
      intsub(double i);

      intsub<n>& operator=(const intsub<n>& rhs);
      intsub<n>& operator=(int i);
      intsub<n>& operator=(float i);
      intsub<n>& operator=(double i);

      /// Is an integer value.
      bool is_integer() const;

      /// Convert this intsub to a larger intsub type.
      template <unsigned m>
      operator intsub<m>();

      /*! \internal Increment by value::iota::value() the
	value to the next one.
       */
      void inc_();
      /*! \internal Decrement by value::iota::value() the
	value to the next one.
       */
      void dec_();

      /*!\internal Return the integer part of this value.
       */
      int to_int() const;

      /*!\internal Construct a intsub using an encoding value. */
      static intsub<n> make_from_enc_(int enc);

      /// Conversion to a float.
      operator float() const;

      /// Unary operator minus.
      intsub<n> operator-() const;

      /// Explicit conversion towards equivalent type.
      float to_equiv() const;

      /// Explicit conversion towards interoperation type.
      float to_interop() const;
    };


    // Safety
    template <> struct intsub<0>;

    //  rounding

    /// Re-implementation of the floor function. \sa math::floor
    template <unsigned n>
    intsub<n> floor_(const intsub<n>& i);
    /// Re-implementation of the ceil function. \sa math::ceil
    template <unsigned n>
    intsub<n> ceil_(const intsub<n>& i);

    // Other ops (overloads of generic ones)

    /// Re-implementation of the min function. \sa math::min
    template <unsigned n>
    intsub<n> min_(const intsub<n>& u1, const intsub<n>& u2);
    /// Re-implementation of the max function. \sa math::max
    template <unsigned n>
    intsub<n> max_(const intsub<n>& u1, const intsub<n>& u2);

    /// Specific implementation of the mean function.
    template <unsigned n>
    intsub<2*n> mean(const intsub<n>& v1, const intsub<n>& v2);

    /// Specific implementation of the mean function.
    template <unsigned n>
    intsub<4*n> mean(const intsub<n>& v1, const intsub<n>& v2,
		     const intsub<n>& v3, const intsub<n>& v4);

    /// Specific implementation of the median function.
    template <unsigned n>
    intsub<2*n> median(const intsub<n>& v1, const intsub<n>& v2);

    /// Specific implementation of the median function.
    template <unsigned n>
    intsub<4*n> median(const intsub<n>& v1, const intsub<n>& v2,
		       const intsub<n>& v3, const intsub<n>& v4);


    // Iota

    template <unsigned n>
    struct iota<intsub<n> >
    {
      static intsub<n> value();
    };

    typedef intsub<1> intsub1;
    typedef intsub<2> intsub2;
    typedef intsub<4> intsub4;


  } // end of namespace mln::value

  extern const value::intsub<2> half;
  extern const value::intsub<4> quarter;

# ifndef MLN_INCLUDE_ONLY

  //  half

#  ifndef MLN_WO_GLOBAL_VARS

  const value::intsub<2> half = value::intsub<2>(0, 2);
  const value::intsub<4> quarter = value::intsub<4>(0, 4);

#  endif // ! MLN_WO_GLOBAL_VARS



  namespace value
  {

    namespace internal
    {

      /// \internal Test if (f * v) is an integer; an error less than
      /// epsilon is ok
      bool
      mult_is_integer(float f, int v)
      {
	float a = f * v;
	float b = float(unsigned(a));
	const float epsilon = 0.00001f;

	if (a > b)
	  return a - b < epsilon;
	else
	  return b - a < epsilon;
      }

    } // end of namespace mln::value::internal


    template <unsigned n>
    intsub<n>::intsub()
    {
    }

    template <unsigned n>
    intsub<n>::intsub(const intsub<n>& rhs)
    {
      this->v_ = rhs.v_;
    }

    template <unsigned n>
    intsub<n>::intsub(int i)
    {
      this->v_ = n * i;
    }

    template <unsigned n>
    intsub<n>::intsub(float i)
    {
      mln_precondition(internal::mult_is_integer(i, n));
      this->v_ = n * i;
    }

    template <unsigned n>
    intsub<n>::intsub(double i)
    {
      mln_precondition(internal::mult_is_integer(i, n));
      this->v_ = n * i;
    }


    template <unsigned n>
    intsub<n>::intsub(int int_part, unsigned denominator)
    {
      mln_precondition(denominator <= n);
      this->v_ = int_part * n + denominator / n;
    }


    template <unsigned n>
    intsub<n>&
    intsub<n>::operator=(const intsub<n>& rhs)
    {
      this->v_ = rhs.v_;
      return *this;
    }

    template <unsigned n>
    intsub<n>&
    intsub<n>::operator=(int i)
    {
      this->v_ = n * i;
      return *this;
    }

    template <unsigned n>
    intsub<n>&
    intsub<n>::operator=(float i)
    {
      mln_precondition(internal::mult_is_integer(i, n));
      this->v_ = n * i;
      return *this;
    }

    template <unsigned n>
    intsub<n>&
    intsub<n>::operator=(double i)
    {
      mln_precondition(internal::mult_is_integer(i, n));
      this->v_ = n * i;
      return *this;
    }


    template <unsigned n>
    intsub<n> intsub<n>::make_from_enc_(int enc)
    {
      intsub<n> i;
      i.v_ = enc;
      return i;
    }

    template <unsigned n>
    bool
    intsub<n>::is_integer() const
    {
      return this->v_ % n == 0;
    }

    template <unsigned n>
    template <unsigned m>
    intsub<n>::operator intsub<m>()
    {
      mln_precondition(n <= m);
      return intsub<m>::make_from_enc_(this->v_ * m / n);
    }

    template <unsigned n>
    void
    intsub<n>::inc_()
    {
      this->v_ += iota<intsub<n> >::value().to_enc();
    }

    template <unsigned n>
    void
    intsub<n>::dec_()
    {
      this->v_ -= iota<intsub<n> >::value().to_enc();
    }

    template <unsigned n>
    int
    intsub<n>::to_int() const
    {
      return this->v_ / n;
    }

    template <unsigned n>
    intsub<n>::operator float() const
    {
      return to_equiv();
    }

    template <unsigned n>
    intsub<n>
    intsub<n>::operator-() const
    {
      return intsub<n>::make_from_enc_(this->v_ * -1);
    }

    template <unsigned n>
    float
    intsub<n>::to_equiv() const
    {
      return float(this->v_) / float(n);
    }

    template <unsigned n>
    float
    intsub<n>::to_interop() const
    {
      return float(this->v_) / float(n);
    }

    // Iota

    template <unsigned n>
    intsub<n>
    iota<intsub<n> >::value()
    {
      return intsub<n>(0,n);
    }


    //  rounding

    template <unsigned n>
    intsub<n> floor_(const intsub<n>& i)
    {
      return i.is_integer() ? i : value::prev(i);
    }

    template <unsigned n>
    intsub<n> ceil_(const intsub<n>& i)
    {
      return i.is_integer() ? i : value::succ(i);
    }


    // Other operators (overloads of generic ones)

    template <unsigned n>
    intsub<n> min_(const intsub<n>& v1, const intsub<n>& v2)
    {
      return intsub<n>::make_from_enc_(v1.to_enc() < v2.to_enc() ? v1.to_enc() : v2.to_enc());
    }

    template <unsigned n>
    intsub<n> max_(const intsub<n>& v1, const intsub<n>& v2)
    {
      return intsub<n>::make_from_enc_(v1.to_enc() > v2.to_enc() ? v1.to_enc() : v2.to_enc());
    }

    // FIXME: Make use of mean_() overloads with math::mean. Require
    // to fix an issue with the return type which differs according to
    // the overload : with 2 (intsub<2*n>) or 4 (intsub<4*n>)
    // arguments.
    template <unsigned n>
    intsub<2*n> mean(const intsub<n>& v1, const intsub<n>& v2)
    {
      return intsub<2*n>::make_from_enc_((v1.to_enc() + v2.to_enc()));
    }

    // FIXME: Make use of mean_() overloads with math::mean. Require
    // to fix an issue with the return type which differs according to
    // the overload : with 2 (intsub<2*n>) or 4 (intsub<4*n>)
    // arguments.
    template <unsigned n>
    intsub<4*n> mean(const intsub<n>& v1, const intsub<n>& v2,
		     const intsub<n>& v3, const intsub<n>& v4)
    {
      return mean_(mean_(v1, v2), mean_(v3, v4));
    }

    // FIXME: Make use of median_() overloads with
    // math::median. Require to fix an issue with the return type
    // which differs according to the overload : with 2 (intsub<2*n>)
    // or 4 (intsub<4*n>) arguments.
    template <unsigned n>
    intsub<2*n> median(const intsub<n>& v1, const intsub<n>& v2)
    {
      return mean(v1, v2);
    }

    // FIXME: Make use of median_() overloads with
    // math::median. Require to fix an issue with the return type
    // which differs according to the overload : with 2 (intsub<2*n>)
    // or 4 (intsub<4*n>) arguments.
    template <unsigned n>
    intsub<4*n> median(const intsub<n>& v1, const intsub<n>& v2,
		       const intsub<n>& v3, const intsub<n>& v4)
    {
      std::vector<intsub<n> > vec(4);
      vec.push_back(v1);
      vec.push_back(v2);
      vec.push_back(v3);
      vec.push_back(v4);
      std::sort(vec.begin(), vec.end());
      return mean(vec[1], vec[2]);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_INTSUB_HH
