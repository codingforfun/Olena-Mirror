#ifndef PROTO_OLN_CORE_NEIGHBORHOOD2D_HH
# define PROTO_OLN_CORE_NEIGHBORHOOD2D_HH


namespace oln {

  struct neighborhood2d;

  template <>
  struct props <neighborhood2d> : public default_props<dim2d>
  {
  };

  struct neighborhood2d : public abstract::neighborhood< neighborhood2d >
  {
    typedef  abstract::neighborhood< neighborhood2d >  super;
    neighborhood2d& add(coord row, coord col)
    {
      // FIXME: below, "this->super::" is required for the call
      // to "add" to be resolved!
      return this->super::add(dpoint2d(row, col));
    }

  };

  const neighborhood2d c4()
  {
    static bool flower = true;
    static neighborhood2d nbh;
    if (flower)
      {
	nbh.add(-1,0).add(0,-1).add(0,1).add(1,0);
	flower = false;
      }
    return nbh;
  }

  const neighborhood2d c8()
  {
    static bool flower = true;
    static neighborhood2d nbh;
    if (flower)
      {
	nbh = c4();
	nbh.add(-1,-1).add(-1,1).add(1,-1).add(1,1);
	flower = false;
      }
    return nbh;
  }


} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_NEIGHBORHOOD2D_HH
