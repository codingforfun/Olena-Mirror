%include swilena_meta_win.i

%define MakeWindow(Dim)
#if Dim == 1

namespace oln
{
  struct window1d
  {
    SimpleWindowMethods(1, window1d);

    window1d& add(int);
  };

  const window1d& win_c2_only();
  const window1d& win_c2p();
  window1d mk_win_segment(unsigned);
}
#endif
#if Dim == 2
namespace oln
{
  struct window2d
  {
    SimpleWindowMethods(2, window2d);

    window2d& add(int, int);
  };

  const window2d& win_c4_only();
  const window2d& win_c4p();
  const window2d& win_c8_only();
  const window2d& win_c8p();
  window2d mk_win_rectangle(unsigned, unsigned);
  window2d mk_win_ellipse(float, float);
  window2d mk_win_square(unsigned);
  window2d mk_win_disc(float);
}
#endif
#if Dim == 3
namespace oln
{
  struct window3d
  {

    SimpleWindowMethods(3, window3d);

    window3d& add(int, int, int);
  };

  const window3d& win_c6_only();
  const window3d& win_c6p();
  const window3d& win_c18_only();
  const window3d& win_c18p();
  const window3d& win_c26_only();
  const window3d& win_c26p();
  window3d mk_win_block(unsigned, unsigned, unsigned);
  window3d mk_win_ellipsoid(float, float, float);
  window3d mk_win_cube(unsigned);
  window3d mk_win_ball(float);
}
#endif

%enddef
