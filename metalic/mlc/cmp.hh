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

#ifndef METALIC_CMP_HH
# define METALIC_CMP_HH

# include <mlc/bool.hh>

namespace mlc 
{

  /*-----------------.
  | Type comparisons |
  `-----------------*/

  template <class T, class U>
  struct type_eq : returns_bool_<false> {};

  template <class T>
  struct type_eq<T, T> : returns_bool_<true> {};


  /*-------------------.
  | Values comparisons |
  `-------------------*/

  // These struct are quite handy since constructions like
  // is_true<a < b>::ensure() cannot be parsed.

  template<int i, int j>
  struct less
  {
    enum { ret = (i < j) };
    static void ensure() { is_true<ret>::ensure(); };
  };

  template<int i, int j>
  struct lesseq
  {
    enum { ret = (i <= j) };
    static void ensure() { is_true<ret>::ensure(); };
  };

  template<int i, int j>
  struct eq
  {
    enum { ret = (i == j) };
    static void ensure() { is_true<ret>::ensure(); };
  };

  template<int i, int j>
  struct neq
  {
    enum { ret = (i != j) };
    static void ensure() { is_true<ret>::ensure(); };
  };

  template<int i, int j>
  struct greater
  {
    enum { ret = (i > j) };
    static void ensure() { is_true<ret>::ensure(); };
  };

  template<int i, int j>
  struct greatereq
  {
    enum { ret = (i >= j) };
    static void ensure() { is_true<ret>::ensure(); };
  };

  template<int i, int j>
  struct min
  {
    enum { ret = (i < j ? i : j) };
  };

  template<int i, int j>
  struct max
  {
    enum { ret = (i > j ? i : j) };
  };

  template<int i, int j, int N>
  struct maxN
  {
    enum { ret = (i > j ? 
		  (i > N ? N : i) : 
		  (j > N ? N : j)) };
  };

  template<int i, int N>
  struct saturateN
  {
    enum { ret = (i > N ? N : i) };
  };

} // end of namespace mlc

#endif // ! METALIC_CMP_HH
