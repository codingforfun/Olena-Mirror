#ifndef OLENA_CORE_2D_POINT2D_HH
# define OLENA_CORE_2D_POINT2D_HH

# include <ostream>

# include <oln/core/abstract/point.hh>
# include <oln/core/coord.hh>
# include <oln/core/cats.hh>
# include <oln/core/props.hh>

// FIXME: doc!

namespace oln {

  struct point2d;
  struct dpoint2d;

  template <>
  struct category_type< point2d > { typedef cat::point ret; };

  template <>
  struct props < cat::point, point2d >
  {
    typedef dpoint2d dpoint_type;
  };

  struct point2d : public abstract::point< point2d >
  {
    point2d()
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

    const coord_t row() const { return row_; }
    const coord_t col() const { return col_; }

    coord_t& row() { return row_; }
    coord_t& col() { return col_; }

# if defined __GNUC__ && __GNUC__ >= 3
    friend class abstract::point< point2d >;
  protected:
# endif

    const point2d impl_plus(const dpoint2d& rhs) const;

    const dpoint2d impl_minus(const point2d& rhs) const;

    bool impl_eq(const point2d& rhs) const
    {
      return this->row_ == rhs.row_ and this->col_ == rhs.col_;
    }

  protected:

    coord_t row_, col_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::point2d& p)
{
  return ostr << '(' << p.row() << ',' << p.col() << ')';
}


# include <oln/core/2d/dpoint2d.hh>


namespace oln {

  const point2d point2d::impl_plus(const dpoint2d& rhs) const
  {
    point2d tmp(this->row() + rhs.row(), this->col() + rhs.col());
    return tmp;
  }

  const dpoint2d point2d::impl_minus(const point2d& rhs) const
  {
    dpoint2d tmp(this->row() - rhs.row(), this->col() - rhs.col());
    return tmp;
  }

} // end of namespace oln


#endif // ! OLENA_CORE_2D_POINT2D_HH
