#ifndef OLENA_CORE_1D_DPOINT1D_HH
# define OLENA_CORE_1D_DPOINT1D_HH

# include <ostream>

# include <oln/core/coord.hh>
# include <oln/core/1d/point1d.hh>

// FIXME: there's an assumption here: we do not need inheritance for
// dpoints.  so abstract::dpoint does not exist...

// FIXME: doc!

// FIXME: test that coords are defined


namespace oln {

  struct dpoint1d
  {
    dpoint1d()
    {
    }

    dpoint1d(coord_t index_) :
      index_(index_)
    {
    }

    dpoint1d(const dpoint1d& rhs) :
      index_(rhs.index_)
    {
    }

    dpoint1d& operator=(const dpoint1d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->index_ = rhs.index_;
      return *this;
    }

    bool operator==(const dpoint1d& rhs) const
    {
      return this->index_ == rhs.index_;
    }

    bool operator!=(const dpoint1d& rhs) const
    {
      return this->index_ != rhs.index_;
    }

    const dpoint1d operator+(const dpoint1d& rhs) const
    {
      dpoint1d tmp(this->index() + rhs.index());
      return tmp;
    }

    const point1d operator+(const point1d& rhs) const
    {
      point1d tmp(this->index() + rhs.index());
      return tmp;
    }

    const dpoint1d operator-() const
    {
      dpoint1d tmp(-this->index());
      return tmp;
    }

    const coord_t index() const { return index_; }
    coord_t& index() { return index_; }

  protected:
    coord_t index_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::dpoint1d& dp)
{
  return ostr << '(' << dp.index() << ')';
}


#endif // ! OLENA_CORE_1D_DPOINT1D_HH
