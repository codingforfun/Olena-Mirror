#ifndef INTEGRE_COLOR_RGB_8_HH
# define INTEGRE_COLOR_RGB_8_HH


# include <mlc/traits.hh>


namespace ntg {


  enum {
    rgb_red = 0,
    rgb_green = 1,
    rgb_blue = 2
  };

  struct rgb_8
  {
    rgb_8()
    {
      this->value_[rgb_red] = 0;
      this->value_[rgb_green] = 0;
      this->value_[rgb_blue] = 0;
    }

    rgb_8(unsigned char red,
	  unsigned char green,
	  unsigned char blue)
    {
      this->value_[rgb_red] = red;
      this->value_[rgb_green] = green;
      this->value_[rgb_blue] = blue;
    }

    rgb_8(const rgb_8& rhs)
    {
      this->value_[rgb_red] = rhs.red();
      this->value_[rgb_green] = rhs.green();
      this->value_[rgb_blue] = rhs.blue();
    }

    rgb_8& operator=(const rgb_8& rhs)
    {
      this->value_[rgb_red] = rhs.red();
      this->value_[rgb_green] = rhs.green();
      this->value_[rgb_blue] = rhs.blue();
      return *this;
    }

    bool operator==(const rgb_8& rhs) const
    {
      return this->value_[rgb_red] == rhs.red() &&
	this->value_[rgb_green] == rhs.green() &&
	this->value_[rgb_blue] == rhs.blue();
    }

    template <typename V>
    bool operator!=(const V& rhs) const
    {
      return this->value_[rgb_red] != rhs.red() ||
	this->value_[rgb_green] != rhs.green() ||
	this->value_[rgb_blue] != rhs.blue();
    }

    unsigned char& red()
    {
      return value_[rgb_red];
    }

    const unsigned char red() const
    {
      return value_[rgb_red];
    }

    unsigned char& green()
    {
      return value_[rgb_green];
    }

    const unsigned char green() const
    {
      return value_[rgb_green];
    }

    unsigned char& blue()
    {
      return value_[rgb_blue];
    }

    const unsigned char blue() const
    {
      return value_[rgb_blue];
    }

  private:

    unsigned char value_[3];
  };


} // end of namespace ntg



namespace mlc {

  template <>
  struct traits < ntg::rgb_8 >
  {
    typedef unsigned char* encoding_type;
  };

} // end of namespace mlc



#endif // ! INTEGRE_COLOR_RGB_8_HH
