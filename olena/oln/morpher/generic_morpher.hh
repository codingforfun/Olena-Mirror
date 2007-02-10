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



#ifndef OLENA_MORPHER_GENERIC_MORPHER_HH
# define OLENA_MORPHER_GENERIC_MORPHER_HH

# include <string>

# include <oln/basics1d.hh>
# include <oln/basics2d.hh>
# include <oln/basics3d.hh>


namespace oln {

  /// Contain all the morpher relative declarations and functions.

  namespace morpher {

    /// Implementation of oln::abstract::generic_morpher.

    namespace abstract {

      template <class SrcType, class Exact>
      class generic_morpher;

    }
  }

  template <class Exact, class SrcType>
  struct image_traits<morpher::abstract::generic_morpher<SrcType, Exact> >:
    public image_traits<abstract::image_with_impl<typename image_id<Exact>::impl_type,
						  typename image_id<Exact>::exact_type> >
  {

  };

  namespace morpher {

    namespace abstract {

      template <unsigned Dim, class T>
      struct dest_type;

      template <class T>
      struct dest_type<1, T>
      {
	typedef image1d<T> ret;
      };


      template <class T>
      struct dest_type<2, T>
      {
	typedef image2d<T> ret;
      };


      template <class T>
      struct dest_type<3, T>
      {
	typedef image3d<T> ret;
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

      template <class SrcType, class Exact>
      class generic_morpher :
	public oln::abstract::image_with_impl<typename image_id<Exact>::impl_type,
					      Exact>
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

	/// The self type.
	typedef generic_morpher<SrcType, Exact> self_type;

	/// The exact type of the morpher.
	typedef Exact exact_type;
	/// The morpher point type.
	typedef typename image_traits<exact_type>::point_type point_type;
 	/// The morpher dpoint type.
	typedef typename image_traits<exact_type>::dpoint_type dpoint_type;
 	/// The morpher iterator type.
	typedef typename image_traits<exact_type>::iter_type iter_type;
	/// The morpher forward iterator type.
	typedef typename image_traits<exact_type>::fwd_iter_type fwd_iter_type;
	/// The morpher backward iterator type.
	typedef typename image_traits<exact_type>::bkd_iter_type bkd_iter_type;
 	/// The morpher value type.
	typedef typename image_traits<exact_type>::value_type value_type;
	/// The morpher size type.
	typedef typename image_traits<exact_type>::size_type size_type;
 	/// The morpher underlying implementation.
	typedef typename image_traits<exact_type>::impl_type impl_type;

	typedef typename dest_type<image_traits<exact_type>::dim, value_type>::ret DestType;

	/// Type of the decorated image.
	typedef SrcType src_self_type;
	/// The decorated image point type.
	typedef oln_point_type(SrcType) src_point_type;
	/// The decorated image dpoint type.
	typedef oln_dpoint_type(SrcType) src_dpoint_type;
	/// The decorated image iterator type.
	typedef oln_iter_type(SrcType) src_iter_type;
	/// The decorated image forward iterator type.
	typedef oln_fwd_iter_type(SrcType) src_fwd_iter_type;
	/// The decorated image backward iterator type.
	typedef oln_bkd_iter_type(SrcType) src_bkd_iter_type;
	/// The decorated image value type.
	typedef oln_value_type(SrcType) src_value_type;
	/// The decorated image size type.
	typedef oln_size_type(SrcType) src_size_type;
	/// The decorated image underlying implementation.
	typedef oln_impl_type(SrcType) src_impl_type;

	/// Exact type of the decorated image.
	typedef oln_exact_type(SrcType) src_exact_type;

	/// The upper class.
	typedef oln::abstract::image_with_impl<impl_type,
					       exact_type>
	super_type;


	/// Return the decorated image.
	const SrcType&
	get_ima() const
	{
	  return ima_;
	}

	/// Instantiate and return the image that the morpher is simulating.
	DestType
	unmorph() const
	{
	  DestType			res(to_exact(*this).size());
	  oln_iter_type(DestType)	it(res);

	  for_all(it)
	    res[it] = (*this)[it];
	  return res;
	}


	/*! Default implementation of at.
	**
	** Return the value stored at \a p in the decorated image.
	**
	** \warning This method should not be called directly.
	** Prefer operator[].
	*/
	const src_value_type
	at(const src_point_type& p) const
	{
	  return ima_[p];
	}

	/*! Default implementation of at.
	**
	** Return a reference to the value stored at \a p in the decorated image.
	**
	** \warning This method should not be called directly.
	** Prefer operator[].
	*/
	src_value_type&
	at(const src_point_type& p)
	{
	  return ima_[p];
	}

	/// Default implementation of impl.
	const src_impl_type*
	impl() const
	{
	  return ima_.impl();
	}

	/// Default implementation of impl.
	src_impl_type*
	impl()
	{
	  return ima_.impl();
	}

	/*! Default implementation of clone_.
	**
	** Return a copy of the decorated image.
	*/
	src_exact_type
	clone_() const
	{
	  return to_exact(ima_).clone();
	}

	/*! Default implementation of npoints_.
	**
	** Return the size of the decorated image.
	*/
	size_t
	npoints_()
	{
	  return to_exact(ima_).npoints();
	}

	/*! Default implementation of assign.
	**
	** Assign \a rhs to the decorated image.
	*/
	src_exact_type&
	assign(src_exact_type& rhs)
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

	/*! Default implementation of the = operator.
	**
	** Assign the decorated image to 'a r.
	*/
	self_type&
	operator=(self_type& rhs)
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


#endif // !OLENA_MORPHER_GENERIC_MORPHER_HH
