
%import swilena.i

/*------------.
| Conversions |
`------------*/

%include swilena_conversions.i

%define SimpleConverter(Dim, To, From)
     SimpleConvert(oln::image##Dim##d<oln:: To >, oln::image##Dim##d<oln:: From >);
%enddef
%define SimpleConverters(Dim, To)
     SimpleConverter(Dim, To, bin);
     SimpleConverter(Dim, To, int_u8);
     SimpleConverter(Dim, To, int_u16);
     SimpleConverter(Dim, To, int_u32);
     SimpleConverter(Dim, To, int_s8);
     SimpleConverter(Dim, To, int_s16);
     SimpleConverter(Dim, To, int_s32);
     SimpleConverter(Dim, To, int_u8s);
     SimpleConverter(Dim, To, int_u16s);
     SimpleConverter(Dim, To, int_u32s);
     SimpleConverter(Dim, To, int_s8s);
     SimpleConverter(Dim, To, int_s16s);
     SimpleConverter(Dim, To, int_s32s);
     SimpleConverter(Dim, To, float_s);
     SimpleConverter(Dim, To, float_d);
%enddef
%define SimpleDimConverters(Dim)
     SimpleConverters(Dim, bin);
     SimpleConverters(Dim, int_u8);
     SimpleConverters(Dim, int_u16);
     SimpleConverters(Dim, int_u32);
     SimpleConverters(Dim, int_s8);
     SimpleConverters(Dim, int_s16);
     SimpleConverters(Dim, int_s32);
     SimpleConverters(Dim, int_u8s);
     SimpleConverters(Dim, int_u16s);
     SimpleConverters(Dim, int_u32s);
     SimpleConverters(Dim, int_s8s);
     SimpleConverters(Dim, int_s16s);
     SimpleConverters(Dim, int_s32s);
     SimpleConverters(Dim, float_s);
     SimpleConverters(Dim, float_d);
%enddef

#if DIMENSION == 1
SimpleDimConverters(1);
#elif DIMENSION == 2
SimpleDimConverters(2);
#elif DIMENSION == 3
SimpleDimConverters(3);
#endif


