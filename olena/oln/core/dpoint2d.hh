#ifndef PROTO_OLN_CORE_DPOINT2D_HH
# define PROTO_OLN_CORE_DPOINT2D_HH


namespace oln {

  struct dpoint2d;

  template <>
  struct props <dpoint2d>
  {
    typedef dim2d dim_type;
  };

  struct dpoint2d : public abstract::dpoint< dpoint2d >
  {
    dpoint2d() :
      row_(0),
      col_(0)
    {
      this->exact_ptr = this;
    }

    dpoint2d(coord row_, coord col_) :
      row_(row_),
      col_(col_)
    {
      this->exact_ptr = this;
    }

    dpoint2d(const dpoint2d& rhs) :
      row_(rhs.row_),
      col_(rhs.col_)
    {
      this->exact_ptr = this;
    }

    dpoint2d& operator=(const dpoint2d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->row_ = rhs.row_;
      this->col_ = rhs.col_;
      return *this;
    }

    coord delta_impl() const
    {
      return std::max(std::abs(row_), std::abs(col_));
    }

    bool op_eq_impl(const dpoint2d& rhs) const
    {
      return this->row_ == rhs.row_ && this->col_ == rhs.col_;
    }

    bool op_less_impl(const dpoint2d& rhs) const
    {
      return
	this->row() < rhs.row() ||
	(this->row() == rhs.row() && this->col() < rhs.col());
    }

    const dpoint2d op_minus_impl() const
    {
      dpoint2d tmp(-row_, -col_);
      return tmp;
    }

    const coord row() const { return row_; }
    const coord col() const { return col_; }

    coord& row() { return row_; }
    coord& col() { return col_; }

  protected:
    coord row_, col_;
  };


  inline
  const point2d op_plus_impl(const point2d& p, const dpoint2d& dp)
  {
    point2d tmp(p.row() + dp.row(), p.col() + dp.col());
    return tmp;
  }


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::dpoint2d& p)
{
  return ostr << '(' << p.row() << ',' << p.col() << ')';
}


#endif // ndef PROTO_OLN_CORE_DPOINT2D_HH
