#ifndef PROTO_OLN_CORE_FWD_NITER1D_HH
# define PROTO_OLN_CORE_FWD_NITER1D_HH



namespace oln {

  struct fwd_niter1d;

  template <>
  struct props <fwd_niter1d> : public default_props<dim1d>
  {
  };

  struct fwd_niter1d : public abstract::niter< fwd_niter1d >
  {
    typedef abstract::niter< fwd_niter1d > super;

    fwd_niter1d() {}

    template <typename D>
    fwd_niter1d(const point1d& center,
		const abstract::struct_elt<D>& dps) :
      super(center, dps)
    {
    }

    template <typename P, typename D>
    fwd_niter1d(const abstract::point_like<P>& center,
		const abstract::struct_elt<D>& dps) :
      super(center, dps)
    {
    }

    // FIXME: for backward compatibility
    template <typename D>
    fwd_niter1d(const abstract::struct_elt<D>& dps,
		const point1d& center) :
      super(center, dps)
    {}
    template <typename P, typename D>
    fwd_niter1d(const abstract::struct_elt<D>& dps,
		const abstract::point_like<P>& center) :
      super(center, dps)
    {}

    const coord_t index() const
    {
      return dps_[i_].index();
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



#endif // ndef PROTO_OLN_CORE_FWD_NITER1D_HH
