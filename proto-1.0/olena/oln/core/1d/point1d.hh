#ifndef OLENA_CORE_1D_POINT1D_HH
# define OLENA_CORE_1D_POINT1D_HH

# include <ostream>

# include <oln/core/abstract/point.hh>
# include <oln/core/coord.hh>
# include <oln/core/cats.hh>
# include <oln/core/props.hh>

// FIXME: doc

// FIXME: test that coords are defined

namespace oln {

  struct point1d;
  struct dpoint1d;

  template <>
  struct category_type< point1d > { typedef cat::point ret; };

  template <>
  struct props < cat::point, point1d >
  {
    typedef dpoint1d dpoint_type;
  };

  struct point1d : public abstract::point< point1d >
  {
    point1d()
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

    const coord_t index() const { return index_; }
    coord_t& index() { return index_; }

# if defined __GNUC__ && __GNUC__ >= 3
    friend class abstract::point< point1d >;
  protected:
# endif

    const point1d impl_plus(const dpoint1d& rhs) const;

    const dpoint1d impl_minus(const point1d& rhs) const;

    bool impl_eq(const point1d& rhs) const
    {
      return this->index_ == rhs.index_;
    }

  protected:
    
    coord_t index_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::point1d& p)
{
  return ostr << '(' << p.index() << ')';
}


# include <oln/core/1d/dpoint1d.hh>


namespace oln {

  const point1d point1d::impl_plus(const dpoint1d& rhs) const
  {
    point1d tmp(this->index() + rhs.index());
    return tmp;
  }

  const dpoint1d point1d::impl_minus(const point1d& rhs) const
  {
    dpoint1d tmp(this->index() - rhs.index());
    return tmp;
  }

} // end of namespace oln



#endif // ! OLENA_CORE_1D_POINT1D_HH
