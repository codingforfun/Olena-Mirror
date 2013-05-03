// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_MACROS_HH
# define MLN_CORE_MACROS_HH

/// \file
///
/// Definition of the set of Milena macros.


/*!
  \def mln_deduce(T, A1, A2)
  \brief Shortcut to access the type T::A1::A2.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_deduce_(T, A1, A2)
  \brief Shortcut to access the type T::A1::A2.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the type T::A1::A2.
/// \{
# define mln_deduce(T, A1, A2) typename T::A1::A2
# define mln_deduce_(T, A1, A2) T::A1::A2
/// \}

// a

/*!
  \def mln_argument(T)
  \brief Shortcut to access the argument type associated to T.

  To be used in templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/*!
  \def mln_argument_(T)
  \brief Shortcut to access the argument type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/// \brief Shortcuts to access the argument type associated to T.
/// \{
# define mln_argument(T)      typename T::argument
# define mln_argument_(T)     T::argument
/// \}


// b

/*!
  \def mln_bkd_eiter(T)
  \brief Shortcut to access the element-backward-iterator type associated
  to T.

  To be used in templated functions only.
  \ingroup macrositerelt
*/
/*!
  \def mln_bkd_eiter_(T)
  \brief Shortcut to access the element-backward-iterator type associated
  to T.

  To be used in non-templated functions only.
  \ingroup macrositerelt
*/
/// \brief Shortcuts to access the element-backward-iterator type
/// associated to T.
/// \{
# define mln_bkd_eiter(T)   typename T::bkd_eiter
# define mln_bkd_eiter_(T)           T::bkd_eiter
/// \}

/*!
  \def mln_bkd_niter(T)

  \brief Shortcut to access the backward neighbor iterator type
  associated to a neighborhood of type T.

  \param T A type of a class inheriting from mln::Neighborhood.

  To be used in templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
/*!
  \def mln_bkd_niter_(T)

  \brief Shortcut to access the backward neighbor iterator type
  associated to a neighborhood of type T.

  \param T A type of a class inheriting from mln::Neighborhood.

  To be used in non-templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
/// Shortcuts to access the backward neighbor iterator type associated
/// to a neighborhood of type T.
/// \{
# define mln_bkd_niter(T)  typename T::bkd_niter
# define mln_bkd_niter_(T)          T::bkd_niter
/// \}

/*!
  \def mln_bkd_piter(T)
  \brief Shortcut to access the backward site iterator type
  associated to T.

  \param T A type of a class inheriting from mln::Image or
  mln::Site_set.

  To be used in templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/*!
  \def mln_bkd_piter_(T)
  \brief Shortcut to access the backward site iterator type
  associated to T.

  \param T A type of a class inheriting from mln::Image or
  mln::Site_set.

  To be used in non-templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/// Shortcuts to access the backward site iterator type associated to
/// T.
/// \{
# define mln_bkd_piter(T)  typename T::bkd_piter
# define mln_bkd_piter_(T)          T::bkd_piter
/// \}

/*!
  \def mln_bkd_qiter(T)

  \brief Shortcut to access the backward neighbor iterator type
  associated to a window of type T.

  \param T A type of a class inheriting from mln::Window.

  To be used in templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
/*!
  \def mln_bkd_qiter_(T)

  \brief Shortcut to access the backward neighbor iterator type
  associated to a window of type T.

  \param T A type of a class inheriting from mln::Window.

  To be used in non-templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
/// Shortcuts to access the backward neighbor iterator type associated
/// to a window of type T.
/// \{
# define mln_bkd_qiter(T)  typename T::bkd_qiter
# define mln_bkd_qiter_(T)          T::bkd_qiter
/// \}

/*!
  \def mln_bkd_viter(T)
  \brief Shortcut to access the bkd_viter type associated to T.

  To be used in templated functions only.
  \ingroup macrositerval
  \relates mln::Value
*/
/*!
  \def mln_bkd_viter_(T)
  \brief Shortcut to access the bkd_viter type associated to T.

  To be used in non-templated functions only.
  \ingroup macrositerval
  \relates mln::Value
*/
/// Shortcuts to access the bkd_viter type associated to T.
/// \{
# define mln_bkd_viter(T)  typename T::bkd_viter
# define mln_bkd_viter_(T)          T::bkd_viter
/// \}

