// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_CORE_VALUE_HH
# define NTG_CORE_VALUE_HH

# include <ntg/core/macros.hh>
# include <ntg/core/type.hh>

// --

# include <ntg/core/typetraits.hh>

namespace ntg {

  namespace internal {

    /*---------.
    | any_ntg_ |
    `---------*/
    //! Bridge to internal for ntg type hierarchy.
    /*!
      This class is a bridge with the internal namespace. Indeed,
      global operators (such as <T,U> operator+(T, U)) are defined in
      the internal namespace to allow a "using namespace ntg"
      directive without breaking external classes interactions.

      However, ntg types need to see those operators. This is done by
      inheriting from internal::any_ntg_, thanks to Koenig lookup.
      
      See comments in <ntg/config/system.hh>.
    */

    // FIXME: it is not possible to inherit from mlc::any, because it
    // gives all subtypes an implicit access to the mlc
    // namespace. This is problematic since min or max for example are
    // defined in mlc and ntg::internal namespaces, and it creates
    // conflicts with g++ (3.2, 3.3, 3.4).

    template <class E>
    class any_ntg_ : public ntg::any<E>
    {};

  } // end of internal.

  /*--------.
  | any_ntg |
  `--------*/
  //! Top of the ntg types hierarchy.
  /*!
    Just an abstraction.
  */
  
  template <class E>
  class any_ntg : public internal::any_ntg_<E>
  {};

  /*------.
  | value |
  `------*/
  //! Concrete value storage class.
  /*!
    This class actually store the real value (usually a builtin). The
    value type is determined from the exact type.

    For example, int_u8::val() returns an "unsigned char".
  */

  template <class E>
  class value : public any_ntg<E>
  {
    typedef ntg_storage_type(E) storage_type;

  public:
    // Hooks
    storage_type& val() { return val_; }
    const storage_type& val() const { return val_; }

  protected:
    storage_type val_;
  };

} // namespace ntg

#endif // ndef NTG_CORE_VALUE_HH
