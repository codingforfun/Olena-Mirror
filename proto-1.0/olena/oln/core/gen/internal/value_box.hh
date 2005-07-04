// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_GEN_INTERNAL_VALUE_BOX_HH
# define OLENA_CORE_GEN_INTERNAL_VALUE_BOX_HH

# include <iostream>
# include <mlc/cmp.hh>
# include <oln/core/abstract/image.hh>
# include <ntg/core/value.hh>
# include <ntg/core/internal/global_ops_traits.hh>


// FIXME: not coherent cause in internal/ but not internal::


/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {



  // fwd decl
  namespace abstract {
    template <typename I> class image;
  }



  /*! \class value_box<I>
  **
  ** This class mimics a value returned by an image via calls like
  ** 'ima[p]'.  It actually represents a box that hides the value from
  ** the client; this box somewhat encloses the value.  This box aims
  ** at controlling whether the value is read or written.  It then
  ** calls the get and set methods provided in abstract::readonly_image
  ** and abstract::readwrite_image.
  **
  ** Parameter I is the exact type of the image from whom the value
  ** comes (the client has thus called abstract::image<I>::operator[]).
  **
  ** When the value comes from a 'const image', the specialization
  ** 'value_box<const I>' is used.
  **
  ** FIXME: a lot more to say...
  */

  template <typename I>
  class value_box
  {

  public:

    /// typedefs

    typedef oln_type_of(I, value) value_type;
    typedef oln_type_of(I, point) point_type;



    /*! \brief op=
    ** FIXME:...
    ** \return (*this)
    */

    template <typename V>
    value_box& operator=(const V& value)
    {
      this->ima_->set(this->p_, value);
      return *this;
    }

    /*! \brief op=
    ** FIXME:...
    **
    ** IDEA: why not add a set method...
    **
    ** \return (*this)
    */
    /// \{
    template <typename II>
    value_box& operator=(const value_box<II>& rhs)
    {
      ima_->set(p_, rhs.value()); // automatic conversion from rhs to value_type
      return *this;
    }

    value_box& operator=(const value_box<I>& rhs)
    {
      ima_->set(p_, rhs.value()); // automatic conversion from rhs to value_type
      return *this;
    }
    /// \}


    /*! \brief Set
    **
    ** NEW code
    **
    ** FIXME:...
    */

//     template <typename A, typename V>
//     value_box& set(void (I::*method)(A), const V& value)
//     {
//       ima_->set(p_, method, value);
//       return *this;
//     }




    /*! \brief Conversion op
    ** FIXME:...
    ** \return converted
    */

    template <typename V>
    operator const V() const
    {
      const V value = ima_->get(p_);
      return value;
    }

    operator const value_type() const
    {
      return ima_->get(p_);
    }


    /*! \brief hook
    ** FIXME:...
    **
    ** IDEA: provide op-> instead (?)
    **
    ** FIXME: keep in mind that there will be pbs with constness knowledge propagation
    ** ex: input[p]->red = 0
    **
    ** \return a value (const, temp)
    */

    const value_type value() const
    {
      return ima_->get(p_);
    }

    template <typename R, typename T>
    R call(R (T::*method)() const) const
    {
      return ima_->call(p_, method);
    }

    template <typename T, typename A, typename V>
    void call(void (T::*method)(A),
	      V arg)
    {
      ima_->call(p_, method, arg);
    }

    /// Cpy ctor.

    value_box(const value_box& rhs) :
      ima_(rhs.ima_),
      p_(rhs.p_)
    {
    }


  private:

    /*-------*
     ! ctors !
     *-------*/

    friend class abstract::image<I>;

    /// Ctor (restricted access).

    value_box(abstract::image<I>& ima, const point_type& p) :
      ima_(&ima.exact()),
      p_(p)
    {
    }

    /*------------*
     ! attributes !
     *------------*/

    I* ima_;
    point_type p_;

  };


  /*! \class value_box<const I>
  **
  ** Specialization of 'value_box<I>'.
  **
  ** FIXME: some more to say...
  **
  ** \see value_box<I>
  */

  template <typename I>
  struct value_box <const I>
  {

  public:

    /// typedefs

    typedef oln_type_of(I, value) value_type;
    typedef oln_type_of(I, point) point_type;


    /*! \brief Assignment (op=) is declared but undefined.
    */

    template <typename V>
    void operator=(const V&) const;


    /*! \brief Conversion op
    ** FIXME:...
    ** \return converted
    */

    template <typename V>
    operator const V() const
    {
      const V value = ima_->get(p_);
      return value;
    }

    operator const value_type() const
    {
      return ima_->get(p_);
    }


    /*! \brief hook
    ** FIXME:...
    **
    ** IDEA: provide op-> instead (?)
    **
    ** FIXME: keep in mind that there will be pbs with constness knowledge propagation
    ** ex: input[p]->red = 0
    **
    ** \return a value (const, temp)
    */

    const value_type value() const
    {
      return ima_->get(p_);
    }

    // IDEA: provide op->

    template <typename R, typename T>
    R call(R (T::*method)() const) const
    {
      return ima_->call(p_, method);
    }


    /// Cpy ctor.

    value_box(const value_box& rhs) :
      ima_(rhs.ima_),
      p_(rhs.p_)
    {
    }


  private:

    /*-------*
     ! ctors !
     *-------*/

    friend class abstract::image<I>;

    /// Ctor (restricted access).

    value_box(const abstract::image<I>& ima, const point_type& p) :
      ima_(&ima.exact()),
      p_(p)
    {
    }

    /*------------*
     ! attributes !
     *------------*/

    const I* ima_;
    point_type p_;

  };


  template <typename I, typename II>
  oln_type_of(I, value) min(const value_box<I>& lhs,
			    const value_box<II>& rhs)
  {
    // FIXME: uncomment?
//     mlc::eq< oln_type_of(I, value), oln_type_of(II, value) >::ensure();
    return lhs.value() < rhs.value() ? lhs.value() : rhs.value();
  }

  template <typename I, typename II>
  oln_type_of(I, value) max(const value_box<I>& lhs,
			    const value_box<II>& rhs)
  {
    // FIXME: uncomment?
//     mlc::eq< oln_type_of(I, value), oln_type_of(II, value) >::ensure();
    return lhs.value() < rhs.value() ? rhs.value() : lhs.value();
  }


} // end of namespace oln



