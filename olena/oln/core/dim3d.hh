#ifndef PROTO_OLN_CORE_DIM3D_HH
# define PROTO_OLN_CORE_DIM3D_HH

# include <oln/core/abstract/dim.hh>

namespace oln {

  // fwd decls:

  template <typename T> struct image3d;
  struct image3d_size;
  struct point3d;
  struct dpoint3d;
  struct fwd_iter3d;
  struct bkd_iter3d;
  struct neighborhood3d;
  struct window3d;
  struct fwd_niter3d;
  // FIXME: to be continued


  struct dim3d;

  template <typename E>
  struct props;

  template <>
  struct props <dim3d>
  {
    template <typename T>
    struct concrete_type {
      typedef image3d<T> ret;
    };

    enum { linbuf_value = false };

    enum { dim_value = 3 };
    typedef dim3d dim_type;

    typedef image3d_size image_size_type;

    typedef point3d point_type;
    typedef dpoint3d dpoint_type;

    typedef fwd_iter3d iter_type;
    typedef fwd_iter3d fwd_iter_type;
    typedef bkd_iter3d bkd_iter_type;

    typedef neighborhood3d neighborhood_type;
    typedef window3d window_type;

    typedef fwd_niter3d niter_type;
    typedef fwd_niter3d fwd_niter_type;
//     typedef bkd_niter3d bkd_niter_type;

    // FIXME: to be continued
  };

  struct dim3d : public abstract::dim< dim3d >
  {
  };

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_DIM3D_HH
