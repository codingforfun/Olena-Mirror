#ifndef MINIOLN_WITH_NBH_HH
# define MINIOLN_WITH_NBH_HH

# include <iostream>
# include "abstract.hh"
# include "misc.hh"
# include "gen.hh"



template <class Info, class N>
struct with_nbh_info : public Info
{
  typedef with_nbh_info<Info, N> self_type;
  typedef Info super;
  const Info& get_super() const { return *this; } // use in op<<


  // basic properties:
  typedef N nbh_type;
  // (*)
  typedef mlc::undefined fwd_niter_type;
  typedef mlc::undefined bkd_niter_type;
  
  // derived property in use:
  typedef_super(point_type);


  // (*) we do not want to inherit the niter type from Info; for
  // instance with (image2d+c4), Info is size2d so Info::niter is
  // no_type and thus looks defined!  FIXME: explain more precisely...


  template <class Info2>
  with_nbh_info(const abstract_info<Info2>& info)
    : super(info),
      nbh_(info.exact().nbh())
  {}

  with_nbh_info(const abstract_info<Info>& info,
		const abstract::nbh<N>& nbh)
    : super(info),
      nbh_(nbh.exact())
  {}

  const N& nbh() const { return this->nbh_; }

protected:
  N nbh_;
};

set_name_TCC(with_nbh_info);

template <class Info, class N>
std::ostream&
operator<<(std::ostream& ostr,
	   const with_nbh_info<Info, N>& i)
{
  return ostr << "(super=" << i.get_super() << ", nbh=" << i.nbh() << ")";
}


// set_*iter_of

template <class Info, class N>
struct set_fwd_piter_of < with_nbh_info<Info, N> >
{
  typedef oln_info_type_of(Info, fwd_piter) ret;
};

template <class Info, class N>
struct set_bkd_piter_of < with_nbh_info<Info, N> >
{
  typedef oln_info_type_of(Info, bkd_piter) ret;
};

template <class Grid, class Dpoint>
struct set_fwd_niter_of < with_nbh_info< size_gen_<Grid>, neighb_gen_<Dpoint> > >
{
  typedef size_gen_<Grid> info_t;
  typedef fwd_niter_gen_<oln_info_type_of(info_t, point)> ret;
};

template <class Grid, class Dpoint>
struct set_bkd_niter_of < with_nbh_info< size_gen_<Grid>, neighb_gen_<Dpoint> > >
{
  typedef size_gen_<Grid> info_t;
  typedef bkd_niter_gen_<oln_info_type_of(info_t, point)> ret;
};





template <class I, class N>
struct with_nbh_ : public abstract::extended_image< I,
						    with_nbh_<I, N> >
{
  typedef with_nbh_<I, N> self_type;
  typedef abstract::extended_image<I, self_type> super;

  // basic properties
  typedef with_nbh_info<oln_type_of(I, info), N> info_type;
  typedef with_nbh_<oln_type_of(I, concrete), N> concrete_type;
  // end of basic properties


  with_nbh_()
  {}

  with_nbh_(const abstract::image<I>& ima, // IDEA: relax?
	    const abstract::nbh<N>& nbh)
    : super(ima),
      nbh_(nbh.exact())
  {}

  template <class II>
  with_nbh_(const abstract::image<II>& ima) // ima has a nbhood
    : super(ima),
      nbh_(ima.exact().nbh())
  {}

  template <class Info>
  with_nbh_(const abstract_info<Info>& info)
    : super(info),
      nbh_(info.exact().nbh())
  {}

  const abstract_info<info_type> info() const
  {
    info_type tmp(this->image_.info(), nbh_);
    return tmp;
  }

  const N& nbh() const { return nbh_; }

protected:
  N nbh_;
};

set_name_TCC(with_nbh_);


template <class I, class N>
with_nbh_<I, N> operator+(const abstract::image<I>& input,
			  const abstract::nbh<N>& nbh)
{
  mlc::eq<oln_type_of(I, nbh), mlc::no_type>::ensure();
  with_nbh_<I, N> tmp(input, nbh);
  return tmp;
}


#endif
