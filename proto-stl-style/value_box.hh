#ifndef MINIOLN_VALUE_BOX_HH
# define MINIOLN_VALUE_BOX_HH

# include "misc.hh"


template <class I>
struct value_box
{
  typedef_deduced(I, point_type);
  typedef_deduced(I, value_type);

  // attrs
  I* this_;
  mutable point_type p_;

  value_box(I* this_ = 0)
    : this_(this_)
  {}

  template <class V>
  value_box& operator=(const V& value)
  {
    precondition(this_ != 0);
    this_->set(value);
    return *this;
  }

  template <class V>
  operator V() const
  {
    precondition(this_ != 0);
    return this_->get();
  }

  operator value_type() const
  {
    precondition(this_ != 0);
    return this_->get();
  }

  const value_type value() const
  {
    precondition(this_ != 0);
    return this_->get();
  }

  bool operator<(const value_type& rhs) const
  {
    return value() < rhs;
  }

  friend std::ostream& operator<<(std::ostream& ostr,
				  const value_box& vb)
  {
    precondition(vb.this_ != 0);
    return ostr << vb.value();
  }
};

set_name_TC(value_box);


#endif
