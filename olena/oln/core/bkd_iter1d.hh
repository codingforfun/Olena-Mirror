#ifndef PROTO_OLN_CORE_BKD_ITER1D_HH
# define PROTO_OLN_CORE_BKD_ITER1D_HH


namespace oln {


  struct bkd_iter1d;

  template <>
  struct props <bkd_iter1d> : public default_props<dim1d>
  {
  };
  
  struct bkd_iter1d : public abstract::iter< bkd_iter1d >
  {
    bkd_iter1d() {}

    bkd_iter1d(const image1d_size& size) :
      size(size)
    {
    }

    // FIXME: for backward compatibility
    template <typename I>
    bkd_iter1d(const abstract::image<I>& ima) :
      size(ima.size())
    {
    }

    const coord_t index() const
    {
      return p_.index();
    }

    void begin_impl()
    {
      p_.index() = size.nindices() - 1;
    }

    bool is_at_end_impl() const
    {
      return p_.index() == -1;
    }

    void next_impl()
    {
      --p_.index();
    }

  protected:
    image1d_size size;
  };

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_BKD_ITER1D_HH
