#ifndef MINIOLN_WITH_POINTS_HH
# define MINIOLN_WITH_POINTS_HH

# include "abstract.hh"
# include "misc.hh"



// fwd decl
template <class Iter, class Pts> struct partial_iter;



// . . . . . . . . . . . . . . . . . . . . . . . .  with_points



template <class Info, class Pts>
struct with_points_info : public Info // mixin
{
public:
  typedef with_points_info<Info, Pts> self_type;
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
  with_points_info(const abstract_info<Info2>& info)
    : super(info),
      pts_(info.exact().pts())
  {}
  
  with_points_info(const abstract_info<Info>& info,
		 const abstract::image<Pts>& pts) // IDEA: relax
    : super(info),
      pts_(pts.exact())
  {}

  const Pts& pts() const { return this->pts_; }

protected:
  Pts pts_;
};

set_name_TCC(with_points_info);

template <class Info, class Pts>
std::ostream&
operator<<(std::ostream& ostr,
	   const with_points_info<Info, Pts>& i)
{
  return ostr << "(super=" << i.get_super() << ", pts=" << i.pts() << ")";
}


// set_*iter_of

// general form

template <class Info, class Pts>
struct set_fwd_piter_of < with_points_info<Info, Pts> >
{
  typedef oln_info_type_of(Info, fwd_piter) piter_t;
  typedef partial_iter<piter_t, Pts>         ret;
};

template <class Info, class Pts>
struct set_bkd_piter_of < with_points_info<Info, Pts> >
{
  typedef oln_info_type_of(Info, bkd_piter) piter_t;
  typedef partial_iter<piter_t, Pts>         ret;
};

template <class Info, class Pts>
struct set_fwd_niter_of < with_points_info<Info, Pts> >
{
  typedef oln_info_type_of(Info, fwd_niter) ret;
};

template <class Info, class Pts>
struct set_bkd_niter_of < with_points_info<Info, Pts> >
{
  typedef oln_info_type_of(Info, bkd_niter) ret;
};


// re-writing:

// *_niter_of( +nbh(+pts) )  <-  *_niter_of( +pts(+nbh) )

template <class Info, class Pts, class N>
struct set_fwd_niter_of < with_nbh_info<with_points_info<Info, Pts>, N> >
  : public set_fwd_niter_of < with_points_info<with_nbh_info<Info, N>, Pts> >
{
};

template <class Info, class Pts, class N>
struct set_bkd_niter_of < with_nbh_info<with_points_info<Info, Pts>, N> >
  : public set_bkd_niter_of < with_points_info<with_nbh_info<Info, N>, Pts> >
{
};




template <class Iter, class Pts>
struct partial_iter : public Iter
{
  typedef Iter super;

  // typedef partial_iter<typename Iter::to_1d, Pts> to_1d;
  typedef typename Iter::point_type point_type; // FIXME: bad

  const Pts pts_; // the pts cannot be modified here
  using super::p_;

  template <class Info>
  partial_iter(const abstract_info<Info>& info)
    : super(info),
      pts_(info.exact().pts())
  {
    invalidate();
  }
  
  bool is_valid() const
  {
    invariant(not super::is_valid() or pts_[p_] == true);
    // check that: "super::is_valid() => (pts_[p_] == true)"

    return super::is_valid();
  }

  void invalidate()
  {
    super::invalidate();
  }

  void start()
  {
    super::start();
    while (super::is_valid() and pts_[p_] == false)
      super::next();
  }

  void next()
  {
    precondition(this->is_valid());
    do
      super::next();
    while (super::is_valid() and pts_[p_] == false);
  }

//   operator point_type() const;
};

set_name_TCC(partial_iter);


template <class I, class M>
struct with_points_ : public abstract::morphed_image< I,
						      with_points_<I, M> >
{
  typedef with_points_<I, M> self_type;
  typedef abstract::morphed_image<I, self_type> super;

  // basic properties:
  typedef with_points_info<oln_type_of(I, info), M> info_type;
  typedef with_points_<oln_type_of(I, concrete), M> concrete_type;
  // end of basic properties


  // only attribute:

  M pts_; // the pts can be modified

  // ctors

  with_points_()
  {}

  with_points_(const abstract::image<I>& ima,
	     const abstract::image<M>& pts)
    : super(ima),
      pts_(pts.exact())
  {}

  template <class Info>
  with_points_(const abstract_info<Info>& info,
	     const abstract::image<M>& pts)
    : super(info),
      pts_(pts.exact())
  {}

  template <class Info>
  with_points_(const abstract_info<Info>& info)
    : super(info),
      pts_(info.exact().pts())
  {}

  const abstract_info<info_type> info() const
  {
    info_type tmp(this->image_.info(), pts_);
    return tmp;
  }

  // a pts does *not* really restrict the domain
  // (accessing a neighboor outside the pts should be possible)
  // so image methods (has, op[], etc.) are preserved as is

};

set_name_TCC(with_points_);



template <class I, class Pts>
with_points_<I, M> mask(const abstract::image<I>& input,
			const abstract::points<Pts>& pts)
{
  with_points_<I, Pts> tmp(input, pts);
  return tmp;
}


#endif
