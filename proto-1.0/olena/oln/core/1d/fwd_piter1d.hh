#ifndef PROTO_OLN_CORE_1D_FWD_PITER1D_HH
# define PROTO_OLN_CORE_1D_FWD_PITER1D_HH

# include <mlc/contract.hh>

# include <oln/core/abstract/piter.hh>
# include <oln/core/1d/point1d.hh>
# include <oln/core/1d/size1d.hh>
# include <oln/core/props.hh>


namespace oln {

  struct fwd_piter1d;

  template <> struct category_type< fwd_piter1d > { typedef cat::piter ret; };

  template <>
  struct props < cat::piter, fwd_piter1d > : public default_props< cat::piter >
  {
    typedef point1d point_type;
    typedef size1d  size_type;
  };

  struct fwd_piter1d : public abstract::piter< fwd_piter1d >
  {

    typedef abstract::piter<fwd_piter1d> super_type;

    fwd_piter1d(const size1d& size) :
      super_type(size)
    {
      this->exact_ptr = this;
      this->invalidate();
    }

# if defined __GNUC__ && __GNUC__ >= 3
    friend class abstract::piter< fwd_piter1d >;
  protected:
# endif

    void impl_start()
    {
      this->p_.index() = 0;
      postcondition(this->p_.index().is_defined());
    }

    bool impl_is_valid() const
    {
      precondition(this->p_.index().is_defined());
      return this->p_.index() < this->s_.nindices();
    }

    void impl_next()
    {
      precondition(this->p_.index().is_defined());
      precondition(this->p_.index() >= 0 and this->p_.index() <= this->s_.nindices());
      if (this->p_.index() == this->s_.nindices())
	return;
      ++this->p_.index();
      postcondition(this->p_.index().is_defined());
    }

    void impl_invalidate()
    {
      this->p_.index() = this->s_.nindices();
      postcondition(this->p_.index().is_defined());
    }

  };
}


#endif // ndef PROTO_OLN_CORE_1D_FWD_PITER1D_HH