/*!
  \def mln_box(T)
  \brief Shortcut to access the box type associated to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image mln::Site_Set
*/
/*!
  \def mln_box_(T)
  \brief Shortcut to access the box type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image mln::Site_Set
*/
/// Shortcuts to access the box type associated to T.
/// \{
# define mln_box(T)        typename mln::box<mln_psite(T)>
# define mln_box_(T)                mln::box<mln_psite(T)>
/// \}


// c

/*!
  \def mln_coord(T)
  \brief Shortcut to access the coord type associated to T.

  To be used in templated functions only.
  \ingroup macrossite
  \relates mln::GPoint
*/
/*!
  \def mln_coord_(T)
  \brief Shortcut to access the coord type associated to T.

  To be used in non-templated functions only.
  \ingroup macrossite
  \relates mln::GPoint
*/
/// Shortcuts to access the coord type associated to T.
/// \{
# define mln_coord(T)      typename T::coord
# define mln_coord_(T)              T::coord
/// \}


// d

/*!
  \def mln_delta(T)
  \brief Shortcut to access the delta type associated to T.

  To be used in templated functions only.
  \ingroup macrossite
  \relates mln::Image
*/
/*!
  \def mln_delta_(T)
  \brief Shortcut to access the delta type associated to T.

  To be used in non-templated functions only.
  \ingroup macrossite
  \relates mln::Image
*/
/// Shortcuts to access the delta type associated to T.
/// \{
# define mln_delta(T)      typename T::delta
# define mln_delta_(T)              T::delta
/// \}

/*!
  \def mln_dpoint(T)
  \brief Shortcut to access the dpoint type associated to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_dpoint_(T)
  \brief Shortcut to access the dpoint type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the dpoint type associated to T.
/// \{
# define mln_dpoint(T)     typename T::dpoint
# define mln_dpoint_(T)             T::dpoint
/// \}

/*!
  \def mln_dpsite(T)
  \brief Shortcut to access the dpsite type associated to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_dpsite_(T)
  \brief Shortcut to access the dpsite type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the dpsite type associated to T.
/// \{
# define mln_dpsite(T)     typename T::dpsite
# define mln_dpsite_(T)             T::dpsite
/// \}


// e

/*!
  \def mln_eiter(T)
  \brief Shortcut to access the element-iterator type associated to T.

  To be used in templated functions only.
  \ingroup macrositerval
*/
/*!
  \def mln_eiter_(T)
  \brief Shortcut to access the element-iterator type associated to T.

  To be used in non-templated functions only.
  \ingroup macrositerval
*/
/// Shortcuts to access the element-iterator type associated to T.
/// \{
# define mln_eiter(T)      typename T::eiter
# define mln_eiter_(T)              T::eiter
/// \}

/*!
  \def mln_graph(T)
  \brief Shortcut to access the graph type associated to T.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_graph_(T)
  \brief Shortcut to access the graph type associated to T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the graph type associated to T.
/// \{
# define mln_graph(T)         typename T::graph_t
# define mln_graph_(T)                 T::graph_t
/// \}

/*!
  \def mln_element(T)
  \brief Shortcut to access the element type associated to T.

  To be used in templated functions only.
  \ingroup mlnmacros
  \relates mln::Image
*/
/*!
  \def mln_element_(T)
  \brief Shortcut to access the element type associated to T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
  \relates mln::Image
*/
/// Shortcuts to access the element type associated to T.
/// \{
# define mln_element(T)        typename T::element
# define mln_element_(T)                T::element
/// \}

/*!
  \def mln_graph_element(T)
  \brief Shortcut to access the graph_element type associated to T.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_graph_element_(T)
  \brief Shortcut to access the graph_element type associated to T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the graph element type associated to T.
/// \{
# define mln_graph_element(T)        typename T::graph_element
# define mln_graph_element_(T)                T::graph_element
/// \}

/*!
  \def mln_enc(T)
  \brief Shortcut to access the encoding type associated to T.

  To be used in templated functions only.
  \ingroup macrosval
  \relates mln::Value
*/
/*!
  \def mln_enc_(T)
  \brief Shortcut to access the encoding type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosval
  \relates mln::Value
*/
/// Shortcuts to access the encoding type associated to T.
/// \{
# define mln_enc(T)        typename T::enc
# define mln_enc_(T)                T::enc
/// \}

