#ifndef PROTO_OLN_CORE_POINT1D_HH
# define PROTO_OLN_CORE_POINT1D_HH


namespace oln {

  struct point1d;

  template <>
  struct props <point1d>
  {
    typedef dim1d dim_type;
  };

  struct point1d : public abstract::point< point1d >
  {
    point1d() :
      index_(0)
    {
      this->exact_ptr = this;
    }

    point1d(coord index_) :
      index_(index_)
    {
      this->exact_ptr = this;
    }

    point1d(const point1d& rhs) :
      index_(rhs.index_)
    {
      this->exact_ptr = this;
    }

    point1d& operator=(const point1d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->index_ = rhs.index_;
      return *this;
    }

    bool op_eq_impl(const point1d& rhs) const
    {
      return this->index_ == rhs.index_;
    }

    bool op_less_impl(const point1d& rhs) const
    {
      return
	this->index() < rhs.index();
    }

    const coord index() const { return index_; }

    coord& index() { return index_; }

  protected:
    coord index_;
  };

} // end of namespace oln


namespace ntg {

  template <> struct props < oln::point1d > {
    enum { tag_value = ntg::scalar_tag };
  };

} // end of namespace ntg


std::ostream& operator<<(std::ostream& ostr, const oln::point1d& p)
{
  return ostr << '(' << p.index() << ')';
}


#endif // ndef PROTO_OLN_CORE_POINT1D_HH
