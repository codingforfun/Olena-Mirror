#ifndef MINIOLN_COORD_HH
# define MINIOLN_COORD_HH

# include <climits>
# include "abstract.hh"


struct coord : public abstract::coord
{
protected:
  int value_;
public:
  int value() const {
    return value_;
  }

  // particular values
  static const int  plus_infty() { return INT_MAX; }
  static const int minus_infty() { return INT_MIN + 1; } // == - plus_infty()
  static const int       undef() { return INT_MIN; }

  // ctors and related
  coord()
    : value_(undef())
  {
    postcondition(not is_defined());
  }
  coord(int value)
    : value_(value)
  {
    postcondition(is_defined());
  }
  operator int() const {
    precondition(is_defined());
    return value_;
  }
  coord& operator=(int value) {
    precondition(value != undef());
    value_ = value;
    return *this;
  }

  // definition
  bool is_defined() const {
    return value_ != undef();
  }
  void set_undefined() {
    value_ = undef();
  }
  bool is_infty() const {
    return value_ == plus_infty() or value_ == minus_infty();
  }

  // +  
  coord& operator++() {
    precondition(is_defined());
    if (is_infty()) return *this;
    ++value_;
    return *this;
  }
  const coord operator++(int) {
    precondition(is_defined());
    if (is_infty()) return *this;
    coord tmp = *this;
    ++value_;
    return tmp;
  }
  coord operator+(int rhs) const {
    precondition(is_defined());
    precondition(not coord(rhs).is_infty());
    if (is_infty()) return *this;
    return value_ + rhs;
  }
  coord& operator+=(int rhs) {
    precondition(is_defined());
    precondition(not coord(rhs).is_infty());
    if (is_infty()) return *this;
    value_ += rhs;
    return *this;
  }

  // -
  const coord operator-() const {
    precondition(is_defined());
    coord tmp(-value_);
    return tmp;
  }
  coord& operator--() {
    precondition(is_defined());
    if (is_infty()) return *this;
    --value_;
    return *this;
  }
  const coord operator--(int) {
    precondition(is_defined());
    if (is_infty()) return *this;
    coord tmp = *this;
    --value_;
    return tmp;
  }
  coord operator-(int rhs) const {
    precondition(is_defined());
    precondition(not coord(rhs).is_infty());
    if (is_infty()) return *this;
    return value_ - rhs;
  }

  // cmp
  bool operator==(const coord& rhs) const {
    precondition(is_defined() and rhs.is_defined());
    return value_ == rhs.value();
  }
  bool operator< (const coord& rhs) const {
    precondition(is_defined() and rhs.is_defined());
    return value_ < rhs.value();
  }

  // TODO: add other ops (*,/,%)
};

set_name(coord);

std::ostream& operator<<(std::ostream& ostr, const coord& c) {
  if (c.value() == coord::plus_infty())
    return ostr << "+oo";
  if (c.value() == coord::minus_infty())
    return ostr << "-oo";
  if (c.value() == coord::undef())
    return ostr << "?";
  return ostr << c.value();
}


#endif
