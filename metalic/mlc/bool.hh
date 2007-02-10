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
//    - boolean TYPES: `true_type' or `false_type'
//        As types, it's easier to use them in template, and they
//        can't be confounded with integers like booleans do.

namespace mlc 
{  

  struct false_type;

  struct true_type  {
    static void is_true() {}
    typedef false_type not_type;
    typedef void is_true_type;
  };

  struct false_type {
    static void is_false() {}
    typedef true_type not_type;
    typedef void is_false_type;
  };


  /*----.
  | if_ |
  `----*/

  template <bool Cond, class if_true_type, class if_false_type>
  struct if_ {
    typedef if_true_type ret;
  };

  template<class if_true_type, class if_false_type>
  struct if_<false, if_true_type, if_false_type> 
  {
    typedef if_false_type ret;
    typedef false_type ensure_type;
  };

  /*--------.
  | switch_ |
  `--------*/

  struct invalid {};
  
  template<unsigned Cond, class Ret, class Cases = invalid>
  struct case_ {};

  template<unsigned Cond, class Cases, class Default = invalid>
  struct switch_;

  template<unsigned Cond, unsigned Compare, class Ret, class Default>
  struct switch_<Cond, case_<Compare, Ret>, Default >
  {
    typedef typename if_< (Cond == Compare), Ret, Default >::ret ret; 
  };

  template<unsigned Cond, 
	   unsigned Compare, 
	   class Ret, 
	   class Cases, 
	   class Default>
  struct switch_<Cond, case_<Compare, Ret, Cases>, Default > 
  {
    typedef typename 
      if_< (Cond == Compare), 
	   Ret, 
	   typename switch_<Cond, Cases, Default>::ret>::ret ret;
  };
  
  template<bool Cond, class Ret, class Cases = invalid>
  struct bool_case_ {};

  template<class Cases, class Default = invalid>
  struct bool_switch_;

  template<bool Cond, class Ret, class Default>
  struct bool_switch_<bool_case_<Cond, Ret>, Default > 
  {
    typedef typename if_< Cond, Ret, Default >::ret ret; 
  };

  template<bool Cond,class Ret, class Cases, class Default>
  struct bool_switch_<bool_case_<Cond, Ret, Cases>, Default > 
  {
    typedef typename 
      if_< Cond, 
	   Ret, 
	   typename bool_switch_<Cases, Default>::ret >::ret ret; 
  };


  /*-----.
  | misc |
  `-----*/

  template<bool> struct is_true;
  template<> struct is_true<true>
  { 
    static void ensure() {};
    typedef true_type ensure_type;
  };

  template<bool> struct is_false;
  template<> struct is_false<false> { static void ensure() {}; };

  // FIXME: find a good name for this struct.

  // base class for meta-types returning Boolean values

  template<bool> struct returns_bool_;

  template<>
  struct returns_bool_<true> : public true_type
  {
    typedef void ensure_type;
    static const bool ret = true;
    static void ensure() {}
  };

  template<>
  struct returns_bool_<false> : public false_type
  {
    static const bool ret = false;
  };

} // end of namespace mlc


#endif // METALIC_BOOL_HH
