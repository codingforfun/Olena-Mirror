#ifndef PROTO_OLN_CORE_DIM1D_HH
# define PROTO_OLN_CORE_DIM1D_HH


namespace oln {

  // fwd decls:

  template <typename T> struct image1d;  
  struct image1d_size;
  struct point1d;
  struct dpoint1d;
  struct fwd_iter1d;
  struct bkd_iter1d;
  struct neighborhood1d;
  struct window1d;
  struct fwd_niter1d;
  // FIXME: to be continued


  struct dim1d;

  template <>
  struct props <dim1d>
  {
    template <typename T>
    struct concrete_type {
      typedef image1d<T> ret;
    };

    enum { linbuf_value = false };

    enum { dim_value = 2 };
    typedef dim1d dim_type;

    typedef image1d_size image_size_type;

    typedef point1d point_type;
    typedef dpoint1d dpoint_type;

    typedef fwd_iter1d iter_type;
    typedef fwd_iter1d fwd_iter_type;
    typedef bkd_iter1d bkd_iter_type;

    typedef neighborhood1d neighborhood_type;
    typedef window1d window_type;

    typedef fwd_niter1d niter_type;
    typedef fwd_niter1d fwd_niter_type;
//     typedef bkd_niter1d bkd_niter_type;

    // FIXME: to be continued
  };

  struct dim1d : public abstract::dim< dim1d >
  {
  };

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_DIM1D_HH
