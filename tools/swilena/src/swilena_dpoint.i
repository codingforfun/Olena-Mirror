
%define MakeDPoint(Dim)
#if Dim == 1
%{
#include <oln/basics1d.hh>
%}

namespace oln
{

  struct dpoint1d {
    dpoint1d();
    dpoint1d(unsigned);
    unsigned col() const;
    %extend {
      void col(unsigned x) { self->col() = x; }
    };
  };
}
#endif
#if Dim == 2
%{
#include <oln/basics2d.hh>
%}

namespace oln 
{
  struct dpoint2d {
    dpoint2d();
    dpoint2d(unsigned, unsigned);
    unsigned col() const;
    unsigned row() const;
    %extend {
      void col(unsigned x) { self->col() = x; }
      void row(unsigned x) { self->row() = x; }
    };
  };
}

#endif
#if Dim == 3
%{
#include <oln/basics3d.hh>
%}

namespace oln
{
  struct dpoint3d {
    dpoint3d();
    dpoint3d(unsigned, unsigned, unsigned);
    unsigned col() const;
    unsigned row() const;
    unsigned slice() const;
    %extend {
      void col(unsigned x) { self->col() = x; }
      void row(unsigned x) { self->row() = x; }
      void slice(unsigned x) { self->slice() = x; }
    };
  };
  
}
#endif
%enddef
