#ifndef PROTO_OLN_CORE_2D_ITER2D_HH
# define PROTO_OLN_CORE_2D_ITER2D_HH

# include <mlc/contract.hh>
# include <mlc/objs.hh>

# include <oln/core/abstract/iter.hh>
# include <oln/core/2d/point2d.hh>
# include <oln/core/2d/size2d.hh>
# include <oln/core/props.hh>


namespace oln {

  struct iter2d;

  struct iter2d : abstract::iter<iter2d>
  {

    typedef abstract::iter<iter2d> super_type;

    friend class abstract::iter<iter2d>;

    iter2d() : super_type(), nrows_(0), ncols_(0)
    {}

    iter2d(const size2d& size) :
      super_type(),
      nrows_(size.nrows()),
      ncols_(size.ncols())
    {}


    protected:

    coord_t nrows_;
    coord_t ncols_;
    point2d p_;

    point2d
    to_point() const
    {
      precondition(!end_impl());
      invariant(this->p_.row() >= 0 &&
		(this->p_.row() < nrows_ || this->p_.row() == nrows_) &&
		this->p_.col() >= 0 &&
		this->p_.col() < ncols_);
      return this->p_;
    }


    void op_par_impl(const size2d& size)
    {
      ncols_ = size.ncols();
      nrows_ = size.nrows();
    }

    void
    next_impl()
    {
      this->p_.col()++;
      if (this->p_.col() != this->ncols_)
	return;
      this->p_.col() = 0;
      this->p_.row()++;
    }

    void
    begin_impl()
    {
      this->p_.row() = 0;
      this->p_.col() = 0;
    }

    bool
    end_impl() const
    {
      return this->p_.row() == this->nrows_;
    }


    bool
    noteq_impl(const point2d &p) const
    {
      return p_ != p.exact();
    }

    bool
    eq_impl(const point2d &p) const
    {
      return p_ == p.exact();
    }


  };
}


#endif // ndef PROTO_OLN_CORE_ABSTRACT_ITER_HH
