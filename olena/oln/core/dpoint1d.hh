#ifndef PROTO_OLN_CORE_DPOINT1D_HH
# define PROTO_OLN_CORE_DPOINT1D_HH


namespace oln {

  struct dpoint1d;

  template <>
  struct props <dpoint1d>
  {
    typedef dim1d dim_type;
  };

  struct dpoint1d : public abstract::dpoint< dpoint1d >
  {
    dpoint1d() :
      index_(0)
    {
      this->exact_ptr = this;
    }

    dpoint1d(coord_t index_) :
      index_(index_)
    {
      this->exact_ptr = this;
    }

    dpoint1d(const dpoint1d& rhs) :
      index_(rhs.index_)
    {
      this->exact_ptr = this;
    }

    dpoint1d& operator=(const dpoint1d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->index_ = rhs.index_;
      return *this;
    }

    coord_t delta_impl() const
    {
      return std::abs(index_);
    }
    
    bool op_eq_impl(const dpoint1d& rhs) const
    {
      return this->index_ == rhs.index_;
    }

    bool op_less_impl(const dpoint1d& rhs) const
    {
      return this->index() < rhs.index();
    }

    const dpoint1d op_minus_impl() const
    {
      dpoint1d tmp(-index_);
      return tmp;
    }

    const coord_t index() const { return index_; }

    coord_t& index() { return index_; }

  protected:
    coord_t index_;
  };


  inline
  const point1d op_plus_impl(const point1d& p, const dpoint1d& dp)
  {
    point1d tmp(p.index() + dp.index());
    return tmp;
  }


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::dpoint1d& p)
{
  return ostr << '(' << p.index() << ')';
}


#endif // ndef PROTO_OLN_CORE_DPOINT1D_HH
