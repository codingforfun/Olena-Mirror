#ifndef PROTO_OLN_CORE_WINDOW1D_HH
# define PROTO_OLN_CORE_WINDOW1D_HH


namespace oln {

  struct window1d;

  template <>
  struct props <window1d> : public default_props<dim1d>
  {
  };

  struct window1d : public abstract::window< window1d >
  {
    typedef  abstract::window< window1d >  super;
    window1d& add(coord_t index)
    {
      // FIXME: below, "this->super::" is required for the call
      // to "add" to be resolved!
      return this->super::add(dpoint1d(index));
    }
    image1d<int_u8> image() const
    {
      coord_t minindex, maxindex;
      minindex = maxindex = dp(0).index();
      for (unsigned i = 1; i < size(); ++i)
	{
	  if (dp(i).index() < minindex)
	    minindex = dp(i).index();
	  if (dp(i).index() > maxindex)
	    maxindex = dp(i).index();
	}
      image1d<int_u8> output(maxindex - minindex + 1);
      for (coord_t index = 0; index < output.nindices(); ++index)
	output(index) = 0;
      for (unsigned i = 0; i < size(); ++i)
	if (dp(i).index() == 0)
	  output(dp(i).index() - minindex) = 255;
	else
	  output(dp(i).index() - minindex) = 128;
      return output;
    }

  };

  const window1d win_c2()
  {
    return c2().window();
  }

  const window1d segment(int halflength)
  {
    assert(halflength > 0);
    window1d win;
    for (int index = -halflength; index <= halflength; ++index)
      win.add(index);
    return win;
  }

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_WINDOW1D_HH
