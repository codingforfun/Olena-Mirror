#ifndef PROTO_OLN_CORE_MACROS_HH
# define PROTO_OLN_CORE_MACROS_HH


namespace oln {

  template <class E>
  struct props;

} // end of namespace oln



// type aliases

# define oln_dim_type(E) typename oln::props<E>::dim_type
# define oln_image_size_type(E) typename oln::props<E>::image_size_type

# define oln_point_type(E) typename oln::props<E>::point_type
# define oln_dpoint_type(E) typename oln::props<E>::dpoint_type

# define oln_value_type(E) typename oln::props<E>::value_type

# define oln_iter_type(E) typename oln::props<E>::iter_type
# define oln_fwd_iter_type(E) typename oln::props<E>::fwd_iter_type
# define oln_bkd_iter_type(E) typename oln::props<E>::bkd_iter_type

# define oln_neighborhood_type(E) typename oln::props<E>::neighborhood_type
# define oln_window_type(E) typename oln::props<E>::window_type

# define oln_niter_type(E) typename oln::props<E>::niter_type
# define oln_fwd_niter_type(E) typename oln::props<E>::fwd_niter_type

// FIXME: ...



// constant aliases

# define oln_linbuf_value(E) oln::props<E>::linbuf_value
# define oln_dim_value(E) oln::props<oln_dim_type(E)>::dim_value



// others

# define oln_return_type(F) typename oln::props<F>::return_type
# define oln_arg_type(F) typename oln::props<F>::arg_type
# define oln_arg1_type(F) typename oln::props<F>::arg1_type
# define oln_arg2_type(F) typename oln::props<F>::arg2_type

# define oln_concrete_type(I) \
typename oln::props<oln_dim_type(I)>::template concrete_type<oln_value_type(I)>::ret

# define oln_ch_value_type(I, T) typename oln::ch_value<I,T>::ret



// default properties

namespace oln {

  template <class Dim>
  struct default_props
  {
    enum { linbuf_value = oln_linbuf_value(Dim) };
    enum { dim_value = oln_dim_value(Dim) };

    typedef Dim dim_type;
    typedef oln_image_size_type(Dim) image_size_type;

    typedef oln_point_type(Dim) point_type;
    typedef oln_dpoint_type(Dim) dpoint_type;

    typedef oln_iter_type(Dim) iter_type;
    typedef oln_fwd_iter_type(Dim) fwd_iter_type;
    typedef oln_bkd_iter_type(Dim) bkd_iter_type;

    typedef oln_neighborhood_type(Dim) neighborhood_type;
    typedef oln_window_type(Dim) window_type;

    typedef oln_niter_type(Dim) niter_type;
    typedef oln_fwd_niter_type(Dim) fwd_niter_type;
//     typedef oln_bkd_niter_type(Dim) bkd_niter_type;
  };

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_MACROS_HH
