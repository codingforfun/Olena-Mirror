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

#ifndef MLN_ACCU_MATH_SPAN_HH
# define MLN_ACCU_MATH_SPAN_HH

/// \file
///
/// Define an accumulator that computes a span.

# include <mln/accu/internal/base.hh>

namespace mln
{

  namespace accu
  {

    namespace math
    {


      /// \brief Generic span accumulator class.
      ///
      /// The parameter \c T is the type of values used in the
      /// underlying interval.
      ///
      /// \ingroup modaccuvalues
      //
      template <typename T, typename R = T>
      struct span : public mln::accu::internal::base< const R&, span<T,R> >
      {
	typedef T argument;

	span();

	/// Manipulators.
	/// \{
	void init();
	void take_as_init_(const argument& t);
	void take(const argument& t);
	void take(const span<T,R>& other);
	/// \}

	/// Get the value of the accumulator.
	const R& to_result() const;

	/// Check whether this accu is able to return a result.
	/// Always true here.
	bool is_valid() const;

      protected:

	R t_;
	bool is_valid_;
      };

    } // end of mln::accu::math




# ifndef MLN_INCLUDE_ONLY

    namespace math
    {

      template <typename T, typename R>
      inline
      span<T,R>::span()
      {
	init();
      }

      template <typename T, typename R>
      inline
      void
      span<T,R>::init()
      {
	is_valid_ = false;
      }

      template <typename T, typename R>
      inline
      void span<T,R>::take_as_init_(const argument& t)
      {
	t_ = t;
	is_valid_ = true;
      }

      template <typename T, typename R>
      inline
      void span<T,R>::take(const argument& t)
      {
	if (is_valid_)
	  this->t_ = value::span(this->t_, t);
	else
	{
	  this->t_ = t;
	  if (!is_valid_)
	    is_valid_ = true;
	}
      }

      template <typename T, typename R>
      inline
      void
      span<T,R>::take(const span<T,R>& other)
      {
	mln_precondition(other.is_valid());

	if (is_valid_)
	  this->t_ = value::span(this->t_, other.t_);
	else
	{
	  this->t_ = other.t_;
	  is_valid_ = other.is_valid_;
	}
      }

      template <typename T, typename R>
      inline
      const R&
      span<T,R>::to_result() const
      {
	return t_;
      }

      template <typename T, typename R>
      inline
      bool
      span<T,R>::is_valid() const
      {
	return is_valid_;
      }

    } // end of namespace mln::accu::math

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MATH_SPAN_HH
