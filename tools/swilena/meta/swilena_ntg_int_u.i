%module swilena_ntg_int_u
%include swilena_exception.i

%import swilena_ntg.i

%{
#include <ntg/all.hh>
%}

%template(int_u8) ntg::int_u<8, ntg::strict>;
%template(int_u8u) ntg::int_u<8, ntg::unsafe>;
%template(int_u8s) ntg::int_u<8, ntg::saturate>;
%template(int_u16) ntg::int_u<16, ntg::strict>;
%template(int_u16u) ntg::int_u<16, ntg::unsafe>;
%template(int_u16s) ntg::int_u<16, ntg::saturate>;
%template(int_u32) ntg::int_u<32, ntg::strict>;
%template(int_u32u) ntg::int_u<32, ntg::unsafe>;
%template(int_u32s) ntg::int_u<32, ntg::saturate>;

