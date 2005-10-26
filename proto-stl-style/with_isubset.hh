#ifndef MINIOLN_WITH_ISUBSET_HH
# define MINIOLN_WITH_ISUBSET_HH

# include "abstract.hh"
# include "misc.hh"
# include "with_imask.hh"



// Is is an (I)mage (s)ubset
// e.g. a binary image 


// . . . . . . . . . . . . . . . . . . . . . . . .  with_isubset



template <class Info, class Is>
struct with_isubset_info : public Info // mixin
{
public:
  typedef with_isubset_info<Info, Is> self_type;
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
  with_isubset_info(const abstract_info<Info2>& info)
    : super(info),
      subset_(info.exact().subset())
  {}
  
  with_isubset_info(const abstract_info<Info>& info,
		    const abstract::image<Is>& subset)
    : super(info),
      subset_(subset.exact())
  {
    mlc::eq<oln_type_of(Is, value), bool>::ensure();
  }

  const Is& subset() const { return this->subset_; }
  const Is& mask() const   { return this->subset_; }

protected:
  const Is subset_;
};

set_name_TCC(with_isubset_info);

template <class Info, class Is>
std::ostream&
operator<<(std::ostream& ostr,
	   const with_isubset_info<Info, Is>& i)
{
  return ostr << "(super=" << i.get_super() << ", subset=" << i.subset() << ")";
}


// set_*iter_of

// general form

template <class Info, class Is>
struct set_fwd_piter_of < with_isubset_info<Info, Is> >
{
  typedef oln_info_type_of(Info, fwd_piter) piter_t;
  typedef partial_iter<piter_t, Is>          ret;
};

template <class Info, class Is>
struct set_bkd_piter_of < with_isubset_info<Info, Is> >
{
  typedef oln_info_type_of(Info, bkd_piter) piter_t;
  typedef partial_iter<piter_t, Is>          ret;
};

template <class Info, class Is>
struct set_fwd_niter_of < with_isubset_info<Info, Is> >
{
  typedef oln_info_type_of(Info, fwd_niter) niter_t;
  typedef partial_iter<niter_t, Is>          ret;
};

template <class Info, class Is>
struct set_bkd_niter_of < with_isubset_info<Info, Is> >
{
  typedef oln_info_type_of(Info, bkd_niter) niter_t;
  typedef partial_iter<niter_t, Is>          ret;
};


// re-writing:

// *_niter_of( +nbh(+subset) )  <-  *_niter_of( +subset(+nbh) )

template <class Info, class Is, class N>
struct set_fwd_niter_of < with_nbh_info<with_isubset_info<Info, Is>, N> >
  : public set_fwd_niter_of < with_isubset_info<with_nbh_info<Info, N>, Is> >
{
};

template <class Info, class Is, class N>
struct set_bkd_niter_of < with_nbh_info<with_isubset_info<Info, Is>, N> >
  : public set_bkd_niter_of < with_isubset_info<with_nbh_info<Info, N>, Is> >
{
};




template <class I, class Is>
struct with_isubset_ : public abstract::morphed_image< I,
						       with_isubset_<I, Is> >
{
  typedef with_isubset_<I, Is> self_type;
  typedef abstract::morphed_image<I, self_type> super;


  // basic properties:
  typedef with_isubset_info<oln_type_of(I, info), Is> info_type;
  typedef with_isubset_<oln_type_of(I, concrete), Is> concrete_type;
  // end of basic properties

  // derived property in use:
  typedef_super(point_type);


  // only attribute:

  Is subset_;


  // ctors

  with_isubset_()
  {}

  with_isubset_(const abstract::image<I>& ima,
		const abstract::image<Is>& subset)
    : super(ima),
      subset_(subset.exact())
  {
    mlc::eq<oln_type_of(Is, value), bool>::ensure();
  }

  template <class Info>
  with_isubset_(const abstract_info<Info>& info,
	       const abstract::image<Is>& subset)
    : super(info),
      subset_(subset.exact())
  {
    mlc::eq<oln_type_of(Is, value), bool>::ensure();
  }

  template <class Info>
  with_isubset_(const abstract_info<Info>& info)
    : super(info),
      subset_(info.exact().subset())
  {}

  const abstract_info<info_type> info() const
  {
    info_type tmp(this->image_.info(), subset_);
    return tmp;
  }

  // restrict domain:

  bool has(const point_type& p) const
  {
    return this->image_.has(p) and subset_[p] == true;
  }

  bool has_large(const point_type& p) const
  {
    return this->has(p);
  }

};

set_name_TCC(with_isubset_);


template <class I, class Is>
with_isubset_<I, Is> subset(const abstract::image<I>& input,
			    const abstract::image<Is>& subset)
{
  mlc::eq<oln_type_of(Is, value), bool>::ensure();
  with_isubset_<I, Is> tmp(input, subset);
  return tmp;
}


#endif