/*-----------------------.
| Comparison operators.  |
`-----------------------*/

// FIXME: Should we equip Integre to support oln::value_box<> values instead?

// ------------------------- //
// value_box and value_box.  //
// ------------------------- //

// oln::value_box<I> OpSymbol oln::value_box<II>
# define oln_decl_comp_binop_vb_vb(OpSymbol)		\
   template <typename I, typename II>			\
   bool							\
   operator OpSymbol (const oln::value_box<I>& lhs,	\
		      const oln::value_box<II>& rhs)	\
   {							\
     return lhs.value() OpSymbol rhs.value();		\
   }


// ------------------------------ //
// value_box and Integre values.  //
// ------------------------------ //

// oln::value_box<I> OpSymbol ntg::value<V>
# define oln_decl_comp_binop_vb_ntgval(OpSymbol)	\
   template <typename I, typename V>			\
   bool							\
   operator OpSymbol (const oln::value_box<I>& lhs,	\
		      const ntg::value<V>& rhs)		\
   {							\
     return lhs.value() OpSymbol rhs.exact ();		\
   }

// ntg::value<V> OpSymbol oln::value_box<I> 
# define oln_decl_comp_binop_ntgval_vb(OpSymbol)	\
   template <typename I, typename V>			\
   bool							\
   operator OpSymbol (const ntg::value<V>& lhs,		\
		      const oln::value_box<I>& rhs)	\
   {							\
     return lhs.exact() OpSymbol rhs.value();		\
   }

# define oln_decl_comp_binop_with_ntgval(OpSymbol)	\
   oln_decl_comp_binop_vb_ntgval(OpSymbol)		\
   oln_decl_comp_binop_ntgval_vb(OpSymbol)


// -------------------------------------------- //
// value_box and C++ builtin arithmetic types.  //
// -------------------------------------------- //

// oln::value_box<I> OpSymbol Builtin
# define oln_decl_comp_binop_vb_builtin(OpSymbol, Builtin)	\
   template <typename I>					\
   bool								\
   operator OpSymbol (const oln::value_box<I>& lhs,		\
		      Builtin rhs)				\
   {								\
     return lhs.value() OpSymbol rhs;				\
   }

