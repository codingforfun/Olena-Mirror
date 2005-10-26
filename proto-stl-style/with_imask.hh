#ifndef MINIOLN_WITH_IMASK_HH
# define MINIOLN_WITH_IMASK_HH

# include "abstract.hh"
# include "misc.hh"



// Im is an (I)mage (m)ask
// e.g. a binary image 


// fwd decl
template <class Iter, class Im> struct partial_iter;



// . . . . . . . . . . . . . . . . . . . . . . . .  with_imask



template <class Info, class Im>
struct with_imask_info : public Info // mixin
{
public:
  typedef with_imask_info<Info, Im> self_type;
  typedef Info super;
  const Info& get_super() const { return *this; } // use in op<<


  // basic properties are defined below thru functions
  // (mixin should erase previous definitions)
  typedef mlc::undefined fwd_piter_type;
  typedef mlc::undefined bkd_piter_type;
  typedef mlc::undefined fwd_niter_type;
  typedef mlc::undefined bkd_niter_type;

  // derived property in use:
  typedef_super(point_type);


  template <class Info2>
  with_imask_info(const abstract_info<Info2>& info)
    : super(info),
      mask_(info.exact().mask())
  {}
  
  with_imask_info(const abstract_info<Info>& info,
		  const abstract::image<Im>& mask) // IDEA: relax
    : super(info),
      mask_(mask.exact())
  {
    mlc::eq<oln_type_of(Im, value), bool>::ensure();
  }

  const Im& mask() const { return this->mask_; }

protected:
  Im mask_;
};

set_name_TCC(with_imask_info);

template <class Info, class Im>
std::ostream&
operator<<(std::ostream& ostr,
	   const with_imask_info<Info, Im>& i)
{
  return ostr << "(super=" << i.get_super() << ", mask=" << i.mask() << ")";
}


// set_*iter_of

// general form

template <class Info, class Im>
struct set_fwd_piter_of < with_imask_info<Info, Im> >
{
  typedef oln_info_type_of(Info, fwd_piter) piter_t;
  typedef partial_iter<piter_t, Im>         ret;
};

template <class Info, class Im>
struct set_bkd_piter_of < with_imask_info<Info, Im> >
{
  typedef oln_info_type_of(Info, bkd_piter) piter_t;
  typedef partial_iter<piter_t, Im>         ret;
};

template <class Info, class Im>
struct set_fwd_niter_of < with_imask_info<Info, Im> >
{
  typedef oln_info_type_of(Info, fwd_niter) ret;
};

template <class Info, class Im>
struct set_bkd_niter_of < with_imask_info<Info, Im> >
{
  typedef oln_info_type_of(Info, bkd_niter) ret;
};


// re-writing:

// *_niter_of( +nbh(+mask) )  <-  *_niter_of( +mask(+nbh) )

template <class Info, class Im, class N>
struct set_fwd_niter_of < with_nbh_info<with_imask_info<Info, Im>, N> >
  : public set_fwd_niter_of < with_imask_info<with_nbh_info<Info, N>, Im> >
{
};

template <class Info, class Im, class N>
struct set_bkd_niter_of < with_nbh_info<with_imask_info<Info, Im>, N> >
  : public set_bkd_niter_of < with_imask_info<with_nbh_info<Info, N>, Im> >
{
};




template <class Iter, class Im>
struct partial_iter : public Iter
{
  typedef Iter super;

  // typedef partial_iter<typename Iter::to_1d, Im> to_1d;
  typedef typename Iter::point_type point_type; // FIXME: bad

  const Im mask_; // the mask cannot be modified here
  using super::p_;

  template <class Info>
  partial_iter(const abstract_info<Info>& info)
    : super(info),
      mask_(info.exact().mask())
  {
    invalidate();
  }
  
  bool is_valid() const
  {
    invariant(not super::is_valid() or mask_[p_] == true);
    // check that: "super::is_valid() => (mask_[p_] == true)"

    return super::is_valid();
  }

  void invalidate()
  {
    super::invalidate();
  }

  void start()
  {
    super::start();
    while (super::is_valid() and mask_[p_] == false)
      super::next();
  }

  void next()
  {
    precondition(this->is_valid());
    do
      super::next();
    while (super::is_valid() and mask_[p_] == false);
  }

//   operator point_type() const;
};

set_name_TCC(partial_iter);


template <class I, class Im>
struct with_imask_ : public abstract::morphed_image< I,
						     with_imask_<I, Im> >
{
  typedef with_imask_<I, Im> self_type;
  typedef abstract::morphed_image<I, self_type> super;

  // basic properties:
  typedef with_imask_info<oln_type_of(I, info), Im> info_type;
  typedef with_imask_<oln_type_of(I, concrete), Im> concrete_type;
  // end of basic properties


  // only attribute:

  Im mask_; // the mask can be modified

  // ctors

  with_imask_()
  {}

  with_imask_(const abstract::image<I>& ima,
	      const abstract::image<Im>& mask)
    : super(ima),
      mask_(mask.exact())
  {
    mlc::eq<oln_type_of(Im, value), bool>::ensure();
  }

  template <class Info>
  with_imask_(const abstract_info<Info>& info,
	      const abstract::image<Im>& mask)
    : super(info),
      mask_(mask.exact())
  {
    mlc::eq<oln_type_of(Im, value), bool>::ensure();
  }

  template <class Info>
  with_imask_(const abstract_info<Info>& info)
    : super(info),
      mask_(info.exact().mask())
  {}

  const abstract_info<info_type> info() const
  {
    info_type tmp(this->image_.info(), mask_);
    return tmp;
  }

  // a mask does *not* really restrict the domain
  // (accessing a neighboor outside the mask should be possible)
  // so image methods (has, op[], etc.) are preserved as is

};

set_name_TCC(with_imask_);


template <class I, class Im>
with_imask_<I, Im> mask(const abstract::image<I>& input,
			const abstract::image<Im>& mask)
{
  mlc::eq<oln_type_of(Im, value), bool>::ensure();
  with_imask_<I, Im> tmp(input, mask);
  return tmp;
}


#endif
