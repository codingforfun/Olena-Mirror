#ifndef PROTO_OLN_CORE_FWD_NITER2D_HH
# define PROTO_OLN_CORE_FWD_NITER2D_HH



namespace oln {

  struct fwd_niter2d;

  template <>
  struct props <fwd_niter2d> : public default_props<dim2d>
  {
  };

  struct fwd_niter2d : public abstract::niter< fwd_niter2d >
  {
    typedef abstract::niter< fwd_niter2d > super;

    fwd_niter2d() {}

    template <typename D>
    fwd_niter2d(const point2d& center,
		const abstract::struct_elt<D>& dps) :
      super(center, dps)
    {
    }

    template <typename P, typename D>
    fwd_niter2d(const abstract::point_like<P>& center,
		const abstract::struct_elt<D>& dps) :
      super(center, dps)
    {
    }

    // FIXME: for backward compatibility
    template <typename D>
    fwd_niter2d(const abstract::struct_elt<D>& dps,
		const point2d& center) :
      super(center, dps)
    {}
    template <typename P, typename D>
    fwd_niter2d(const abstract::struct_elt<D>& dps,
		const abstract::point_like<P>& center) :
      super(center, dps)
    {}

    const coord row() const
    {
      return dps_[i_].row();
    }

    const coord col() const
    {
      return dps_[i_].row();
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



#endif // ndef PROTO_OLN_CORE_FWD_NITER2D_HH
