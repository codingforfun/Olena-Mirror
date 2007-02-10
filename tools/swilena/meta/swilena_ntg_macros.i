// -*- C++ -*-

// Copyright (C) 2003, 2004  EPITA Research and Development Laboratory
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


// FIXME: avoid code repetitions!

#define ntg_bin ntg::bin
#define ntg_bin_value bool
%inline %{
#define ntg_bin ntg::bin
#define ntg_bin_value bool
%}

#define ntg_int_u32 ntg::int_u< 32, ntg::strict >
#define ntg_int_u32_value unsigned long
%inline %{
#define ntg_int_u32 ntg::int_u< 32, ntg::strict >
#define ntg_int_u32_value unsigned long
%}

#define ntg_int_s32 ntg::int_s< 32, ntg::strict >
#define ntg_int_s32_value long
%inline %{
#define ntg_int_s32 ntg::int_s< 32, ntg::strict >
#define ntg_int_s32_value long
%}

#define ntg_int_u8 ntg::int_u< 8, ntg::strict >
#define ntg_int_u8_value unsigned long
%inline %{
#define ntg_int_u8 ntg::int_u< 8, ntg::strict >
#define ntg_int_u8_value unsigned long
%}

#define ntg_int_s8 ntg::int_s< 8, ntg::strict >
#define ntg_int_s8_value long
%inline %{
#define ntg_int_s8 ntg::int_s< 8, ntg::strict >
#define ntg_int_s8_value long
%}

#define ntg_float ntg::float_d
#define ntg_float_value ntg_float
%inline %{
#define ntg_float ntg::float_d
#define ntg_float_value ntg_float
%}

#define ntg_cplx_rect ntg::cplx< ntg::rect, ntg_float >
#define ntg_cplx_rect_value ntg_cplx_rect
%inline %{
#define ntg_cplx_rect ntg::cplx< ntg::rect, ntg_float >
#define ntg_cplx_rect_value ntg_cplx_rect
%}

#define ntg_cplx_polar ntg::cplx< ntg::polar, ntg_float >
#define ntg_cplx_polar_value ntg_cplx_polar
%inline %{
#define ntg_cplx_polar ntg::cplx< ntg::polar, ntg_float >
#define ntg_cplx_polar_value ntg_cplx_polar
%}
