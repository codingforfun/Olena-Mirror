#ifndef MINIOLN_1D_HH
# define MINIOLN_1D_HH

# include "misc.hh"
# include "mlc.hh"
# include "abstract.hh"
# include "1d_fwd_decls.hh"
# include "gen.hh"
# include "points.hh"



template <class C>
struct grid1d_ : public abstract::grid < grid1d_<C> >
{
  typedef mlc::value<unsigned, 1> dim_type;
  typedef point1d_<C> point_type;
protected:
  grid1d_() {}
};


template <typename C>
struct point1d_ : public abstract::point
{
  typedef point1d_<C> self_type;

  typedef mlc::value<unsigned, 1> dim_type;
  typedef C coord_type;
  typedef dpoint1d_<coord> dpoint_type;
  
  // C is a coord type; we factor for
  // point1d_<float>, point1d_<coord_hexa>, point1d_<very_large_int>

  point1d_(const C& index)
    : index_(index)
  {}

  point1d_()
  {}

  bool operator==(const self_type& rhs) const
  {
    return index_ == rhs.index_;
  }

  bool operator< (const self_type& rhs) const
  {
    return index_ < rhs.index_;
  }

  C& index() { return index_; }
  const C index() const { return index_; }

  C& coord_0() { return index_; } // temp method
  const C coord_1() const { return 0; } // temp method

  const C coord_(unsigned i) const
  {
    precondition(i == 0);
    return index_;
  }

protected:
  C index_;  
};

template <typename C>
std::ostream& operator<<(std::ostream& ostr, const point1d_<C>& p)
{
  return ostr << "p(" << p.index() << ")";
}

const point1d mk_point1d(coord index)
{
  point1d tmp(index);
  return tmp;
}

bbox1d mk_bbox1d(coord min_index,
		 coord max_index)
{
  bbox1d tmp(mk_point1d(min_index),
	     mk_point1d(max_index));
  return tmp;
}


template <typename C>
struct dpoint1d_ : public abstract::dpoint
{
  typedef dpoint1d_<C> self_type;

  typedef C coord_type;
  typedef point1d_<C> point_type;

  dpoint1d_(const C& index)
    : index_(index)
  {}

  dpoint1d_()
  {}

  bool operator==(const self_type& rhs) const {
    return index_ = rhs.index_;
  }

  bool operator< (const self_type& rhs) const
  {
    return index_ < rhs.index_;
  }

  self_type operator-() const {
    self_type tmp(-index_);
    return tmp;
  }

  C& index() { return index_; }
  const C index() const { return index_; }

protected:
  C index_;
};


template <typename C>
std::ostream& operator<<(std::ostream& ostr, const dpoint1d_<C>& dp)
{
  return ostr << "dp(" << dp.index() << ")";
}

template <typename C>
point1d_<C> operator+(const point1d_<C>& p, const dpoint1d_<C>& dp) {
  point1d_<C> tmp(p.index() + dp.index());
  return tmp;
}

template <typename C>
point1d_<C> operator-(const point1d_<C>& p, const dpoint1d_<C>& dp) {
  point1d_<C> tmp(p.index() - dp.index());
  return tmp;
}

template <typename C>
dpoint1d_<C> operator-(const point1d_<C>& p1, const point1d_<C>& p2) {
  dpoint1d_<C> tmp(p1.index() - p2.index());
  return tmp;
}



template <typename C>
struct size_gen_< grid1d_<C> > : public abstract::info
{
  typedef size_gen_< grid1d_<C> > self_type;


  // basic properties:
  typedef grid1d_<C> grid_type;
  typedef fwd_piter1d fwd_piter_type;
  typedef bkd_piter1d bkd_piter_type;
  typedef mlc::no_type nbh_type;
  typedef mlc::no_type fwd_niter_type;
  typedef mlc::no_type bkd_niter_type;

  // derived properties:
  typedef C coord_type;
  typedef point1d_<C> point_type;


  size_gen_()
    : nindices_(0),
      border_(0)
  {}

  size_gen_(const coord_type& nindices,
	    const coord_type& border = 1)
    : nindices_(nindices),
      border_(border)
  {}

  template <class Info>
  size_gen_(const abstract_info<Info>& info)
    : nindices_(info.exact().nindices()),
      border_(1)
  {}

  const coord_type min_index() const { return 0; }
  const coord_type max_index() const { return nindices_ - 1; }

  const bbox_<point_type> bbox() const
  {
    bbox_<point_type> tmp(point_type(0), point_type(nindices_ - 1));
    return tmp;
  }

