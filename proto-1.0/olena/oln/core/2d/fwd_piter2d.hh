#ifndef PROTO_OLN_CORE_2D_FWD_PITER2D_HH
# define PROTO_OLN_CORE_2D_FWD_PITER2D_HH

# include <mlc/contract.hh>

# include <oln/core/abstract/piter.hh>
# include <oln/core/2d/point2d.hh>
# include <oln/core/2d/size2d.hh>
# include <oln/core/props.hh>


namespace oln {

  struct fwd_piter2d;

  template <> struct category_type<fwd_piter2d> { typedef cat::piter ret; };

  template <>
  struct props < cat::piter, fwd_piter2d > : public default_props<cat::piter>
  {
    typedef point2d point_type;
    typedef size2d  size_type;
  };

  struct fwd_piter2d : public abstract::piter< fwd_piter2d >
  {

    typedef abstract::piter<fwd_piter2d> super_type;

    friend class abstract::piter<fwd_piter2d>;

    fwd_piter2d(const size2d& size) :
      super_type(size)
    {
      this->exact_ptr = this;
    }

    void impl_start()
    {
      this->p_.row() = 0;
      this->p_.col() = 0;
    }

    bool impl_is_ok() const
    {
      return this->p_.row() < this->s_.nrows();
    }

    void impl_next()
    {
      ++this->p_.col();
      if (this->p_.col() != this->s_.ncols())
	return;
      this->p_.col() = 0;
      ++this->p_.row();
    }

    void impl_invalidate()
    {
      this->p_.row() = this->s_.nrows();
      this->p_.col() = this->s_.ncols();
    }

  };
}


#endif // ndef PROTO_OLN_CORE_2D_FWD_PITER2D_HH
