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

#ifndef MLN_METAL_IS_FROM_TO_CONVERTIBLE_HH
# define MLN_METAL_IS_FROM_TO_CONVERTIBLE_HH

/// \file
///
/// \brief Definition of a type that checks if a from_to_() conversion
/// routine exists.

/// FIXME: Enable this include if the compiler support c++-11
# include <type_traits>
# include <mln/metal/bool.hh>

# define mlc_is_from_to_convertible(T, U) mln::metal::is_from_to_convertible< T, U >



namespace mln
{

  // Forward declarations.
  namespace value {
    template <unsigned n> struct int_u;
    template <unsigned n> struct int_s;
  }


  namespace metal
  {

    namespace internal
    {

      template <typename T, typename U>
      struct helper_is_from_to_convertible_
      {
	static no_  selector(...);


	/// FIXME: Enable this implementation if the compiler supports it.

	// template <typename V>
	// static yes_ selector(V, typeof(from_to_(*(V*)0, static_cast<U&>(*(U*)0)))* = 0);

	/// FIXME: Enable this implementation with latest compilers supporting c++-11.

	template <typename V, typename C = decltype(from_to_(V(), std::declval<U&>()))>
	static yes_ selector(V);

	static const bool value = sizeof(selector(T())) == sizeof(char);
      };

    } // end of namespace mln::metal::internal



    /*!
      \internal
      \brief Checks if a from_to_() conversion routine exists.

      \warning This structure may use C++-11 features if available or
      typeof keyword which is a specific feature of g++. Be sure that
      your compiler supports them.
    */
    template <typename T, typename U>
    struct is_from_to_convertible : bool_<internal::helper_is_from_to_convertible_<T,U>::value>
    {
    };


  } // end of namespace mln::metal



} // end of namespace mln


#endif // ! MLN_METAL_IS_FROM_TO_CONVERTIBLE_HH
