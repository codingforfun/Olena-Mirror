// Copyright (C) 2001, 2003, 2004  EPITA Research and Development Laboratory
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
# include <ntg/all.hh>

namespace oln {

  /*! \namespace morpher
  **
  ** Contain all the morpher relative declarations and functions.
  */
  
  namespace morpher {

    /*! \namespace abstract
    **
    ** generic_morpher implementation.
    */
    
    namespace abstract {

      /*! \class gm_inherit
      **
      ** Perform a conditionnal inheritance for the \a generic_morpher
      ** class regarding its template parameters.
      */
      template <class T, class Exact>
      struct gm_inherit;

      /*! \class gm_inherit<oln::image1d<T>, Exact >
      **
      ** Return \a image1d with an \a exact_type of \a Exact.
      */
      template <class T, class Exact>
      struct gm_inherit<oln::image1d<T>, Exact >
      {
	typedef oln::image1d<T, Exact> ret;
      };

      /*! \class gm_inherit<oln::image1d<T>, Exact >
      **
      ** Return \a image2d with an \a exact_type of \a Exact.
      */
      template <class T, class Exact>
      struct gm_inherit<oln::image2d<T>, Exact >
      {
	typedef oln::image2d<T, Exact> ret;
      };

      /*! \class gm_inherit<oln::image1d<T>, Exact >
      **
      ** Return \a image3d with an \a exact_type of \a Exact.
      */
      template <class T, class Exact>
      struct gm_inherit<oln::image3d<T>, Exact >
      {
	typedef oln::image3d<T, Exact> ret;
      };

      /*! \class generic_morpher
      **
      ** An abstract class from whom derive all other
      ** concrete morphers. Define a default implementation
      ** for all the dispatched methods of the image hierarchy.
      */
      
      template <class Inherit, class Deco, class Exact>
      class generic_morpher : public gm_inherit<
	Inherit,
	Exact>::ret
      {
      protected:
	
	/*! \brief  Construct an instance of generic_morpher by assigning
	** \a Ima to Ima_.
	*/
	generic_morpher(const Deco &Ima) :  super_type(), Ima_(Ima) {}
	
	/// Default Constructor.
	generic_morpher(): Ima(Deco()) {}
  
	/// The decorated image.
	const Deco	&Ima_;

      public:
	
	/// Type of the decorated image.
	typedef Deco deco_type;
	typedef oln_point_type(Deco) point_type;
	typedef oln_dpoint_type(Deco) dpoint_type;
	typedef oln_iter_type(Deco) iter_type;
	typedef oln_fwd_iter_type(Deco) fwd_iter_type;
	typedef oln_bkd_iter_type(Deco) bkd_iter_type;
	typedef oln_value_type(Deco) value_type;
	typedef oln_size_type(Deco) size_type;
	
	/// Underlying implementation of the decorated image.
	typedef oln_impl_type(Deco) impl_type;
	
	/// Exact type of the decorated image.
	typedef oln_exact_type(Deco) image_type;
	
	typedef typename gm_inherit<Inherit, Exact>::ret super_type;

	/// Return the decorated image.
	const Deco& 
	get_ima() const
	{
	  return this->Ima_;
	}
    
	/// Return the value stored at \a p in the decorated image.
	const value_type 
	at(const point_type& p) const
	{
	  return this->Ima_.exact().at(p);
	}
  
	/// Return a reference to the value stored at \a p in the decorated image.
	value_type&
	at(const point_type& p)
	{
	  return this->Ima_.exact().at(p);
	}

	/// Return a pointer to the value container of the decorated image.
	const impl_type*
	impl() const
	{
	  return this->Ima_.exact().impl();
	}

	/// Return a pointer to the value container of the decorated image.
	impl_type*
	impl()
	{
	  return this->Ima_.exact().impl();
	}

	/// Return a copy of the decorated image.
	image_type
	clone_() const
	{
	  return this->Ima_.exact().clone_();
	}
	
	/// Return the size of the decorated image.
	size_t 
	npoints_()
	{
	  return this->Ima_.exact().npoints_();
	}
	
	/// Assign \a rhs to the decorated image.
	image_type&
	assign(deco_type& rhs)
	{
	  return this->Ima_.exact().assign(rhs);
	}

	/// Assign the decorated image to \a r
	deco_type& 
	operator=(const oln::io::internal::anything& r)
	{
	  return r.assign(this->Ima_);
	}
  
	/*! \brief Set the border width of the decorated image to
	** \a min_border.
	*/
	void 
	border_set_width(oln::coord min_border,
			 bool copy_border = false) const
	{
	  return this->Ima_.exact().border_set_width(min_border, copy_border);
	}
  
	static std::string name()
	{ return "generic_morpher<" + super_type::name() + ">"; }

  
      };

    } // end of namespace abstract

  } // end of namespace morpher

} // end of namespace oln


#endif // !GENERIC_MORPHER_HH
