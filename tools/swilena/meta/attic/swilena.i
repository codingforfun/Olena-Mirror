// -*- C++ -*-

%module swilena

%include swilena_exceptions.i

/*------.
| Types |
`------*/

%include swilena_types.i

MakeValue(int_u8);
MakeValue(int_s8);
MakeValue(int_u16);
MakeValue(int_s16);
MakeValue(int_u32);
MakeValue(int_s32);
MakeValue(int_u8s);
MakeValue(int_s8s);
MakeValue(int_u16s);
MakeValue(int_s16s);
MakeValue(int_u32s);
MakeValue(int_s32s);

MakeFloatValue(float_s);
MakeFloatValue(float_d);

MakeRgbValue(rgb_8);
MakeRgbValue(rgb_16);
MakeRgbValue(rgb_32);







