#ifndef PROTO_OLN_CORE_POINT2D_HH
# define PROTO_OLN_CORE_POINT2D_HH

# include <oln/core/abstract/point.hh>

namespace oln {

  struct point2d;

  template <>
  struct props <point2d>
  {
    typedef dim2d dim_type;
  };

  struct point2d : public abstract::point< point2d >
  {
    point2d() :
      row_(0),
      col_(0)
    {
      this->exact_ptr = this;
    }

    point2d(coord row_, coord col_) :
      row_(row_),
      col_(col_)
    {
      this->exact_ptr = this;
    }

    point2d(const point2d& rhs) :
      row_(rhs.row_),
      col_(rhs.col_)
    {
      this->exact_ptr = this;
    }

    point2d& operator=(const point2d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->row_ = rhs.row_;
      this->col_ = rhs.col_;
      return *this;
    }

    bool op_eq_impl(const point2d& rhs) const
    {
      // FIXME: pb when abstract::point inherits from
      // any__best_memory
//       std::cout << this->row_ << ' '
// 		<< rhs.row_ << ' '
// 		<< this->col_ << ' '
// 		<< rhs.col_ << std::endl;
//       std::cout << this->row() << ' '
// 		<< rhs.row() << ' '
// 		<< this->col() << ' '
// 		<< rhs.col() << std::endl;
      return this->row_ == rhs.row_ && this->col_ == rhs.col_;
    }

    bool op_less_impl(const point2d& rhs) const
    {
      return
	this->row() < rhs.row() ||
	(this->row() == rhs.row() && this->col() < rhs.col());
    }

    const coord row() const { return row_; }
    const coord col() const { return col_; }

    coord& row() { return row_; }
    coord& col() { return col_; }

  protected:
    coord row_, col_;
  };

} // end of namespace oln


// FIXME: I had to specialize ntg::props; it is strange that
// ntg intervenes!!!
namespace ntg {

  template <> struct props < oln::point2d > {
    enum { tag_value = ntg::vector_tag };
  };

} // end of namespace ntg


std::ostream& operator<<(std::ostream& ostr, const oln::point2d& p)
{
  return ostr << '(' << p.row() << ',' << p.col() << ')';
}


#endif // ndef PROTO_OLN_CORE_POINT2D_HH
