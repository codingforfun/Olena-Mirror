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

    fwd_piter2d(const size2d& size) :
      super_type(size)
    {
      this->exact_ptr = this;
      this->invalidate();
    }

# if defined __GNUC__ && __GNUC__ >= 3
    friend class abstract::piter< fwd_piter2d >;
  protected:
# endif

    void impl_start()
    {
      this->p_.row() = 0;
      this->p_.col() = 0;
      postcondition(this->p_.row().is_defined() and this->p_.col().is_defined());
    }

    bool impl_is_valid() const
    {
      precondition(this->p_.row().is_defined() and this->p_.col().is_defined());
      return this->p_.row() < this->s_.nrows();
    }

    void impl_next()
    {
      precondition(this->p_.row().is_defined() and this->p_.col().is_defined());
      precondition(this->p_.row() >= 0 and this->p_.row() <= this->s_.nrows()
		   and
		   this->p_.col() >= 0 and this->p_.col() <= this->s_.ncols());
      ++this->p_.col();
      if (this->p_.col() != this->s_.ncols())
	return;
      this->p_.col() = 0;
      precondition(this->p_.row() != this->s_.nrows());
      ++this->p_.row();
      postcondition(this->p_.row().is_defined() and this->p_.col().is_defined());
    }

    void impl_invalidate()
    {
      this->p_.row() = this->s_.nrows();
      this->p_.col() = this->s_.ncols();
      postcondition(this->p_.row().is_defined() and this->p_.col().is_defined());
    }

  };
}


#endif // ndef PROTO_OLN_CORE_2D_FWD_PITER2D_HH
