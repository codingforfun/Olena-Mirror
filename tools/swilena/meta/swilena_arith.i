// -*- C++ -*-

// Copyright (C) 2003, 2006  EPITA Research and Development Laboratory
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


%include swilena_config.i

%include swilena_describe.i
%include swilena_core.i

%define decl_arith(Dim)

%include swilena_basics ## Dim ## d.i

%{
#include <oln/arith/ops.hh>
%}

%inline %{

  template<class R, class I1, class I2>
  static R my_plus(const I1& i1, const I2& i2)
  { return ::oln::arith::plus_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_minus(const I1& i1, const I2& i2)
  { return ::oln::arith::minus_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_times(const I1& i1, const I2& i2)
  { return ::oln::arith::times_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_div(const I1& i1, const I2& i2)
  { return ::oln::arith::div_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_min(const I1& i1, const I2& i2)
  { return ::oln::arith::min_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_max(const I1& i1, const I2& i2)
  { return ::oln::arith::max_with_ret<R>(i1, i2); }

%}

%enddef
