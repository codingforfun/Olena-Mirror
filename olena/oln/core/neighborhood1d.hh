#ifndef PROTO_OLN_CORE_NEIGHBORHOOD1D_HH
# define PROTO_OLN_CORE_NEIGHBORHOOD1D_HH


namespace oln {

  struct neighborhood1d;

  template <>
  struct props <neighborhood1d> : public default_props<dim1d>
  {
  };

  struct neighborhood1d : public abstract::neighborhood< neighborhood1d >
  {
    typedef  abstract::neighborhood< neighborhood1d >  super;
    neighborhood1d& add(coord_t index)
    {
      // FIXME: below, "this->super::" is required for the call
      // to "add" to be resolved!
      return this->super::add(dpoint1d(index));
    }
    
  };

  const neighborhood1d c2()
  {
    static bool flower = true;
    static neighborhood1d nbh;
    if (flower)
      {
	nbh.add(-1).add(1);
	flower = false;
      }
    return nbh;
  }


} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_NEIGHBORHOOD1D_HH
