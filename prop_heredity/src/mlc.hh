#ifndef PROTO_MLC_HH
# define PROTO_MLC_HH

# include <vector>
# include "props.hh"


enum { scalar_tag, binary_tag, vector_tag, label_tag };

struct label_type
{
  int i;
  label_type() : i(0) {}
  label_type(int i) : i(i) {}
  operator int() const { return i; }
  void operator=(int i) { this->i = i; }
};


template <> struct props <unsigned> { enum { tag_value = scalar_tag }; };
template <> struct props <bool> { enum { tag_value = binary_tag }; };
template <> struct props <label_type> { enum { tag_value = label_tag }; };
template <typename T> struct props < std::vector<T> > { enum { tag_value = vector_tag }; };

# define Tag_value(T) props<T>::tag_value


#endif // ndef PROTO_MLC_HH