  const coord_type nindices() const { return nindices_; }
  const coord_type border() const { return border_; }

protected:
  coord_type nindices_, border_;
};

template <typename C>
std::ostream& operator<<(std::ostream& ostr, const size_gen_<grid1d_<C> >& s)
{
  return ostr << "(nindices=" << s.nindices << ", border=" << s.border << ")";
}


template <class C>
struct fwd_piter1d_ : public abstract::piter
{
  typedef fwd_piter1d_<C> self_type;

  typedef C coord_type;
  typedef point1d_<C> point_type;

  typedef self_type to_1d;

  template <class Info>
  fwd_piter1d_(const abstract_info<Info>& info)
    : bb_(info.exact().bbox())
  {
    invalidate();
  }

  fwd_piter1d_(const bbox_<point_type>& bb)
    : bb_(bb)
  {
    invalidate();
  }

  bool is_valid() const {
    return p_.index() <= bb_.max().index();
  }
  void invalidate() {
    p_.index() = bb_.max().index() + 1;
    postcondition(not is_valid());
  }
  void start() {
    p_.index() = bb_.min().index();
    postcondition(is_valid());
  }
  void next() {
    precondition(is_valid());
    ++p_.index();
  }
  operator point_type() const {
    precondition(is_valid());
    return p_;
  }

  // warning: specific method
  const coord_type index() const { return p_.index(); }

protected:
  bbox_<point_type> bb_;
  point_type p_;
};


template <class C>
struct bkd_piter1d_ : public abstract::piter
{
  typedef bkd_piter1d_<C> self_type;

  typedef C coord_type;
  typedef point1d_<C> point_type;

  typedef self_type to_1d;

  template <class Info>
  bkd_piter1d_(const abstract_info<Info>& info)
    : bb_(info.exact().bbox())
  {
    invalidate();
  }

  bkd_piter1d_(const bbox_<point_type>& bb)
    : bb_(bb)
  {
    invalidate();
  }

  bool is_valid() const {
    return p_.index() >= bb_.min().index();
  }
  void invalidate() {
    p_.index() = bb_.min().index() - 1;
    postcondition(not is_valid());
  }
  void start() {
    p_.index() = bb_.max().index();
  }
  void next() {
    precondition(is_valid());
    --p_.index();
  }
  operator point_type() const {
    precondition(is_valid());
    return p_;
  }

  // warning: specific method
  const coord_type index() const { return p_.index(); }

protected:
  bbox_<point_type> bb_;
  point_type p_;
};





// . . . . . . . . . . . . . . . . . . . . . . . .  image1d


template <class T> 
struct image1d : public abstract::data_image< image1d<T> >
{
  typedef image1d<T> self_type;


  // basic properties
  typedef size1d  info_type;
  typedef T value_type;
  typedef T value_get_type;
  typedef T value_set_type;
  // end of basic properties
  
    // derived property in use:
  typedef point1d point_type;


  // interface:

  const bbox1d& bbox() const
  {
    return info_.bbox();
  }

  abstract_info<info_type> info() const
  {
    return info_;
  }

  bool has(const point_type& p) const
  {
    return p.index() >= 0
      and p.index() < info_.nindices();
  }

  bool has_large(const point_type& p) const
  {
    return
      p.index() >= - info_.border()
      and p.index() < info_.nindices() + info_.border();
  }

  const value_get_type operator[](const point_type& p) const
  {
    precondition(has_large(p));
    return data_[p.index() + info_.border()];
  }

  value_set_type& operator[](const point_type& p)
  {
    precondition(has_large(p));
    return data_[p.index() + info_.border()];
  }


  // dedicated methods are evil!
  // TODO: explain why...


  // classic equipment
  image1d() {
    data_ = 0; // no data is possible...
  }
  image1d(const image1d& rhs) {
    data_ = rhs.data_; // ...data is shared
    info_ = rhs.info_;
  }
  image1d& operator=(const image1d& rhs) {
    data_ = rhs.data_; // ...likewise
    info_ = rhs.info_;
    return *this;
  }

  // ctors

  image1d(const coord& nindices)
    : info_(nindices)
  {
    data_ = new T[info_.nindices() + 2 * info_.border()];
  }

  template <class Info>
  image1d(const abstract_info<Info>& info)
  {
    info_ = info_type(info.exact().nindices(),
		      info.exact().border());
    data_ = new T[info_.nindices() + 2 * info_.border()];
  }

  T* data_;
protected:
  info_type info_;
};

set_name_TC(image1d);



neighb1d c2 = neighb1d()
  .add(dpoint1d(-1))
  .add(dpoint1d( 1));


#endif
