#ifndef PROTO_NTG_BASICS_HH
# define PROTO_NTG_BASICS_HH

# include <vector>
# include <values.h>

# include <ntg/core/predecls.hh>
# include <ntg/real/int_u.hh>
# include <ntg/real/int_s.hh>


namespace ntg {

  enum { scalar_tag, binary_tag, vector_tag, label_tag, unknown_tag };

//   typedef unsigned char int_u8;
//   typedef bool bin;

  struct label_type
  {
    int i;
    label_type() : i(0) {}
    label_type(int i) : i(i) {}
    operator int() const { return i; }
    void operator=(int i) { this->i = i; }
  };

  template <typename E>
  struct props
  {
    enum { tag_value = ntg::unknown_tag };
  };


  // built-in types

  template <> struct props <float> {
    enum { tag_value = ntg::scalar_tag };
  };
  template <> struct props <unsigned  int> {
    enum { tag_value = ntg::scalar_tag };
  };
  template <> struct props <unsigned char> {
    enum { tag_value = ntg::scalar_tag };
  };
  template <> struct props <bool> {
    enum { tag_value = ntg::binary_tag };
  };
  template <typename T> struct props < std::vector<T> > {
    enum { tag_value = ntg::vector_tag };
  };

  // ntg types

//   template <> struct props <ntg::int_u8> { enum { tag_value = ntg::scalar_tag }; };
//   template <> struct props <ntg::bin> { enum { tag_value = ntg::binary_tag }; };
  template <> struct props <ntg::label_type> { enum { tag_value = ntg::label_tag }; };

  // Already in macros.hh (ntg::typetraits)
//   // min

//   template <typename T>
//   struct min;

//   template<> struct min<unsigned int> { static const unsigned int value() { return 0; } };
//   template<> struct min<int_u8> { static const int_u8 value() { return 0; } };
//   template<> struct min<bool> { static const bool value() { return false; } };
//   template<> struct min<float> { static const bool value() { return MINFLOAT; } };

//   // max

//   template <typename T>
//   struct max;

//   template<> struct max<unsigned int> { static const unsigned int value() { return MAXINT; } };
//   template<> struct max<int_u8> { static const int_u8 value() { return 255; } };
//   template<> struct max<bool> { static const bool value() { return true; } };
//   template<> struct max<float> { static const bool value() { return MAXFLOAT; } };

} // end of namespace ntg


# define ntg_tag_value(T) ntg::props<T>::tag_value
// # define ntg_min_value(T) ntg::min<T>::value()
// # define ntg_max_value(T) ntg::max<T>::value()


#endif // ndef PROTO_NTG_BASICS_HH