/*!
  \def mln_equiv(T)
  \brief Shortcut to access the equivalent type associated to T.

  To be used in templated functions only.
  \ingroup macrosval
  \relates mln::Value
*/
/*!
  \def mln_equiv_(T)
  \brief Shortcut to access the equivalent type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosval
  \relates mln::Value
*/
/// Shortcuts to access the equivalent type associated to T.
/// \{
# define mln_equiv(T)      typename T::equiv
# define mln_equiv_(T)              T::equiv
/// \}


// f

/*!
  \def mln_fwd_eiter(T)
  \brief Shortcut to access the element-forward-iterator type
  associated to T.

  To be used in templated functions only.
  \ingroup macrositerelt
*/
/*!
  \def mln_fwd_eiter_(T)
  \brief Shortcut to access the element-forward-iterator type
  associated to T.

  To be used in non-templated functions only.
  \ingroup macrositerelt
*/
/// Shortcuts to access the element-forward-iterator type associated
/// to T.
/// \{
# define mln_fwd_eiter(T)   typename T::fwd_eiter
# define mln_fwd_eiter_(T)           T::fwd_eiter
/// \}

/*!
  \def mln_fwd_niter(T)

  \brief Shortcut to access the forward neighbor iterator type
  associated to a neighborhood of type T.

  \param T A type of a class inheriting from mln::Neighborhood.

  To be used in templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
/*!
  \def mln_fwd_niter_(T)

  \brief Shortcut to access the forward neighbor iterator type
  associated to a neighborhood of type T.

  \param T A type of a class inheriting from mln::Neighborhood.

  To be used in non-templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
/// Shortcuts to access the forward neighbor iterator type associated
/// to a neighborhood of type T.
/// \{
# define mln_fwd_niter(T)  typename T::fwd_niter
# define mln_fwd_niter_(T)          T::fwd_niter
/// \}

/*!
  \def mln_fwd_piter(T)

  \brief Shortcut to access the forward site iterator type associated
  to T.

  \param T A type of a class inheriting from mln::Image or
  mln::Site_set.

  To be used in templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/*!
  \def mln_fwd_piter_(T)

  \brief Shortcut to access the forward site iterator type associated
  to T.

  \param T A type of a class inheriting from mln::Image or
  mln::Site_set.

  To be used in non-templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/// Shortcuts to access the forward site iterator type associated to
/// T.
/// \{
# define mln_fwd_piter(T)  typename T::fwd_piter
# define mln_fwd_piter_(T)          T::fwd_piter
/// \}

/*!
  \def mln_fwd_qiter(T)

  \brief Shortcut to access the forward neighbor iterator type
  associated to a window of type T.

  \param T A type of a class inheriting from mln::Window.

  To be used in templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
/*!
  \def mln_fwd_qiter_(T)

  \brief Shortcut to access the forward neighbor iterator type
  associated to a window of site T.

  \param T A type of a class inheriting from mln::Window.

  To be used in non-templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
/// Shortcuts to access the forward neighbor iterator type associated
/// to a window of site T.
/// \{
# define mln_fwd_qiter(T)  typename T::fwd_qiter
# define mln_fwd_qiter_(T)          T::fwd_qiter
/// \}

/*!
  \def mln_fwd_viter(T)
  \brief Shortcut to access the fwd_viter type associated to T.

  To be used in templated functions only.
  \ingroup macrositerval
  \relates mln::Value
*/
/*!
  \def mln_fwd_viter_(T)
  \brief Shortcut to access the fwd_viter type associated to T.

  To be used in non-templated functions only.
  \ingroup macrositerval
  \relates mln::Value
*/
/// Shortcuts to access the fwd_viter type associated to T.
/// \{
# define mln_fwd_viter(T)  typename T::fwd_viter
# define mln_fwd_viter_(T)          T::fwd_viter
/// \}


// g

/*!
  \def mln_geom(T)
  \brief Shortcut to access the geometry type associated to T.

  To be used in templated functions only.
  \ingroup mlnmacros
  \relates mln::Image
*/
/*!
  \def mln_geom_(T)
  \brief Shortcut to access the geometry type associated to T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
  \relates mln::Image
*/
/// Shortcuts to access the geometry type associated to T.
/// \{
# define mln_geom(T)      typename T::geom
# define mln_geom_(T)              T::geom
/// \}

/*!
  \def mln_grid(T)
  \brief Shortcut to access the grid type associated to T.

  To be used in templated functions only.
  \ingroup mlnmacros
  \relates mln::Image
*/
/*!
  \def mln_grid_(T)
  \brief Shortcut to access the grid type associated to T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
  \relates mln::Image
*/
/// Shortcuts to access the grid type associated to T.
/// \{
# define mln_grid(T)      typename T::grid
# define mln_grid_(T)              T::grid
/// \}


