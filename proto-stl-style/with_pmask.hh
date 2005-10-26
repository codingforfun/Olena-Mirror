#ifndef MINIOLN_WITH_PMASK_HH
# define MINIOLN_WITH_PMASK_HH

# include "misc.hh"
# include "abstract.hh"
# include "points.hh"



// Pm is an (I)mage (m)ask
// e.g. a binary image 


// fwd decl
template <class Iter, class Pm> struct with_pmask_iter;



// . . . . . . . . . . . . . . . . . . . . . . . .  with_pmask



template <class Info, class Pm>
struct with_pmask_info : public Info // mixin
{
public:
  typedef with_pmask_info<Info, Pm> self_type;
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
  with_pmask_info(const abstract_info<Info2>& info)
    : super(info),
      pmask_(info.exact().pmask())
  {}
  
  with_pmask_info(const abstract_info<Info>& info,
		  const abstract::points<Pm>& pmask)
    : super(info),
      pmask_(pmask.exact())
  {}

  const Pm& pmask() const { return this->pmask_; }

protected:
  Pm pmask_;
};

set_name_TCC(with_pmask_info);

template <class Info, class Pm>
std::ostream&
operator<<(std::ostream& ostr,
	   const with_pmask_info<Info, Pm>& i)
{
  return ostr << "(super=" << i.get_super() << ", pmask=" << i.pmask() << ")";
}


// set_*iter_of

// general form

template <class Info, class Pm>
struct set_fwd_piter_of < with_pmask_info<Info, Pm> >
{
  typedef oln_pts_type_of(Pm, fwd_piter) piter_t;
  typedef piter_t                        ret;
};

template <class Info, class Pm>
struct set_bkd_piter_of < with_pmask_info<Info, Pm> >
{
  typedef oln_pts_type_of(Pm, bkd_piter) piter_t;
  typedef piter_t                        ret;
};

template <class Info, class Pm>
struct set_fwd_niter_of < with_pmask_info<Info, Pm> >
{
  typedef oln_info_type_of(Info, fwd_niter) ret;
};

template <class Info, class Pm>
struct set_bkd_niter_of < with_pmask_info<Info, Pm> >
{
  typedef oln_info_type_of(Info, bkd_niter) ret;
};


// re-writing:

// *_niter_of( +nbh(+pmask) )  <-  *_niter_of( +pmask(+nbh) )

template <class Info, class Pm, class N>
struct set_fwd_niter_of < with_nbh_info<with_pmask_info<Info, Pm>, N> >
  : public set_fwd_niter_of < with_pmask_info<with_nbh_info<Info, N>, Pm> >
{
};

template <class Info, class Pm, class N>
struct set_bkd_niter_of < with_nbh_info<with_pmask_info<Info, Pm>, N> >
  : public set_bkd_niter_of < with_pmask_info<with_nbh_info<Info, N>, Pm> >
{
};





template <class I, class Pm>
struct with_pmask_ : public abstract::morphed_image< I,
						     with_pmask_<I, Pm> >
{
  typedef with_pmask_<I, Pm> self_type;
  typedef abstract::morphed_image<I, self_type> super;

  // basic properties:
  typedef with_pmask_info<oln_type_of(I, info), Pm> info_type;
  typedef with_pmask_<oln_type_of(I, concrete), Pm> concrete_type;
  // end of basic properties


  // only attribute:

  Pm pmask_; // the pmask can be modified

  // ctors

  with_pmask_()
  {}

  with_pmask_(const abstract::image<I>& ima,
	      const abstract::points<Pm>& pmask)
    : super(ima),
      pmask_(pmask.exact())
  {}

  template <class Info>
  with_pmask_(const abstract_info<Info>& info,
	      const abstract::points<Pm>& pmask)
    : super(info),
      pmask_(pmask.exact())
  {}

  template <class Info>
  with_pmask_(const abstract_info<Info>& info)
    : super(info),
      pmask_(info.exact().pmask())
  {}

  const abstract_info<info_type> info() const
  {
    info_type tmp(this->image_.info(), pmask_);
    return tmp;
  }

  // a pmask does *not* really restrict the domain
  // (accessing a neighboor outside the pmask should be possible)
  // so image methods (has, op[], etc.) are preserved as is

};

set_name_TCC(with_pmask_);


template <class I, class Pm>
with_pmask_<I, Pm> mask(const abstract::image<I>& input,
			const abstract::points<Pm>& pmask)
{
  with_pmask_<I, Pm> tmp(input, pmask);
  return tmp;
}


#endif
