#ifndef PROTO_OLN_CORE_FWD_DITER2D_HH
# define PROTO_OLN_CORE_FWD_DITER2D_HH



namespace oln {

  struct fwd_diter2d;

  template <>
  struct props <fwd_diter2d> : public default_props<dim2d>
  {
  };

  struct fwd_diter2d : public abstract::diter< fwd_diter2d >
  {
    typedef abstract::diter< fwd_diter2d > super;

    fwd_diter2d() {}

    template <typename D>
    fwd_diter2d(const abstract::struct_elt<D>& dps) :
      super(dps)
    {
    }

    void begin_impl()
    {
      i_ = 0;
    }

    bool is_at_end_impl() const
    {
      return i_ == dps_.size();
    }

    void next_impl()
    {
      ++i_;
    }
  };

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_FWD_DITER2D_HH