// i

/*!
  \def mln_i_element(T)
  \brief Shortcut to access the insertion-element type associated to
  T.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_i_element_(T)
  \brief Shortcut to access the insertion-element type associated to
  T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the insertion-element type associated to T.
/// \{
# define mln_i_element(T)  typename T::i_element
# define mln_i_element_(T)          T::i_element
/// \}

/*!
  \def mln_image(T)
  \brief Shortcut to access the image type associated to
  T.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_image_(T)
  \brief Shortcut to access the image type associated to
  T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the image type associated to T.
/// \{
# define mln_image(T)      typename T::image
# define mln_image_(T)              T::image
/// \}

/*!
  \def mln_invert(T)
  \brief Shortcut to access the invert type associated to
  T.

  To be used in templated functions only.
  \ingroup macrosval
  \relates mln::Value
*/
/*!
  \def mln_invert_(T)
  \brief Shortcut to access the invert type associated to
  T.

  To be used in non-templated functions only.
  \ingroup macrosval
  \relates mln::Value
*/
/// Shortcuts to access the invert type associated to T.
/// \{
# define mln_invert(T)	   typename T::invert
# define mln_invert_(T)             T::invert
/// \}


// l

/*!
  \def mln_lvalue(T)
  \brief Shortcut to access the lvalue type associated to
  T.

  To be used in templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/*!
  \def mln_lvalue_(T)
  \brief Shortcut to access the lvalue type associated to
  T.

  To be used in non-templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/// Shortcuts to access the lvalue type associated to T.
/// \{
# define mln_lvalue(T)     typename T::lvalue
# define mln_lvalue_(T)             T::lvalue
/// \}


// m

/*!
  \def mln_mesh(T)
  \brief Shortcut to access the mesh type associated to
  T.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_mesh_(T)
  \brief Shortcut to access the mesh type associated to
  T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the mesh type associated to T.
/// \{
# define mln_mesh(T)     typename T::mesh
# define mln_mesh_(T)             T::mesh
/// \}


// p

/*!
  \def mln_piter(T)

  \brief Shortcut to access the default site iterator type associated
  to T.

  \param T A type of a class inheriting from mln::Image or
  mln::Site_set.

  To be used in templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/*!
  \def mln_piter_(T)

  \brief Shortcut to access the default site iterator type associated
  to T.

  \param T A type of a class inheriting from mln::Image or
  mln::Site_set.

  To be used in non-templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/// Shortcuts to access the default site iterator type associated to
/// T.
/// \{
# define mln_piter(T)      typename T::piter
# define mln_piter_(T)              T::piter
/// \}

/*!
  \def mln_line_piter(T)
  \brief Shortcut to access the type of point by line iterator
  (line_piter) associated to T.

  \param T A type of a class inheriting from mln::Image or
  mln::Site_set.

  To be used in templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/*!
  \def mln_line_piter_(T)
  \brief Shortcut to access the type of point by line iterator
  (line_piter) associated to T.

  \param T A type of a class inheriting from mln::Image or
  mln::Site_set.

  To be used in non-templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/// Shortcuts to access the type of point by line iterator
/// (line_piter) associated to T.
/// \{
# define mln_line_piter(T) typename T::line_piter
# define mln_line_piter_(T)         T::line_piter
/// \}

/*!
  \def mln_domain(T)
  \brief Shortcut to access the domain type associated to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_domain_(T)
  \brief Shortcut to access the domain type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the domain type associated to T.
/// \{
# define mln_domain(T)       typename T::domain_t
# define mln_domain_(T)               T::domain_t
/// \}

/*!
  \def mln_pset(T)
  \brief Shortcut to access the type of point set (pset) associated
  to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_pset_(T)
  \brief Shortcut to access the type of point set (pset) associated
  to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the type of point set (pset) associated to T.
/// \{
# define mln_pset(T)       typename T::pset
# define mln_pset_(T)               T::pset
/// \}

/*!
  \def mln_psite(T)
  \brief Shortcut to access the type of point site (psite) associated
  to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_psite_(T)
  \brief Shortcut to access the type of point site (psite) associated
  to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the type of point site (psite) associated to T.
/// \{
# define mln_psite(T)      typename T::psite
# define mln_psite_(T)              T::psite
/// \}

/*!
  \def mln_point(T)
  \brief Shortcut to access the point type associated to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_point_(T)
  \brief Shortcut to access the point type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the point type associated to T.
/// \{
# define mln_point(T)      typename T::point
# define mln_point_(T)              T::point
/// \}


// q

/*!
  \def mln_qlf_value(T)
  \brief Shortcut to access the qualified (const or mutable) value
  type associated to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_qlf_value_(T)
  \brief Shortcut to access the qualified (const or mutable) value
  type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the qualified (const or mutable) value type
/// associated to T.
/// \{
# define mln_qlf_value(T)      typename mln::trait::qlf_value< T >::ret
# define mln_qlf_value_(T)              mln::trait::qlf_value< T >::ret
/// \}

/*!
  \def mln_qiter(T)

  \brief Shortcut to access the default neighbor iterator type
  associated to the window of type T.

  \param T A type of a class inheriting from mln::Window.

  To be used in templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
/*!
  \def mln_qiter_(T)

  \brief Shortcut to access the default neighbor iterator type
  associated to the window of type T.

  \param T A type of a class inheriting from mln::Window.

  To be used in non-templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
/// Shortcuts to access the default neighbor iterator type associated
/// to the window of type T.
/// \{
# define mln_qiter(T)      typename T::fwd_qiter
# define mln_qiter_(T)              T::fwd_qiter
/// \}

/*!
  \def mln_q_result(T)
  \brief Shortcut to access the qualified-result type associated to
  T.

  To be used in templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/*!
  \def mln_q_result_(T)
  \brief Shortcut to access the qualified-result type associated to
  T.

  To be used in non-templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/// Shortcuts to access the qualified-result type associated to T.
/// \{
# define mln_q_result(T)     typename T::q_result
# define mln_q_result_(T)    T::q_result
/// \}

/*!
  \def mln_q_subject(T)
  \brief Shortcut to access the qualified-subject type associated to
  T.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_q_subject_(T)
  \brief Shortcut to access the qualified-subject type associated to
  T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the qualified-subject type associated to T.
/// \{
# define mln_q_subject(T)      typename T::q_subject
# define mln_q_subject_(T)              T::q_subject
/// \}


// n

/*!
  \def mln_niter(T)

  \brief Shortcut to access the default neighbor iterator type
  associated to a neighborhood of type T.

  \param T A type of a class inheriting from mln::Neighborhood.

  To be used in templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
/*!
  \def mln_niter_(T)

  \brief Shortcut to access the default neighbor iterator type
  associated to a neighborhood of type T.

  \param T A type of a class inheriting from mln::Neighborhood.

  To be used in non-templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
/// Shortcuts to access the default neighbor iterator type associated
/// to a neighborhood of type T.
/// \{
# define mln_niter(T)      typename T::fwd_niter
# define mln_niter_(T)              T::fwd_niter
/// \}


// r

/*!
  \def mln_r_element(T)
  \brief Shortcut to access the removal-element type associated to T.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_r_element_(T)
  \brief Shortcut to access the removal-element type associated to T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the removal-element type associated to T.
/// \{
# define mln_r_element(T)  typename T::r_element
# define mln_r_element_(T)          T::r_element
/// \}

/*!
  \def mln_regular(T)
  \brief Shortcut to access the regular type associated to T.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_regular_(T)
  \brief Shortcut to access the regular type associated to T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the regular type associated to T.
/// \{
# define mln_regular(T)     typename T::regular
# define mln_regular_(T)    T::regular
/// \}

/*!
  \def mln_result(T)
  \brief Shortcut to access the result type associated to T.

  To be used in templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/*!
  \def mln_result_(T)
  \brief Shortcut to access the result type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/// Shortcuts to access the result type associated to T.
/// \{
# define mln_result(T)     typename T::result
# define mln_result_(T)    T::result
/// \}

/*!
  \def mln_rvalue(T)
  \brief Shortcut to access the value type associated to T.

  To be used in templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/*!
  \def mln_rvalue_(T)
  \brief Shortcut to access the value type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosfun
  \relates mln::Function
*/
/// Shortcuts to access the rvalue type associated to T.
/// \{
# define mln_rvalue(T)     typename T::rvalue
# define mln_rvalue_(T)             T::rvalue
/// \}


