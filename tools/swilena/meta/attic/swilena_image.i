%{
#include <oln/basics1d.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/utils/copy.hh>
%}

%define ExtendIO
%extend {
  bool load(const char *name)
    { return oln::load(*self, name); }
  bool save(const char *name) const
    { return oln::save(*self, name); }
};
%enddef

namespace oln
{  
  template<typename T>
    struct image1d 
    {
      image1d();
      image1d(unsigned ncols, unsigned border = 2);
      
      unsigned ncols() const;
      unsigned border() const;
      ExtendIO;
      
      %extend {
	T& at(const point1d &p) { return (*self)[p]; }
	T& at(unsigned x) { return (*self)(x); }
      };
    };
  
  template<typename T>
    struct image2d 
    {
      image2d();
      image2d(unsigned ncols, unsigned nrows, unsigned border = 2);
      
      unsigned ncols() const;
      unsigned nrows() const;
      unsigned border() const;
      ExtendIO;
      
      %extend { 
	T& at(const point2d &p) { return (*self)[p]; }
	T& at(unsigned x, unsigned y) { return (*self)(x, y); }
      };
    };
  
  template<typename T>
    struct image3d 
    {
      image3d();
      image3d(unsigned nslices, unsigned ncols, unsigned nrows, unsigned border = 2);
      
      unsigned ncols() const;
      unsigned nrows() const;
      unsigned nslices() const;
      unsigned border() const;
      ExtendIO;
      
      %extend { 
	T& at(const point3d &p) { return (*self)[p]; }
	T& at(unsigned x, unsigned y, unsigned z) { return (*self)(x, y, z); }
      };
    };
  
}

%define MakeImage(Name, Dim, ValueType)
%{
  typedef oln::image ## Dim ## d<oln:: ValueType > Name;
%}
typedef oln::image ## Dim ## d<oln:: ValueType > Name;

%template(Name) oln::image ## Dim ## d<oln:: ValueType >;
%enddef



