#ifndef OLENA_CORE_2D_SIZE2D_HH
# define OLENA_CORE_2D_SIZE2D_HH

# include <ostream>

# include <oln/core/abstract/size.hh>
# include <oln/core/coord.hh>

namespace oln {


  struct size2d : public abstract::size< size2d >
  {
    size2d() :
      nrows_(0),
      ncols_(0),
      border_(0)
    {}

    size2d(coord_t nrows_, coord_t ncols_) :
      nrows_(nrows_),
      ncols_(ncols_),
      border_(2) // FIXME: 2!
    {}

    size2d(coord_t nrows_, coord_t ncols_, coord_t border_) :
      nrows_(nrows_),
      ncols_(ncols_),
      border_(border_)
    {}

    size2d(const size2d& rhs) :
      nrows_(rhs.nrows_),
      ncols_(rhs.ncols_),
      border_(rhs.border_)
    {}

    void operator=(const size2d& rhs)
    {
      this->nrows_ = rhs.nrows_;
      this->ncols_ = rhs.ncols_;
      this->border_ = rhs.border_;
    }

    unsigned long impl_npoints() const
    {
      return (unsigned long)nrows_ * (unsigned long)ncols_;
    }

    bool impl_eq(const size2d& rhs) const
    {
      return nrows_ == rhs.nrows_ and ncols_ == rhs.ncols_;
    }

    const coord_t nrows() const { return nrows_; }
    const coord_t ncols() const { return ncols_; }
    const coord_t border() const { return border_; }

  protected:
    coord_t nrows_, ncols_, border_;
  };


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::size2d& s)
{
  return ostr << "(nrows=" << s.nrows()
	      << ", ncols=" << s.ncols()
	      << ", border=" << s.border() << ")";
}


#endif // ! OLENA_CORE_2D_SIZE2D_HH