// s

/*!
  \def mln_site(T)
  \brief Shortcut to access the site type associated to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_site_(T)
  \brief Shortcut to access the site type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the site type associated to T.
/// \{
# define mln_site(T)      typename T::site
# define mln_site_(T)              T::site
/// \}

// /// Shortcuts to access the subject type associated to T.
// /// \{
// # define mln_subject(T)      typename T::subject
// # define mln_subject_(T)              T::subject
// /// \}


// t

/*!
  \def mln_target(T)
  \brief Shortcut to access the target type associated to T.

  To be used in templated functions only.
  \ingroup mlnmacros
*/
/*!
  \def mln_target_(T)
  \brief Shortcut to access the target type associated to T.

  To be used in non-templated functions only.
  \ingroup mlnmacros
*/
/// Shortcuts to access the target type associated to T.
/// \{
# define mln_target(T)      typename T::target
# define mln_target_(T)              T::target
/// \}



// u

/*!
  \def mln_unmorph(T)
  \brief Shortcut to access the unmorph type associated to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_unmorph_(T)
  \brief Shortcut to access the unmorph type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the unmorph type associated to T.
/// \{
# define mln_unmorph(T)      typename T::unmorph
# define mln_unmorph_(T)              T::unmorph
/// \}



// v

/*!
  \def mln_value(T)
  \brief Shortcut to access the value type associated to T.

  To be used in templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/*!
  \def mln_value_(T)
  \brief Shortcut to access the value type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosima
  \relates mln::Image
*/
/// Shortcuts to access the value type associated to T.
/// \{
# define mln_value(T)      typename T::value
# define mln_value_(T)              T::value
/// \}

