#ifndef OLENA_CORE_2D_DPOINT2D_HH
# define OLENA_CORE_2D_DPOINT2D_HH

# include <ostream>

# include <oln/core/coord.hh>
# include <oln/core/2d/point2d.hh>

// FIXME: there's an assumption here: we do not need inheritance for
// dpoints.  so abstract::dpoint does not exist...

// FIXME: doc!

namespace oln {

  struct dpoint2d
  {
    dpoint2d()
    {
    }

    dpoint2d(coord_t row_, coord_t col_) :
      row_(row_),
      col_(col_)
    {
    }

    dpoint2d(const dpoint2d& rhs) :
      row_(rhs.row_),
      col_(rhs.col_)
    {
    }

    dpoint2d& operator=(const dpoint2d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->row_ = rhs.row_;
      this->col_ = rhs.col_;
      return *this;
    }

    bool operator==(const dpoint2d& rhs) const
    {
      return this->row_ == rhs.row_ and this->col_ == rhs.col_;
    }

    bool operator!=(const dpoint2d& rhs) const
    {
      return not this->operator==(rhs);
    }

    const dpoint2d operator+(const dpoint2d& rhs) const
    {
      dpoint2d tmp(this->row() + rhs.row(), this->col() + rhs.col());
      return tmp;
    }

    const point2d operator+(const point2d& rhs) const
    {
      point2d tmp(this->row() + rhs.row(), this->col() + rhs.col());
      return tmp;
    }

    const dpoint2d operator-() const
    {
      dpoint2d tmp(-this->row(), -this->col());
      return tmp;
    }

    const coord_t row() const { return row_; }
    const coord_t col() const { return col_; }

    coord_t& row() { return row_; }
    coord_t& col() { return col_; }

  protected:
    coord_t row_, col_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::dpoint2d& dp)
{
  return ostr << '(' << dp.row() << ',' << dp.col() << ')';
}


#endif // ! OLENA_CORE_2D_DPOINT2D_HH
