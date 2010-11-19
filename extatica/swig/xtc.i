							// -*- C++ -*-

/* Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE).

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

/// \file
/// \brief A wrapper of libextatica.

%module xtc

%include std_string.i

%{
#include <cstring>
#include <string>
#include <sstream>

#include "xtc-all.hh"
%}

// Ignore global objects causing trouble to swig.
%ignore xtc::logger;

%include "xtc-all.hh";

/*---------------.
| Construction.  |
`---------------*/

/* The natural, single-argument ctors of xtc::data manipulate the
   encapsulated member by reference, which is wrong when these data
   are of builtin types of the target language (e.g., Pythons'
   `int's), because we have no control on them and they can vanish at
   any moment.
    
   To prevent this, provide constructors helpers creating xtc::data
   objects manipulating data by value (copy).  */
%define generate_data_ctor(Arg, Type)
%extend xtc::data
{
  data(Arg v)
  {
    xtc::proxy_tag* dummy = 0;
    // This dummy pointer passed as second argument is required to
    // call the right ctor.
    return new xtc::data(new xtc::data_proxy_by_cpy< Type >(v), dummy);
  }
}
%enddef // !generate_data_ctor

// Shortcut to generate a ctor taking its argument by value (copy).
%define generate_data_ctor_val(Type)
generate_data_ctor(Type, Type)
%enddef

// Shortcut to generate a ctor taking its argument by (const) reference.
%define generate_data_ctor_ref(Type)
generate_data_ctor(const Type &, Type)
%enddef

// Generate xtc::data ctors using a proxy-by-value for classic types.
generate_data_ctor_val(int)
generate_data_ctor_ref(std::string)

/*-------------.
| Assignment.  |
`-------------*/

%rename(assign) xtc::data::operator=;

/*--------------.
| Conversions.  |
`--------------*/

// Instantiate xtc::data explicit conversion routines for some types.
%extend xtc::data
{
  %template(to_bool) convert_to<bool>;
  %template(to_int) convert_to<int>;
  %template(to_string) convert_to<std::string>;
}

/*------------------.
| Pretty-printing.  |
`------------------*/

// Many Milena objects know how to print themselves with operator<<.
// This is however not very convient in Python, where `str' is usally
// used to produce a string representation of an object.  Fill this
// gap.
%extend xtc::data
{
  char* __str__() const
  {
    std::ostringstream s;
    s << *$self;
    // FIXME: This is admittedly ugly; can't we use std::string as
    // return type?  See Swig's manual.
    return strdup(s.str().c_str());
  }
}
