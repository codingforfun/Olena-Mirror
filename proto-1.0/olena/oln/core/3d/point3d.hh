#ifndef OLENA_CORE_3D_POINT3D_HH
# define OLENA_CORE_3D_POINT3D_HH

# include <ostream>

# include <oln/core/abstract/point.hh>
# include <oln/core/coord.hh>

namespace oln {

  struct point3d : public abstract::point< point3d >
  {
    point3d() :
      slice_(0),
      row_(0),
      col_(0)
    {
    }

    point3d(coord_t slice_, coord_t row_, coord_t col_) :
      slice_(slice_),
      row_(row_),
      col_(col_)
    {
    }

    point3d(const point3d& rhs) :
      slice_(rhs.slice_),
      row_(rhs.row_),
      col_(rhs.col_)
    {
    }

    point3d& operator=(const point3d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->slice_ = rhs.slice_;
      this->row_ = rhs.row_;
      this->col_ = rhs.col_;
      return *this;
    }

    bool impl_eq(const point3d& rhs) const
    {
      return this->slice_ == rhs.slice_ and
             this->row_ == rhs.row_     and
             this->col_ == rhs.col_;
    }

    const coord_t slice() const { return slice_; }
    const coord_t row() const { return row_; }
    const coord_t col() const { return col_; }

    coord_t& slice() { return slice_; }
    coord_t& row() { return row_; }
    coord_t& col() { return col_; }

  protected:
    coord_t slice_, row_, col_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::point3d& p)
{
  return ostr << '(' << p.slice() << ',' << p.row() << ',' << p.col() << ')';
}


#endif // ! OLENA_CORE_3D_POINT3D_HH
