// -*- C++ -*-
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
