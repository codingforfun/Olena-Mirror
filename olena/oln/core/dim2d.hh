#ifndef PROTO_OLN_CORE_DIM2D_HH
# define PROTO_OLN_CORE_DIM2D_HH

# include <oln/core/abstract/dim.hh>

namespace oln {

  // fwd decls:

  template <typename T> struct image2d;
  struct image2d_size;
  struct point2d;
  struct dpoint2d;
  struct fwd_iter2d;
  struct bkd_iter2d;
  struct neighborhood2d;
  struct window2d;
  struct fwd_niter2d;
  // FIXME: to be continued


  struct dim2d;

  template <typename E>
  struct props;

  template <>
  struct props <dim2d>
  {
    template <typename T>
    struct concrete_type {
      typedef image2d<T> ret;
    };

    enum { linbuf_value = false };

    enum { dim_value = 2 };
    typedef dim2d dim_type;

    typedef image2d_size image_size_type;

    typedef point2d point_type;
    typedef dpoint2d dpoint_type;

    typedef fwd_iter2d iter_type;
    typedef fwd_iter2d fwd_iter_type;
    typedef bkd_iter2d bkd_iter_type;

    typedef neighborhood2d neighborhood_type;
    typedef window2d window_type;

    typedef fwd_niter2d niter_type;
    typedef fwd_niter2d fwd_niter_type;
//     typedef bkd_niter2d bkd_niter_type;

    // FIXME: to be continued
  };

  struct dim2d : public abstract::dim< dim2d >
  {
  };

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_DIM2D_HH
