#ifndef PROTO_OLN_CORE_CH_VALUE_HH
# define PROTO_OLN_CORE_CH_VALUE_HH


namespace oln {


  // fwd decls

  template <typename T> struct image1d;
  template <typename T> struct image2d;

  template <typename I, typename T>
  struct ch_value;

  template <typename U, typename T>
  struct ch_value < image1d<U>, T >
  {
    typedef image1d<T> ret;
  };

  template <typename U, typename T>
  struct ch_value < image2d<U>, T >
  {
    typedef image2d<T> ret;
  };

  // ...


  // FIXME: for backward compatibility
  template <typename I, typename T>
  struct mute
  {
    typedef typename ch_value<I,T>::ret ret;
  };


} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_CH_VALUE_HH
