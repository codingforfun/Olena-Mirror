#ifndef METALIC_CMP_HH
# define METALIC_CMP_HH


# include <mlc/bool.hh>


namespace mlc 
{

  namespace type {

    template <typename T, typename U>
    struct eq : returns_bool_<false> {};

    template <typename T>
    struct eq<T, T> : returns_bool_<true> {};

  } // end of namespace type


} // end of namespace mlc


#endif // ! METALIC_CMP_HH
