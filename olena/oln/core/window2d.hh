#ifndef PROTO_OLN_CORE_WINDOW2D_HH
# define PROTO_OLN_CORE_WINDOW2D_HH

# include <ntg/basics.hh>

namespace oln {

  struct window2d;

  template <>
  struct props <window2d> : public default_props<dim2d>
  {
  };

  struct window2d : public abstract::window< window2d >
  {
    typedef  abstract::window< window2d >  super;
    window2d& add(coord row, coord col)
    {
      // FIXME: below, "this->super::" is required for the call
      // to "add" to be resolved!
      return this->super::add(dpoint2d(row, col));
    }
    image2d<ntg::int_u8> image() const
    {
      coord minrow, maxrow, mincol, maxcol;
      minrow = maxrow = dp(0).row();
      mincol = maxcol = dp(0).col();
      for (unsigned i = 1; i < size(); ++i)
	{
	  if (dp(i).row() < minrow)
	    minrow = dp(i).row();
	  if (dp(i).row() > maxrow)
	    maxrow = dp(i).row();
	  if (dp(i).col() < mincol)
	    mincol = dp(i).col();
	  if (dp(i).col() > maxcol)
	    maxcol = dp(i).col();
	}
      image2d<ntg::int_u8> output(maxrow - minrow + 1,
			     maxcol - mincol + 1);
      for (coord row = 0; row < output.nrows(); ++row)
	for (coord col = 0; col < output.ncols(); ++col)
	  output(row, col) = 0;
      for (unsigned i = 0; i < size(); ++i)
	if (dp(i).row() == 0 and dp(i).col() == 0)
	  output(dp(i).row() - minrow, dp(i).col() - mincol) = 255;
	else
	  output(dp(i).row() - minrow, dp(i).col() - mincol) = 128;
      return output;
    }

  };

  const window2d win_c4()
  {
    return c4().window();
  }

  const window2d win_c4p()
  {
    window2d wc4 = c4().window();
    return wc4.add(0,0);
  }

  const window2d win_c8()
  {
    return c8().window();
  }

  const window2d win_c8p()
  {
    window2d wc8 = c8().window();
    return wc8.add(0,0);
  }

  const window2d disk(int radius)
  {
    assert(radius > 0);
    const unsigned r2 = radius * radius;
    window2d win;
    for (int row = -radius; row <= radius; ++row)
      for (int col = -radius; col <= radius; ++col)
	{
	  unsigned d2 = row * row + col * col;
	  if (d2 <= r2)
	    win.add(row, col);
	}
    return win;
  }

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_WINDOW2D_HH