// Builtin OpSymbol oln::value_box<I>
# define oln_decl_comp_binop_builtin_vb(OpSymbol, Builtin)	\
   template <typename I>					\
   bool								\
   operator OpSymbol (Builtin lhs,				\
		      const oln::value_box<I>& rhs)		\
   {								\
     return lhs OpSymbol rhs.value();				\
   }

# define oln_decl_comp_binop_with_builtin(OpSymbol, Builtin)	\
   oln_decl_comp_binop_vb_builtin(OpSymbol, Builtin)		\
   oln_decl_comp_binop_builtin_vb(OpSymbol, Builtin)


// ----------- //
// Shortcuts.  //
// ----------- //

# define oln_decl_comp_binop_all(OpSymbol)			\
								\
   oln_decl_comp_binop_vb_vb(OpSymbol)				\
								\
   oln_decl_comp_binop_with_ntgval(OpSymbol)			\
								\
   oln_decl_comp_binop_with_builtin(OpSymbol, signed   long)	\
   oln_decl_comp_binop_with_builtin(OpSymbol, unsigned long)	\
   oln_decl_comp_binop_with_builtin(OpSymbol, signed   int)	\
   oln_decl_comp_binop_with_builtin(OpSymbol, unsigned int)	\
   oln_decl_comp_binop_with_builtin(OpSymbol, signed   short)	\
   oln_decl_comp_binop_with_builtin(OpSymbol, unsigned short)	\
   oln_decl_comp_binop_with_builtin(OpSymbol, char)		\
   oln_decl_comp_binop_with_builtin(OpSymbol, signed   char)	\
   oln_decl_comp_binop_with_builtin(OpSymbol, unsigned char)	\
								\
   oln_decl_comp_binop_with_builtin(OpSymbol, bool)		\
								\
   oln_decl_comp_binop_with_builtin(OpSymbol, float)		\
   oln_decl_comp_binop_with_builtin(OpSymbol, double)


// ------------------------------------ //
// ``Instantiation'' of the operators.  //
// ------------------------------------ //

namespace oln
{

// ``Instantiation'' of the operators.
oln_decl_comp_binop_all(==)
oln_decl_comp_binop_all(!=)
oln_decl_comp_binop_all(<)
oln_decl_comp_binop_all(<=)
oln_decl_comp_binop_all(>)
oln_decl_comp_binop_all(>=)

} // end of namespace oln


# undef oln_decl_comp_binop_vb_vb
# undef oln_decl_comp_binop_vb_ntgval
# undef oln_decl_comp_binop_ntgval_vb
# undef oln_decl_comp_binop_with_ntgval
# undef oln_decl_comp_binop_vb_builtin
# undef oln_decl_comp_binop_builtin_vb
# undef oln_decl_comp_binop_with_builtin
# undef oln_decl_comp_binop



/*-------------------------.
| Arithmetical operators.  |
`-------------------------*/

// FIXME: Should we equip Integre to support oln::value_box<> values instead?

// ------------------------- //
// value_box and value_box.  //
// ------------------------- //

// oln::value_box<I> OpSymbol oln::value_box<II>
# define oln_decl_arith_binop_vb_vb(OpSymbol, OpName)		\
   template <typename I, typename II>				\
   ntg_return_type(OpName,					\
		   typename oln::value_box<I>::value_type,	\
		   typename oln::value_box<II>::value_type)	\
   operator OpSymbol (const oln::value_box<I>& lhs,		\
		      const oln::value_box<II>& rhs)		\
   {								\
     return lhs.value() OpSymbol rhs.value();			\
   }


// ------------------------------ //
// value_box and Integre values.  //
// ------------------------------ //

// oln::value_box<I> OpSymbol ntg::value<V>
# define oln_decl_arith_binop_vb_ntgval(OpSymbol, OpName)	\
   template <typename I, typename V>				\
   ntg_return_type(OpName,					\
		   typename oln::value_box<I>::value_type,	\
		   V)						\
   operator OpSymbol (const oln::value_box<I>& lhs,		\
		      const ntg::value<V>& rhs)			\
   {								\
     return lhs.value() OpSymbol rhs.exact ();			\
   }

