#ifndef PROTO_OLN_CORE_FWD_DITER1D_HH
# define PROTO_OLN_CORE_FWD_DITER1D_HH



namespace oln {

  struct fwd_diter1d;

  template <>
  struct props <fwd_diter1d> : public default_props<dim1d>
  {
  };

  struct fwd_diter1d : public abstract::diter< fwd_diter1d >
  {
    typedef abstract::diter< fwd_diter1d > super;

    fwd_diter1d() {}

    template <typename D>
    fwd_diter1d(const abstract::struct_elt<D>& dps) :
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



#endif // ndef PROTO_OLN_CORE_FWD_DITER1D_HH
