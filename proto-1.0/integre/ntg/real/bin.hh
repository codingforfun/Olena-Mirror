#ifndef INTEGRE_REAL_BIN_HH
# define INTEGRE_REAL_BIN_HH


# include <mlc/traits.hh>


namespace ntg {


  struct bin
  {
    bin() :
      value_(0)
    {
    }

    bin(unsigned char value) :
      value_(value)
    {
    }

    bin(const bin& rhs) :
      value_(rhs)
    {
    }

    bin& operator=(const bin& rhs)
    {
      this->value_ = rhs;
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
    bin operator+(const V& rhs) const
    {
      bin tmp((this->value_ + rhs) % 2);
      return tmp;
    }

  private:

    unsigned char value_;
  };


} // end of namespace ntg



namespace mlc {

  template <>
  struct traits < ntg::bin >
  {
    typedef unsigned char encoding_type;
  };

} // end of namespace mlc



#endif // ! INTEGRE_REAL_BIN_HH