// ntg::value<V> OpSymbol oln::value_box<I> 
# define oln_decl_arith_binop_ntgval_vb(OpSymbol, OpName)	\
   template <typename I, typename V>				\
   ntg_return_type(OpName,					\
		   V,						\
		   typename oln::value_box<I>::value_type)	\
   operator OpSymbol (const ntg::value<V>& lhs,			\
		      const oln::value_box<I>& rhs)		\
   {								\
     return lhs.exact() OpSymbol rhs.value();			\
   }

# define oln_decl_arith_binop_with_ntgval(OpSymbol, OpName)	\
   oln_decl_arith_binop_vb_ntgval(OpSymbol, OpName)		\
   oln_decl_arith_binop_ntgval_vb(OpSymbol, OpName)


// -------------------------------------------- //
// value_box and C++ builtin arithmetic types.  //
// -------------------------------------------- //

// oln::value_box<I> OpSymbol Builtin
# define oln_decl_arith_binop_vb_builtin(OpSymbol, OpName, Builtin)	\
   template <typename I>						\
   ntg_return_type(OpName,						\
		   typename oln::value_box<I>::value_type,		\
		   Builtin)						\
   operator OpSymbol (const oln::value_box<I>& lhs,			\
		      Builtin rhs)					\
   {									\
     return lhs.value() OpSymbol rhs;					\
   }

// Builtin OpSymbol oln::value_box<I>
# define oln_decl_arith_binop_builtin_vb(OpSymbol, OpName, Builtin)	\
   template <typename I>						\
   ntg_return_type(OpName,						\
		   Builtin,						\
		   typename oln::value_box<I>::value_type)		\
   operator OpSymbol (Builtin lhs,					\
		      const oln::value_box<I>& rhs)			\
   {									\
     return lhs OpSymbol rhs.value();					\
   }

# define oln_decl_arith_binop_with_builtin(OpSymbol, OpName, Builtin)	\
   oln_decl_arith_binop_vb_builtin(OpSymbol, OpName, Builtin)		\
   oln_decl_arith_binop_builtin_vb(OpSymbol, OpName, Builtin)

# define oln_decl_arith_binop_with_builtin_int(OpSymbol, OpName)	\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, signed   long)	\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, unsigned long)	\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, signed   int)	\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, unsigned int)	\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, signed   short)	\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, unsigned short)	\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, char)		\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, signed   char)	\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, unsigned char)

# define oln_decl_arith_binop_with_builtin_float(OpSymbol, OpName)	\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, float)		\
   oln_decl_arith_binop_with_builtin(OpSymbol, OpName, double)


// ----------- //
// Shortcuts.  //
// ----------- //

# define oln_decl_arith_binop_all_but_builtin_float(OpSymbol, OpName)	\
   oln_decl_arith_binop_vb_vb(OpSymbol, OpName)				\
   oln_decl_arith_binop_with_ntgval(OpSymbol, OpName)			\
   oln_decl_arith_binop_with_builtin_int(OpSymbol, OpName)

# define oln_decl_arith_binop_all(OpSymbol, OpName)			\
   oln_decl_arith_binop_all_but_builtin_float(OpSymbol, OpName)		\
   oln_decl_arith_binop_with_builtin_float(OpSymbol, OpName)


// ------------------------------------ //
// ``Instantiation'' of the operators.  //
// ------------------------------------ //

namespace oln
{
 
oln_decl_arith_binop_all(+, plus)
oln_decl_arith_binop_all(-, minus)
oln_decl_arith_binop_all(*, times)
oln_decl_arith_binop_all(/, div)
oln_decl_arith_binop_all_but_builtin_float(%, mod)

} // end of namespace oln


# undef oln_decl_arith_binop_vb_vb
# undef oln_decl_arith_binop_vb_ntgval
# undef oln_decl_arith_binop_ntgval_vb
# undef oln_decl_arith_binop_with_ntgval
# undef oln_decl_arith_binop_vb_builtin
# undef oln_decl_arith_binop_builtin_vb
# undef oln_decl_arith_binop_with_builtin
# undef oln_decl_arith_binop_with_builtin_int
# undef oln_decl_arith_binop_with_builtin_float
# undef oln_decl_arith_binop_all_but_builtin_float
# undef oln_decl_arith_binop_all



/// Operator <<.
template <typename I>
std::ostream& operator<<(std::ostream& ostr, const oln::value_box<I>& vb)
{
  return ostr << vb.value();
}


#endif // ! OLENA_CORE_GEN_INTERNAL_VALUE_BOX_HH
