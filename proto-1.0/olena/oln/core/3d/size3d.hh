#ifndef OLENA_CORE_3D_SIZE3D_HH
# define OLENA_CORE_3D_SIZE3D_HH

# include <ostream>

# include <oln/core/abstract/size.hh>
# include <oln/core/coord.hh>

namespace oln {


  struct size3d : public abstract::size< size3d >
  {
    size3d() :
      nslices_(0),
      nrows_(0),
      ncols_(0),
      border_(0)
    {}

    size3d(coord_t nslices_, coord_t nrows_, coord_t ncols_) :
      nslices_(nslices_),
      nrows_(nrows_),
      ncols_(ncols_),
      border_(2) // FIXME: 2!
    {}

    size3d(coord_t nslices_, coord_t nrows_, coord_t ncols_, coord_t border_) :
      nslices_(nslices_),
      nrows_(nrows_),
      ncols_(ncols_),
      border_(border_)
    {}

    size3d(const size3d& rhs) :
      nslices_(rhs.nslices_),
      nrows_(rhs.nrows_),
      ncols_(rhs.ncols_),
      border_(rhs.border_)
    {}

    void operator=(const size3d& rhs)
    {
      this->nslices_ = rhs.nslices_;
      this->nrows_ = rhs.nrows_;
      this->ncols_ = rhs.ncols_;
      this->border_ = rhs.border_;
    }

    unsigned long impl_npoints() const
    {
      return (unsigned long)nslices_ * 
             (unsigned long)nslices_ * 
             (unsigned long)ncols_;
    }

    bool impl_eq(const size3d& rhs) const
    {
      return this->nslices_ == rhs.nslices_   and
             this->nrows_ == rhs.nrows_       and
             this->ncols_ == rhs.ncols_;
    }

    const coord_t nslices() const { return nslices_; }
    const coord_t nrows() const { return nrows_; }
    const coord_t ncols() const { return ncols_; }
    const coord_t border() const { return border_; }

  protected:
    coord_t nslices_, nrows_, ncols_, border_;
  };


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::size3d& s)
{
  return ostr << "(nslices=" << s.nslices()
	      << ", nrows=" << s.nrows()
	      << ", ncols=" << s.ncols()
	      << ", border=" << s.border() << ")";
}


#endif // ! OLENA_CORE_3D_SIZE3D_HH
