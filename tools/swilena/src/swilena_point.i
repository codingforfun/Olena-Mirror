
%define MakePoint(Dim)
#if Dim == 1
namespace oln
{

  struct point1d {
    point1d();
    point1d(unsigned);
    unsigned col() const;
    %extend {
      void col(unsigned x) { self->col() = x; }
      point1d& add(const dpoint1d& other)
	{ return (*self) += other; }
    };
    point1d operator+(const dpoint1d&) const;
    point1d& operator+=(const dpoint1d&);
    
  };
}
#endif
#if Dim == 2
namespace oln
{
  struct point2d {
    point2d();
    point2d(unsigned, unsigned);
    unsigned col() const;
    unsigned row() const;
    %extend {
      void col(unsigned x) { self->col() = x; }
      void row(unsigned x) { self->row() = x; }
      point2d& add(const dpoint2d& other)
	{ return (*self) += other; }
    };
    point2d operator+(const dpoint2d&) const;
    point2d& operator+=(const dpoint2d&);
  };
}
#endif
#if Dim == 3
namespace oln
{
  struct point3d {
    point3d();
    point3d(unsigned, unsigned, unsigned);
    unsigned col() const;
    unsigned row() const;
    unsigned slice() const;
    %extend {
      void col(unsigned x) { self->col() = x; }
      void row(unsigned x) { self->row() = x; }
      void slice(unsigned x) { self->slice() = x; }
      point3d& add(const dpoint3d& other)
	{ return (*self) += other; }
    };
    point3d operator+(const dpoint3d&) const;
    point3d& operator+=(const dpoint3d&);
  };
  
}
#endif
%enddef