/*!
  \def mln_vec(T)
  \brief Shortcut to access the algebra::vector (vec) type associated
  to T.

  To be used in templated functions only.
  \ingroup macrosval
*/
/*!
  \def mln_vec_(T)
  \brief Shortcut to access the algebra::vector (vec) type associated
  to T.

  To be used in non-templated functions only.
  \ingroup macrosval
*/
/// Shortcuts to access the algebra::vector (vec) type associated to T.
/// \{
# define mln_vec(T)        typename T::vec
# define mln_vec_(T)                T::vec
/// \}

/*!
  \def mln_vset(T)
  \brief Shortcut to access the type of value set (vset) associated
  to T.

  To be used in templated functions only.
  \ingroup macrosval
  \relates mln::Value
*/
/*!
  \def mln_vset_(T)
  \brief Shortcut to access the type of value set (vset) associated
  to T.

  To be used in non-templated functions only.
  \ingroup macrosval
  \relates mln::Value
*/
/// Shortcuts to access the type of value set (vset) associated to T.
/// \{
# define mln_vset(T)       typename T::t_eligible_values_set
# define mln_vset_(T)               T::t_eligible_values_set
/// \}

/*!
  \def mln_viter(T)
  \brief Shortcut to access the viter type associated to T.

  To be used in templated functions only.
  \ingroup macrositerval
  \relates mln::Value
*/
/*!
  \def mln_viter_(T)
  \brief Shortcut to access the viter type associated to T.

  To be used in non-templated functions only.
  \ingroup macrositerval
  \relates mln::Value
*/
/// Shortcuts to access the viter type associated to T.
/// \{
# define mln_viter(T)      typename T::fwd_viter
# define mln_viter_(T)              T::fwd_viter
/// \}


// w

/*!
  \def mln_weight(T)
  \brief Shortcut to access the weight type associated to T.

  To be used in templated functions only.
  \ingroup macroswin
  \relates mln::Weighted_Window
*/
/*!
  \def mln_weight_(T)
  \brief Shortcut to access the weight type associated to T.

  To be used in non-templated functions only.
  \ingroup macroswin
  \relates mln::Weighted_Window
*/
/// Shortcuts to access the weight type associated to T.
/// \{
# define mln_weight(T)     typename T::weight
# define mln_weight_(T)             T::weight
/// \}

/*!
  \def mln_window(T)
  \brief Shortcut to access the window type associated to T.

  To be used in templated functions only.
  \ingroup macrosnbh
  \relates mln::Neighborhood
*/
/*!
  \def mln_window_(T)
  \brief Shortcut to access the window type associated to T.

  To be used in non-templated functions only.
  \ingroup macrosnbh
  \relates mln::Neighborhood
*/
/// Shortcuts to access the window type associated to T.
/// \{
# define mln_window(T)     typename T::window
# define mln_window_(T)             T::window
/// \}




// FIXME: Just include trait/all.hh !!!

