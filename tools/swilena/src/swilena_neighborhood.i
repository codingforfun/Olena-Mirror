%include swilena_meta_win.i

%define MakeNeigh(Dim)
#if Dim == 1

namespace oln
{
  struct neighborhood1d
  {
    SimpleWindowMethods(1, neighborhood1d);

    neighborhood1d& add(int);
  };

  const neighborhood1d& neighb_c2();
  neighborhood1d mk_neighb_segment(unsigned);
  window1d mk_win_from_neighb(const neighborhood1d&);
}
#endif
#if Dim == 2
namespace oln
{

  struct neighborhood2d
  {
    SimpleWindowMethods(2, neighborhood2d);

    neighborhood2d& add(int, int);
  };

  const neighborhood2d& neighb_c4();
  const neighborhood2d& neighb_c8();
  neighborhood2d mk_neighb_square(unsigned);
  neighborhood2d mk_neighb_rectangle(unsigned, unsigned);
  window2d mk_win_from_neighb(const neighborhood2d&);
}
#endif
#if Dim == 3
namespace oln
{
  struct neighborhood3d
  {
    SimpleWindowMethods(3, neighborhood3d);

    neighborhood3d& add(int, int, int);
  };

  const neighborhood3d& neighb_c6();
  const neighborhood3d& neighb_c18();
  const neighborhood3d& neighb_c26();
  neighborhood3d mk_neighb_block(unsigned, unsigned, unsigned);
  neighborhood3d mk_neighb_cube(unsigned);
  window3d mk_win_from_neighb(const neighborhood3d&);
}
#endif

%enddef
