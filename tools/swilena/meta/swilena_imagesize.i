// -*- C++ -*-

/*---------------------------.
| oln::image_size structures |
`---------------------------*/

%define imagesize_methods(T, Dim)
     coord nth(unsigned) const;
     
     bool operator==(const T&) const;
     %extend
     {
       bool eq(const T& other) const
	 { return (*self) == other; }
       unsigned dim() const
        { return self->dim; }
       std::string describe() const
	 {
	   std::ostringstream os;
	   os << #T "@" << self << " = (";
	   for (unsigned i = 0; i < self->dim; ++i)
	     {
	       if (i)
		 os << ',';
	       os << self->nth(i);
	     }
	   os << ')';
	   return os.str();
	 }
     }
#if Dim == 1
T(coord, coord);
#elif Dim == 2
T(coord, coord, coord);
#elif Dim == 3
T(coord, coord, coord, coord);
#endif

#if Dim >= 1
coord ncols() const;
#endif
#if Dim >= 2
coord nrows() const;
#endif
#if Dim >= 3
coord nslices() const;
#endif

%enddef

%define decl_imagesize(Dim)

%include swilena_core.i
#if Dim == 1
%{
#include <oln/core/image1d_size.hh>
%}
#elif Dim == 2
%{
#include <oln/core/image2d_size.hh>
%}  
#elif Dim == 3
%{
#include <oln/core/image3d_size.hh>
%}
#endif

namespace oln
{
  struct image ## Dim ## d_size
  {
    imagesize_methods(image ## Dim ## d_size, Dim)
  };
}

%enddef
