// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef METALIC_BOOL_HH
# define METALIC_BOOL_HH

# include <mlc/value.hh>


namespace mlc
{

  struct Boolean_value
  {
  };

  /// Specializations of mlc::value for T = bool; when v = true, ensure() is provided.

  template <>
  struct value <bool, true> : public Boolean_value
  {
    static const bool b   = true;
    static const bool val = true;
    static void ensure() {}
  protected:
    value() {}
  };

  template <>
  struct value <bool, false> : public Boolean_value
  {
    static const bool b   = false;
    static const bool val = false;
  protected:
    value() {}
  };


  /// Typedefs of true_type and false_type.
  typedef value<bool, true>  true_type;
  typedef value<bool, false> false_type;


  /// Classes is_true<b> and is_false<b> (provided for bkd compability).

  template <bool b> struct is_true {};
  template <> struct is_true <true> { static void ensure() {} };

  template <bool b> struct is_false {};
  template <> struct is_false <false> { static void ensure() {} };



  /// Logical unary not of a Boolean type

  template <typename T>
  struct not_ : public value<bool, !T::b>
  {
    // FIXME: static assert here and below s.a. ~not_() { mlc_is_a(not_<T>, Boolean_value); }
  };

  /// Logical binary operators between a couple of Boolean types

  template <typename L, typename R> struct and_  : public value <bool, (L::b && R::b)> {};
  template <typename L, typename R> struct or_   : public value <bool, (L::b || R::b)> {};
  template <typename L, typename R> struct nand_ : public value <bool, (L::b && !R::b)> {}; 
  template <typename L, typename R> struct xor_  : public value <bool, ((L::b && !R::b) || (!L::b && R::b))> {};

} // end of namespace mlc


#endif // ! METALIC_BOOL_HH
