#ifndef INTEGRE_INT_U8_HH
# define INTEGRE_INT_U8_HH


# include <mlc/traits.hh>


namespace ntg {


  struct int_u8
  {
    int_u8() :
      value_(0)
    {
    }

    int_u8(unsigned char value) :
      value_(value)
    {
    }

    int_u8(const int_u8& rhs) :
      value_(rhs.value_)
    {
    }

    int_u8& operator=(const int_u8& rhs)
    {
      this->value_ = rhs.value_;
      return *this;
    }

    operator unsigned char() const
    {
      return value_;
    }

    template <typename V>
    bool operator==(const V& rhs) const
    {
      return this->value_ == rhs;
    }

    template <typename V>
    bool operator!=(const V& rhs) const
    {
      return this->value_ != rhs;
    }

    template <typename V>
    int_u8 operator+(const V& rhs) const
    {
      int_u8 tmp(this->value_ + rhs);
      return tmp;
    }

  private:

    unsigned char value_;    
  };


} // end of namespace ntg



namespace mlc {

  template <>
  struct traits < ntg::int_u8 >
  {
    typedef unsigned char encoding_type;
  };

} // end of namespace mlc



#endif // ! INTEGRE_INT_U8_HH
