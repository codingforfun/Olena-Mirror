#ifndef OLENA_CORE_2D_POINT2D_HH
# define OLENA_CORE_2D_POINT2D_HH

# include <ostream>

# include <oln/core/abstract/point.hh>
# include <oln/core/coord.hh>

namespace oln {

  struct point2d : public abstract::point< point2d >
  {
    point2d() :
      row_(0),
      col_(0)
    {
    }

    point2d(coord_t row_, coord_t col_) :
      row_(row_),
      col_(col_)
    {
    }

    point2d(const point2d& rhs) :
      row_(rhs.row_),
      col_(rhs.col_)
    {
    }

    point2d& operator=(const point2d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->row_ = rhs.row_;
      this->col_ = rhs.col_;
      return *this;
    }

    bool impl_eq(const point2d& rhs) const
    {
      return this->row_ == rhs.row_ and this->col_ == rhs.col_;
    }

    const coord_t row() const { return row_; }
    const coord_t col() const { return col_; }

    coord_t& row() { return row_; }
    coord_t& col() { return col_; }

  protected:
    coord_t row_, col_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::point2d& p)
{
  return ostr << '(' << p.row() << ',' << p.col() << ')';
}


#endif // ! OLENA_CORE_2D_POINT2D_HH
