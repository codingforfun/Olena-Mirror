
%define TypedefImage(Dim, Name, ValueType)
%{
  typedef oln::image ## Dim ## d< oln::ValueType > Name;
%}
  typedef oln::image ## Dim ## d< oln::ValueType > Name;
%enddef

%define DimTypedefs(Dim)
TypedefImage(Dim, image ## Dim ## d_u8, int_u8);
TypedefImage(Dim, image ## Dim ## d_s8, int_s8);
TypedefImage(Dim, image ## Dim ## d_u16, int_u16);
TypedefImage(Dim, image ## Dim ## d_s16, int_s16);
TypedefImage(Dim, image ## Dim ## d_bin, bin);
TypedefImage(Dim, image ## Dim ## d_rgb_8, rgb_8);
TypedefImage(Dim, image ## Dim ## d_rgb_16, rgb_16);
TypedefImage(Dim, image ## Dim ## d_sfloat, sfloat);
TypedefImage(Dim, image ## Dim ## d_dfloat, dfloat);
%enddef

DimTypedefs(1);
DimTypedefs(2);
DimTypedefs(3);
