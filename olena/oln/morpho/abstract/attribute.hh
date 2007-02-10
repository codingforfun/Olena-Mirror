// Copyright (C) 2004 EPITA Research and Development Laboratory
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


#ifndef OLN_MORPHO_ABSTRACT_ATTRIBUTE_HH
# define OLN_MORPHO_ABSTRACT_ATTRIBUTE_HH
# include <oln/morpho/environments.hh>

// attribute dedicated macros
# define attr_lambda_type(T)	typename oln::morpho::attr::attr_traits< T >::lambda_type
# define attr_env_type(T)	typename oln::morpho::attr::attr_traits< T >::env_type
# define attr_value_type(T)	typename oln::morpho::attr::attr_traits< T >::value_type

# define attr_lambda_type_(T)	 oln::morpho::attr::attr_traits< T >::lambda_type
# define attr_env_type_(T)	 oln::morpho::attr::attr_traits< T >::env_type
# define attr_value_type_(T)	 oln::morpho::attr::attr_traits< T >::value_type


# define attr_type_decl(self_type) \
      typedef mlc_exact_vt_type(self_type, Exact)	exact_type; \
      typedef attr_value_type(exact_type)		value_type; \
      typedef attr_env_type(exact_type)			env_type; \
      typedef attr_lambda_type(exact_type)		lambda_type

namespace oln
{
  namespace morpho
  {
    namespace attr
    {
      // the traits fwd declaration
      /*!
      ** Traits  for attributes information.
      ** \param T Exact type of the attribute.
      */
      template <class T>
      struct attr_traits;
      //    }

      namespace abstract
      {
	//       namespace attr
	//       {

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
	  void operator+=(const exact_type &rhs);

	  /*!
	  ** \brief >= operator
	  **
	  ** This is a static dispatcher for the >= operator.
	  */
	  bool operator>=(const lambda_type &lambda) const;

	  /*!
	  ** \brief "<" operator
	  **
	  ** This is a static dispatcher for the "<" operator.
	  ** This method is abstract.
	  */
	  bool operator<(const lambda_type &lambda) const;

	  /*!
	  ** \brief "<" operator
	  **
	  ** This is a static dispatcher for the "<" operator.
	  ** This method is abstract.
	  */
	  bool operator<(const exact_type &x) const;

	  /*!
	  ** \brief != operator
	  **
	  ** This is a static dispatcher for the != operator.
	  ** This method is abstract.
	  */
	  bool operator!=(const lambda_type &lambda) const;

	  /*!
	  ** \brief conversion to lambda type.
	  **
	  ** \warning Virtual method.
	  */
	  const lambda_type &toLambda() const;

	  /*!
	  ** \brief >= operator implementation.
	  **
	  ** This is an implementation  of the >= operator.  Override this
	  ** method to provide a new implementation of this operator.
	  ** \warning This method SHOULDN'T be called.
	  */
	  bool ge_impl(const lambda_type &lambda) const;

	  /*!
	  ** \brief "<" operator implementation.
	  **
	  ** This is an implementation  of the < operator.  Override this
	  ** method to provide a new implementation of this operator.
	  ** \warning This method SHOULDN'T be called.
	  */
	  bool less2_impl(const exact_type &x) const;

	protected:
	  attribute();
	};
      } // !abstract
    } // !attr
  } // !morpho
} // !oln

# include <oln/morpho/abstract/attribute.hxx>

#endif // !OLN_MORPHO_ABSTRACT_ATTRIBUTE_HH
