%{
#include <oln/convol/convolution.hh>
#include <oln/convol/fast_gaussian.hh>
%}

%inline %{
struct Convol
{};

%}

%define ConvolFunc1(Name, Impl, R, T1, T2)
%extend Convol {
  static R Name(T1 x, T2 y)
    { return oln::convol:: Impl(x, y); }
};
%enddef


	
%define ExtendFastConvol(Img)
     ConvolFunc1(fast_gaussian, 
		 fast::gaussian, Img, const Img&, oln::float_s);
     ConvolFunc1(fast_gaussian_derivative,
		 fast::gaussian_derivative, Img, const Img&, oln::float_s);
     ConvolFunc1(fast_gaussian_second_derivative,
		 fast::gaussian_second_derivative, Img, const Img&, oln::float_s);
%enddef

%define MakeConvol(Img, W)
%extend Convol {
  static Img convolve(const Img& i, const W& w)
    {
      return oln::apply(oln::convert::force<Img::value >(), 
			oln::convol::slow::convolve(i, w));
    }
}; 
%enddef

%define ExtendConvol(Img, Dim)
     MakeConvol(Img, w_win ## Dim ## d_int );
     MakeConvol(Img, w_win ## Dim ## d_float );
%enddef

