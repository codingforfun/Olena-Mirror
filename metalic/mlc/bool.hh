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

#ifndef METALIC_BOOL_HH
# define METALIC_BOOL_HH

// This file defines helper structures that deal with two kinds of
// booleans.
//
//    - classic boolean values: `true' or `false'
//        These can be checked with is_true and is_false, and
//        used in the `if_' type selector.
//        They usually are the result of a test made in the
//        argument itself.
//          if_<N != 0, some_type, another_type>
//
//    - boolean TYPES: `true_t' or `false_t'
//        As types, it's easier to use them in template, and they
//        can't be confounded with integers like booleans do.

namespace mlc 
{

  struct false_t;

  struct true_t  {
    static void is_true() {}
    typedef false_t not_t;
    typedef void is_true_t;
  };

  struct false_t {
    static void is_false() {}
    typedef true_t not_t;
    typedef void is_false_t;
  };


  template <bool Cond, class if_true_type, class if_false_type>
  struct if_ {
    typedef if_true_type ret_t;
  };

  template<class if_true_type, class if_false_type>
  struct if_<false, if_true_type, if_false_type> 
  {
    typedef if_false_type ret_t;
    typedef false_t ensure_t;
  };

  template<bool> struct is_true;
  template<> struct is_true<true>
  { 
    static void ensure() {};
    typedef true_t ensure_t;
  };

  template<bool> struct is_false;
  template<> struct is_false<false> { static void ensure() {}; };

  // FIXME: find a good name for this struct.

  // base class for meta-types returning Boolean values

  template<bool> struct returns_bool_;

  template<>
  struct returns_bool_<true> : public true_t
  {
    static const bool ret = true;
    static void ensure() {}
  };

  template<>
  struct returns_bool_<false> : public false_t
  {
    static const bool ret = false;
  };

  //
  // FIXME: remove these logical operators.
  //
  // They can easily be replaced by is_true<b1 OP b2>::ensure()
  //
  /////////////////////////////////////////////////////////////

  //
  // or
  //
    
  template<bool b1, bool b2>
  struct logical_or {
    enum { ret = (b1 || b2) };
    static void ensure() {}
  };

  template<>
  struct logical_or<false, false> {
    enum { ret = false };
  };


  //
  // and
  //
    
  template<bool b1, bool b2>
  struct logical_and {
    enum { ret = (b1 && b2) };
  };

  template<>
  struct logical_and<true, true> {
    enum { ret = true };
    static void ensure() {}
  };

} // mlc


#endif // METALIC_BOOL_HH
