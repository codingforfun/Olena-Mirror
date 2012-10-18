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

#ifndef MLN_FUN_VVVV2V_SPAN_HH
# define MLN_FUN_VVVV2V_SPAN_HH

/// \file
///
/// Functor that computes the spanimum of two values.

# include <mln/core/concept/function.hh>
# include <mln/value/interval.hh>


namespace mln
{

  namespace fun
  {

    namespace vvvv2v
    {

      // FIXME: Doc.

      /// \brief A functor computing the span of two interval values.
      template <typename T>
      struct span : public Function_vvvv2v< span<T> >
      {
	typedef value::interval<T> result;

	value::interval<T> operator()(const value::interval<T>& v1,
				      const value::interval<T>& v2,
				      const value::interval<T>& v3,
				      const value::interval<T>& v4) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename T>
      value::interval<T>
      span<T>::operator()(const value::interval<T>& v1,
			  const value::interval<T>& v2,
			  const value::interval<T>& v3,
			  const value::interval<T>& v4) const
      {
	return value::span(v1, v2, v3, v4);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::vvvv2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_VVVV2V_SPAN_HH
