#ifndef OLENA_CORE_1D_SIZE1D_HH
# define OLENA_CORE_1D_SIZE1D_HH

# include <ostream>

# include <oln/core/abstract/size.hh>
# include <oln/core/coord.hh>

namespace oln {


  struct size1d : public abstract::size< size1d >
  {
    size1d()
    {}

    size1d(coord_t nindices_) :
      nindices_(nindices_),
      border_(2) // FIXME: 2!
    {}

    size1d(coord_t nindices_, coord_t border_) :
      nindices_(nindices_),
      border_(border_)
    {}

    size1d(const size1d& rhs) :
      nindices_(rhs.nindices_),
      border_(rhs.border_)
    {}

    void operator=(const size1d& rhs)
    {
      this->nindices_ = rhs.nindices_;
      this->border_ = rhs.border_;
    }

    unsigned long impl_npoints() const
    {
      return (unsigned long)nindices_;
    }

    bool impl_eq(const size1d& rhs) const
    {
      return this->nindices_ == rhs.nindices_;
    }

    const coord_t nindices() const { return nindices_; }
    const coord_t border() const { return border_; }

  protected:
    coord_t nindices_, border_;
  };


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::size1d& s)
{
  return ostr << "(nindices=" << s.nindices()
	      << ", border=" << s.border() << ")";
}


#endif // ! OLENA_CORE_1D_SIZE1D_HH
