// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_DEBUG_HH
# define OLENA_VALUE_DEBUG_HH

# include <oln/config/system.hh>
# include <oln/value/predefs.hh>

# include <iostream>
# include <sstream>
# include <string>
using std::string;


namespace oln {


  // predecl
  template <class T> class image;


  namespace internal {


    template<class T>
    struct debug_type
    {
      /* By default, we call the ::name function of type T.  */
      static string name() { return T::name (); }
    };


    // built-in

    template<> struct debug_type<void> 			{ static string name() { return "void"; } };

    template<> struct debug_type<bool> 			{ static string name() { return "bool"; } };

    template<> struct debug_type<unsigned char> 	{ static string name() { return "unsigned char"; } };
    template<> struct debug_type<char> 			{ static string name() { return "char"; } };
    template<> struct debug_type<unsigned short> 	{ static string name() { return "unsigned short"; } };
    template<> struct debug_type<short> 		{ static string name() { return "short"; } };
    template<> struct debug_type<unsigned long> 	{ static string name() { return "unsigned long"; } };
    template<> struct debug_type<long> 			{ static string name() { return "long"; } };

    template<> struct debug_type<unsigned int> 		{ static string name() { return "unsigned int"; } };
    template<> struct debug_type<int> 			{ static string name() { return "int"; } };

    template<> struct debug_type<float> 		{ static string name() { return "float"; } };
    template<> struct debug_type<double> 		{ static string name() { return "double"; } };

  } // end of internal

  //
  //  usage: typename_of_var(obj)
  //
  /////////////////////////////////

  template<class T> inline
  string typename_of_var(const T&) { return internal::debug_type<T>::name(); }


  //
  //  typename_of<T>()
  //
  /////////////////////////////////

  template<class T> inline
  string typename_of() { return internal::debug_type<T>::name(); }

} // end of oln


#endif // ! OLENA_VALUE_DEBUG_HH
