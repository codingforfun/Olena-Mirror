#ifndef OLENA_CORE_COORD_HH
# define OLENA_CORE_COORD_HH

# include <iostream>
# include <limits.h>
# include <mlc/contract.hh>


// FIXME: doc


namespace oln {


  class coord_t
  {
  public:

    typedef int value_type;

    coord_t() :
      value_(undef_())
    {
    }

    coord_t(value_type value) :
      value_(value)
    {
    }

    coord_t& operator=(value_type rhs)
    {
      this->value_ = rhs;
      return *this;
    }

    operator value_type() const
    {
      precondition(this->is_defined());
      return value_;
    }

    bool operator==(value_type rhs) const
    {
      precondition(this->is_defined());
      return value_ == rhs;
    }

    bool operator!=(value_type rhs) const
    {
      precondition(this->is_defined());
      return value_ != rhs;
    }

    const coord_t operator+(value_type rhs) const
    {
      precondition(this->is_defined());
      coord_t tmp(value_ + rhs); return tmp;
    }

    coord_t& operator++()
    {
      precondition(this->is_defined());
      ++value_;
      return *this;
    }

    const coord_t operator++(int)
    {
      precondition(this->is_defined());
      coord_t tmp = *this;
      ++value_;
      return tmp;
    }

    const coord_t operator-(value_type rhs) const
    {
      precondition(this->is_defined());
      coord_t tmp(value_ - rhs);
      return tmp;
    }

    const coord_t operator-() const
    {
      precondition(this->is_defined());
      coord_t tmp(-value_); return tmp;
    }

    coord_t& operator--()
    {
      precondition(this->is_defined());
      --value_;
      return *this;
    }

    const coord_t operator--(int)
    {
      precondition(this->is_defined());
      coord_t tmp = *this;
      --value_;
      return tmp;
    }

    bool is_defined() const
    {
      return value_ != undef_();
    }

    bool is_undefined() const
    {
      return value_ == undef_();
    }

    static const coord_t& infty()
    {
      static coord_t infty_ = INT_MAX;
      return infty_;
    }

    static const coord_t& _infty()
    {
      static coord_t _infty_ = INT_MIN + 1;
      return _infty_;
    }

    // FIXME: to be continued

  private:

    value_type value_;

    static const value_type undef_()
    {
      return INT_MIN;
    }

  };


} // end of namespace oln



std::ostream& operator<<(std::ostream& ostr, const oln::coord_t& c)
{
  precondition(c.is_defined());
  return ostr << oln::coord_t::value_type(c);
}
 

#endif // ! OLENA_CORE_COORD_HH