/*!
  \def mln_fwd_pixter(T)
  \brief Shortcut to access the fwd_pixter type associated to T.

  Iterator on image sites, to be used with concrete images only.

  To be used in templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/*!
  \def mln_fwd_pixter_(T)
  \brief Shortcut to access the fwd_pixter type associated to T.

  Iterator on image sites, to be used with concrete images only.

  To be used in non-templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
# define mln_fwd_pixter(I)  typename mln::trait::fwd_pixter< I >::ret
# define mln_fwd_pixter_(I)          mln::trait::fwd_pixter< I >::ret

/*!
  \def mln_bkd_pixter(T)
  \brief Shortcut to access the bkd_pixter type associated to T.

  Iterator on image sites, to be used with concrete images only.

  To be used in templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/*!
  \def mln_bkd_pixter_(T)
  \brief Shortcut to access the bkd_pixter type associated to T.

  Iterator on image sites, to be used with concrete images only.

  To be used in non-templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
# define mln_bkd_pixter(I) typename mln::trait::bkd_pixter< I >::ret
# define mln_bkd_pixter_(I)         mln::trait::bkd_pixter< I >::ret

/*!
  \def mln_pixter(T)
  \brief Shortcut to access the pixter type associated to T.

  Iterator on image sites, to be used with concrete images only.

  To be used in templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
/*!
  \def mln_pixter_(T)
  \brief Shortcut to access the pixter type associated to T.

  Iterator on image sites, to be used with concrete images only.

  To be used in non-templated functions only.
  \ingroup macrositerima
  \relates mln::Image
*/
# define mln_pixter(I)  mln_fwd_pixter(I)
# define mln_pixter_(I) mln_fwd_pixter_(I)

/*!
  \def mln_fwd_qixter(T)
  \brief Shortcut to access the fwd_qixter type associated to T.

  Iterator on window elements, to be used with concrete images only.

  To be used in templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
/*!
  \def mln_fwd_qixter_(T)
  \brief Shortcut to access the fwd_qixter type associated to T.

  Iterator on window elements, to be used with concrete images only.

  To be used in non-templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
# define mln_fwd_qixter(I, W) typename mln::trait::fwd_qixter< I, W >::ret
# define mln_fwd_qixter_(I, W)         mln::trait::fwd_qixter< I, W >::ret

/*!
  \def mln_bkd_qixter(T)
  \brief Shortcut to access the bkd_qixter type associated to T.

  Iterator on window elements, to be used with concrete images only.

  To be used in templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
/*!
  \def mln_bkd_qixter_(T)
  \brief Shortcut to access the bkd_qixter type associated to T.

  Iterator on window elements, to be used with concrete images only.

  To be used in non-templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
# define mln_bkd_qixter(I, W) typename mln::trait::bkd_qixter< I, W >::ret
# define mln_bkd_qixter_(I, W)         mln::trait::bkd_qixter< I, W >::ret


/*!
  \def mln_qixter(T)
  \brief Shortcut to access the qixter type associated to T.

  Iterator on window elements, to be used with concrete images only.

  To be used in templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
/*!
  \def mln_qixter_(T)
  \brief Shortcut to access the qixter type associated to T.

  Iterator on window elements, to be used with concrete images only.

  To be used in non-templated functions only.
  \ingroup macrositerwin
  \relates mln::Window mln::Weighted_Window
*/
# define mln_qixter(I, W)  mln_fwd_qixter(I, W)
# define mln_qixter_(I, W) mln_fwd_qixter_(I, W)

/*!
  \def mln_fwd_nixter(T)
  \brief Shortcut to access the fwd_nixter type associated to T.

  Iterator on neighborhood elements, to be used with concrete images
  only.

  To be used in templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
/*!
  \def mln_fwd_nixter_(T)
  \brief Shortcut to access the fwd_nixter type associated to T.

  Iterator on neighborhood elements, to be used with concrete images
  only.

  To be used in non-templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
# define mln_fwd_nixter(I, N) typename mln::trait::fwd_nixter< I, N >::ret
# define mln_fwd_nixter_(I, N)         mln::trait::fwd_nixter< I, N >::ret

/*!
  \def mln_bkd_nixter(T)
  \brief Shortcut to access the bkd_nixter type associated to T.

  Iterator on neighborhood elements, to be used with concrete images
  only.

  To be used in templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
/*!
  \def mln_bkd_nixter_(T)
  \brief Shortcut to access the bkd_nixter type associated to T.

  Iterator on neighborhood elements, to be used with concrete images
  only.

  To be used in non-templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
# define mln_bkd_nixter(I, N) typename mln::trait::bkd_nixter< I, N >::ret
# define mln_bkd_nixter_(I, N)         mln::trait::bkd_nixter< I, N >::ret

/*!
  \def mln_nixter(T)
  \brief Shortcut to access the nixter type associated to T.

  Iterator on neighborhood elements, to be used with concrete images
  only.

  To be used in templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
/*!
  \def mln_nixter_(T)
  \brief Shortcut to access the nixter type associated to T.

  Iterator on neighborhood elements, to be used with concrete images
  only.

  To be used in non-templated functions only.
  \ingroup macrositernbh
  \relates mln::Neighborhood
*/
# define mln_nixter(I, N)  mln_fwd_nixter(I, N)
# define mln_nixter_(I, N) mln_fwd_nixter_(I, N)

