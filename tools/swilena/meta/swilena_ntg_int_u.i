%module swilena_ntg_int_u
%include swilena_exception.i

%import swilena_ntg.i

%{
#include <ntg/all.hh>
%}

%define declare_int_u(Name, nbits, behavior)

%template(Name) ntg::int_u<nbits, behavior>;

/*
  This macro should be used in all modules
  Using ntg::int_u8 directly leads to type matching problems:

  - it is a typedef never used in module swilena_ntg_int_u 
    => no type  correspondence is created between ntg::int_u8 
       and ntg::int_u<8, ntg::strict>.

  - if another module use ntg::int_u8, operations on it won't be
    possible since this module won't recognize the type ntg::int_u8.

  - ntg::int_u<8, ntg::strict> or ntg_int_u8 should be used
    everywhere. ntg_int_u8 is just a convenient macro.
*/

#define ntg_ ## Name ntg::int_u<nbits, behavior>

%enddef

declare_int_u(int_u8, 8, ntg::strict);
declare_int_u(int_u8u, 8, ntg::unsafe);
declare_int_u(int_u8s, 8, ntg::saturate);
declare_int_u(int_u16, 16, ntg::strict);
declare_int_u(int_u16u, 16, ntg::unsafe);
declare_int_u(int_u16s, 16, ntg::saturate);
declare_int_u(int_u32, 32, ntg::strict);
declare_int_u(int_u32u, 32, ntg::unsafe);
declare_int_u(int_u32s, 32, ntg::saturate);
