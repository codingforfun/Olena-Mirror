#ifndef PROTO_POINT_HH
# define PROTO_POINT_HH

# include "dim2d.hh"


typedef int coord_t;

//Define_exact_type_2(abstract, point);

namespace abstract {

  template <typename E>
  struct point : public mlc::any__best_memory<E>
  {
    bool operator==(const point& rhs) const {
      return this->exact().opeq_impl(rhs.exact());
    }
  protected:
    point() {}
  };

} // end of namespace abstract



// point2d


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
  {}
  point2d(coord_t row_, coord_t col_) :
    row_(row_),
    col_(col_)
  {}
  bool opeq_impl(const point2d& rhs) const {
    return row_ == rhs.row_ && col_ == rhs.col_;
  }
  const coord_t row() const { return row_; }
  const coord_t col() const { return col_; }
  coord_t& row() { return row_; }
  coord_t& col() { return col_; }

  // FIXME:
  point2d operator+(const point2d& rhs) const {
    point2d tmp(row_ + rhs.row_, col_ + rhs.col_);
    return tmp;
  }
protected:
  coord_t row_, col_;
};




#endif // ndef PROTO_POINT_HH
