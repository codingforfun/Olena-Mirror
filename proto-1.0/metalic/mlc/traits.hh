#ifndef METALIC_TRAITS_HH
# define METALIC_TRAITS_HH



namespace mlc {

  template <typename T>
  struct traits
  {
  };

} // end of namespace mlc


# define mlc_encoding_type(T) typename mlc::traits<T>::encoding_type



#endif // ! METALIC_TRAITS_HH
