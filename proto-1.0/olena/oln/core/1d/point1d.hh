#ifndef OLENA_CORE_1D_POINT1D_HH
# define OLENA_CORE_1D_POINT1D_HH

# include <ostream>

# include <oln/core/abstract/point.hh>
# include <oln/core/coord.hh>

namespace oln {

  struct point1d : public abstract::point< point1d >
  {
    point1d() :
      index_(0)
    {
    }

    point1d(coord_t index_) :
      index_(index_)
    {
    }

    point1d(const point1d& rhs) :
      index_(rhs.index_)
    {
    }

    point1d& operator=(const point1d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->index_ = rhs.index_;
      return *this;
    }

    bool impl_eq(const point1d& rhs) const
    {
      return this->index_ == rhs.index_;
    }

    const coord_t index() const { return index_; }

    coord_t& index() { return index_; }

  protected:
    coord_t index_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::point1d& p)
{
  return ostr << '(' << p.index() << ')';
}


#endif // ! OLENA_CORE_1D_POINT1D_HH