/// Shortcuts to access the vertex iterator type associated to a graph G.
/// \{
# define mln_vertex_iter(G)	    typename G::vertex_iter
# define mln_vertex_iter_(G)		     G::vertex_iter
# define mln_vertex_fwd_iter(G)     typename G::vertex_fwd_iter
# define mln_vertex_fwd_iter_(G)             G::vertex_fwd_iter
# define mln_vertex_bkd_iter(G)     typename G::vertex_bkd_iter
# define mln_vertex_bkd_iter_(G)             G::vertex_bkd_iter
/// \}

/// Shortcuts to access the edge iterator type associated to a graph G.
/// \{
# define mln_edge_iter(G)	  typename G::edge_iter
# define mln_edge_iter_(G)		   G::edge_iter
# define mln_edge_fwd_iter(G)     typename G::edge_fwd_iter
# define mln_edge_fwd_iter_(G)             G::edge_fwd_iter
# define mln_edge_bkd_iter(G)     typename G::edge_bkd_iter
# define mln_edge_bkd_iter_(G)             G::edge_bkd_iter
/// \}

/// Shortcuts to access the vertex centered edge neighbors iterator type
/// associated to a graph G.
/// \{
# define mln_vertex_nbh_vertex_iter(G)	       typename G::vertex_nbh_vertex_iter
# define mln_vertex_nbh_vertex_iter_(G)			G::vertex_nbh_vertex_iter
# define mln_vertex_nbh_vertex_fwd_iter(G)     typename G::vertex_nbh_vertex_fwd_iter
# define mln_vertex_nbh_vertex_fwd_iter_(G)             G::vertex_nbh_vertex_fwd_iter
# define mln_vertex_nbh_vertex_bkd_iter(G)     typename G::vertex_nbh_vertex_bkd_iter
# define mln_vertex_nbh_vertex_bkd_iter_(G)             G::vertex_nbh_vertex_bkd_iter
/// \}

/// Shortcuts to access the vertex centered edge neighbors iterator type
/// associated to a graph G.
/// \{
# define mln_vertex_nbh_edge_iter(G)	     typename G::vertex_nbh_edge_iter
# define mln_vertex_nbh_edge_iter_(G)	              G::vertex_nbh_edge_iter
# define mln_vertex_nbh_edge_fwd_iter(G)     typename G::vertex_nbh_edge_fwd_iter
# define mln_vertex_nbh_edge_fwd_iter_(G)             G::vertex_nbh_edge_fwd_iter
# define mln_vertex_nbh_edge_bkd_iter(G)     typename G::vertex_nbh_edge_bkd_iter
# define mln_vertex_nbh_edge_bkd_iter_(G)             G::vertex_nbh_edge_bkd_iter
/// \}

/// Shortcuts to access the edge centered edge neighbors iterator type
/// associated to a graph G.
/// \{
# define mln_edge_nbh_edge_iter(G)	 typename G::edge_nbh_edge_iter
# define mln_edge_nbh_edge_iter_(G)		  G::edge_nbh_edge_iter
# define mln_edge_nbh_edge_fwd_iter(G)   typename G::edge_nbh_edge_fwd_iter
# define mln_edge_nbh_edge_fwd_iter_(G)           G::edge_nbh_edge_fwd_iter
# define mln_edge_nbh_edge_bkd_iter(G)   typename G::edge_nbh_edge_bkd_iter
# define mln_edge_nbh_edge_bkd_iter_(G)           G::edge_nbh_edge_bkd_iter
/// \}

#endif // ! MLN_CORE_MACROS_HH
