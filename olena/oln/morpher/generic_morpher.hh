// Copyright (C) 2004  EPITA Research and Development Laboratory
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



#ifndef GENERIC_MORPHER_HH
# define GENERIC_MORPHER_HH

# include <string>

# include <oln/basics1d.hh>
# include <oln/basics2d.hh>
# include <oln/basics3d.hh>

namespace oln {

  /*! \namespace oln::morpher
  **
  ** Contain all the morpher relative declarations and functions.
  */

  namespace morpher {

    /*! \namespace oln::morpher::abstract
    **
    ** Implementation of oln::abstract::generic_morpher.
    */

    namespace abstract {

      /*! Traits for conditionnal inheritance used by the \a generic_morpher
      **
      ** It changes the exact type of the image in the input (the exact
      ** type becomes the concrete morpher).
      **
      ** \see oln::morpher::generic_morpher
      */
      template <class T, class Exact>
      struct gm_inherit;

      /// Return \a image1d with an \a exact_type of \a Exact.
      template <class T, class Exact>
      struct gm_inherit<oln::image1d<T>, Exact >
      {
	typedef oln::image1d<T, Exact> ret;
      };

      /// Return \a image2d with an \a exact_type of \a Exact.
      template <class T, class Exact>
      struct gm_inherit<oln::image2d<T>, Exact >
      {
	typedef oln::image2d<T, Exact> ret;
      };

      /// Return \a image3d with an \a exact_type of \a Exact.
      template <class T, class Exact>
      struct gm_inherit<oln::image3d<T>, Exact >
      {
	typedef oln::image3d<T, Exact> ret;
      };

      /*! The Abstract morpher class.
      **
      ** Define a default implementation for all the
      ** dispatched methods of the image hierarchy.
      **
      ** \param DestType Output type of the morpher.
      **
      ** \param SrcType Input type decorated.
      **
      ** \param Exact Exact type
      */

      template <class DestType, class SrcType, class Exact>
      class generic_morpher : public gm_inherit<
	DestType,
	Exact>::ret
      {
      protected:

	/*! \brief  Construct an instance of generic_morpher by assigning
	** \a Ima to Ima_.
	*/
	generic_morpher(const SrcType &Ima) :  super_type(), ima_(Ima) {}

	/// Empty Constructor.
	generic_morpher(): ima_(SrcType()) {}

	/// The decorated image.
	const SrcType	&ima_;

      public:

	typedef generic_morpher<DestType, SrcType, Exact> self_type;
	typedef Exact exact_type;
	typedef oln_point_type(DestType) point_type;
	typedef oln_dpoint_type(DestType) dpoint_type;
	typedef oln_iter_type(DestType) iter_type;
	typedef oln_fwd_iter_type(DestType) fwd_iter_type;
	typedef oln_bkd_iter_type(DestType) bkd_iter_type;
	typedef oln_value_type(DestType) value_type;
	typedef oln_size_type(DestType) size_type;
	typedef oln_impl_type(DestType) impl_type;


	/// Type of the decorated image.
	typedef SrcType src_self_type;
	typedef oln_point_type(SrcType) src_point_type;
	typedef oln_dpoint_type(SrcType) src_dpoint_type;
	typedef oln_iter_type(SrcType) src_iter_type;
	typedef oln_fwd_iter_type(SrcType) src_fwd_iter_type;
	typedef oln_bkd_iter_type(SrcType) src_bkd_iter_type;
	typedef oln_value_type(SrcType) src_value_type;
	typedef oln_size_type(SrcType) src_size_type;
	/// Underlying implementation of the decorated image.
	typedef oln_impl_type(SrcType) src_impl_type;

	/// Exact type of the decorated image.
	typedef oln_exact_type(SrcType) src_exact_type;

	typedef typename gm_inherit<DestType, Exact>::ret super_type;

	/// Return the decorated image.
	const SrcType&
	get_ima() const
	{
	  return ima_;
	}

	/*! Default implementation of at.
	**
	** Return the value stored at \a p in the decorated image.
	*/
	const src_value_type
	at(const src_point_type& p) const
	{
	  return to_exact(ima_).at(p);
	}

	/*! Default implementation of at.
	**
	** Return a reference to the value stored at \a p in the decorated image.
	*/
	src_value_type&
	at(const src_point_type& p)
	{
	  return to_exact(ima_).at(p);
	}

	/// Default implementation of impl.
	const src_impl_type*
	impl() const
	{
	  return to_exact(ima_).impl();
	}

	/// Default implementation of impl.
	src_impl_type*
	impl()
	{
	  return to_exact(ima_).impl();
	}

	/*! Default implementation of clone_.
	**
	** Return a copy of the decorated image.
	*/
	src_self_type
	clone_() const
	{
	  return to_exact(ima_).clone_();
	}

	/*! Default implementation of npoints_.
	**
	** Return the size of the decorated image.
	*/
	size_t
	npoints_()
	{
	  return to_exact(ima_).npoints_();
	}

	/*! Default implementation of assign.
	**
	** Assign \a rhs to the decorated image.
	*/
	src_self_type&
	assign(src_self_type& rhs)
	{
	  return to_exact(ima_).assign(rhs);
	}

	/*! Default implementation of border_set_width.
	**
	** Set the border width of the decorated image to
	** \a min_border.
	*/
	void
	border_set_width(oln::coord min_border,
			 bool copy_border = false) const
	{
	  return to_exact(ima_).border_set_width(min_border, copy_border);
	}

	self_type& operator=(self_type& rhs)
	{
	  return to_exact(*this).assign(rhs.exact());
	}

	static std::string
	name()
	{
	  return "generic_morpher<" + super_type::name() + ">";
	}

      };

    } // end of namespace abstract

  } // end of namespace morpher

} // end of namespace oln


#endif // !GENERIC_MORPHER_HH
