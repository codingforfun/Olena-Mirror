// -*- C++ -*-

/*----------------------.
| oln::image structures |
`----------------------*/


%define image_methods(T, Val, Dim)

     // default constructor
     T();
     T(const image ## Dim ## d_size&, coord border = 2);
     T(const T& other);

     // operators
     T clone() const;

     void clear();

     image ## Dim ## d_size size() const;
     size_t npoints() const;
     coord border() const;

     bool hold(const point ## Dim ## d&) const;

     %extend {
       Val at(const point ## Dim ## d& p) 
	 { return (*self)[p]; }
       T& set(const point ## Dim ## d& p, Val v) 
	 { (*self)[p] = v; return *self; }

       Val& ref(const point ## Dim ## d& p)
	 { return (*self)[p]; }
     }
     
     EXTEND_DESCRIBE(T)

#if Dim == 1
     T(coord, coord border = 2);

     Val& operator()(coord);
     %extend {
       Val at(coord x) { return (*self)(x); }
       T& set(coord x, Val v) { (*self)(x) = v; return *self; }
       Val& ref(coord x) { return (*self)(x); }
     }

#elif Dim == 2
     T(coord, coord, coord border = 2);

     Val& operator()(coord, coord);
     %extend {
       Val at(coord x, coord y)
	 { return (*self)(x, y); }
       T& set(coord x, coord y, Val v) 
	 { (*self)(x, y) = v; return *self; }
       Val& ref(coord x, coord y) 
	 { return (*self)(x, y); }
     }

#elif Dim == 3
     T(coord, coord, coord, coord border = 3);

     Val& operator()(coord, coord, coord);
     %extend {
       Val at(coord x, coord y, coord z)
	 { return (*self)(x, y, z); }
       T& set(coord x, coord y, coord z, Val v) 
	 { (*self)(x, y, z) = v; return *self; }
       Val& ref(coord x, coord y, coord z) 
	 { return (*self)(x, y, z); }
     }
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

// border methods

%extend
{
  void border_set_width(coord new_border, bool copy_border = false)
    { oln::border::set_width(*self, new_border, copy_border); }
  void border_adapt_width(coord new_border, bool copy_border = false)
    { oln::border::adapt_width(*self, new_border, copy_border); }
  void border_adapt_copy(coord min_border)
    { oln::border::adapt_copy(*self, min_border); }
  void border_adapt_mirror(coord min_border)
    { oln::border::adapt_mirror(*self, min_border); }
  void border_adapt_assign(coord min_border, Val v)
    { oln::border::adapt_assign(*self, min_border, v); }
}

// image I/O

%extend
{
  bool load(const char *name)
    { return oln::load(*self, name); }
  bool save(const char *name) const
    { return oln::save(*self, name); }
}
%enddef

%define decl_image(Dim)

%include swilena_describe.i
%include swilena_core.i

%import swilena_imagesize ## Dim ## d.i
%import swilena_point ## Dim ## d.i

#if Dim == 1
%{
#include <oln/core/image1d.hh>
#include <oln/core/border1d.hh>
%}
#elif Dim == 2
%{
#include <oln/core/image2d.hh>
#include <oln/core/border2d.hh>
%}  
#elif Dim == 3
%{
#include <oln/core/image3d.hh>
#include <oln/core/border3d.hh>
%}
#endif

%{
#include <oln/io/basics.hh>
#include <oln/io/pnm.hh>
%}

namespace oln
{
  template<typename T>
  struct image ## Dim ## d
  {
    image_methods(image ## Dim ## d<T>, T, Dim)
  };
}

%enddef

%define make_image(name, Dim, T)
%template(name) oln::image ## Dim ## d<T >;
%enddef

