// Copyright 2001  EPITA Research and Development Laboratory
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

// Jul 28
#ifndef OLENA_VALUE_TO_OLN_HH
# define OLENA_VALUE_TO_OLN_HH

# include "value/predefs.hh"


namespace oln {

  namespace internal {


    template<class T> struct to_oln { typedef T ret; };


    template<class Self> struct to_oln<rec_value<Self> >   { typedef Self ret; };

    // C built-in type  ->  oln type

    template<> struct to_oln<bool>           { typedef bin ret; };

    template<> struct to_oln<unsigned char>  { typedef int_u8  ret; };
    template<> struct to_oln<  signed char>  { typedef int_s8  ret; };
    template<> struct to_oln<unsigned short> { typedef int_u16 ret; };
    template<> struct to_oln<  signed short> { typedef int_s16 ret; };
    template<> struct to_oln<unsigned long>  { typedef int_u32 ret; };
    template<> struct to_oln<  signed long>  { typedef int_s32 ret; };

    template<> struct to_oln<int>	     { typedef int_s8  ret; }; // FIXME : good type
    template<> struct to_oln<float>          { typedef float_p1 ret; };
    template<> struct to_oln<double>         { typedef float_p2 ret; };


  } // end of internal

} // end of oln


#endif // ! OLENA_VALUE_TO_OLN_HH
