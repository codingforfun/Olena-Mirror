#ifndef MINIOLN_POINTS_HH
# define MINIOLN_POINTS_HH

# include <iostream>
# include <vector>
# include <list>
# include <utility>
# include <set>

# include "abstract.hh"
# include "misc.hh"
# include "mlc.hh"
# include "deductions_1.hh"
# include "type_of.hh"
# include "algos.hh"



// ---------------------------------------- (not abstract) generic bbox


template <class P>
struct bbox_ : public abstract::points< bbox_<P> >
{
  typedef bbox_<P> self_type;

  // basic properties:
  typedef P point_type;
  typedef fwd_piter_from(point_type) fwd_piter_type;
  typedef bkd_piter_from(point_type) bkd_piter_type;

  bbox_(const P& min, const P& max)
    : min_(min), max_(max)
  {
    precondition(min < max or min == max);
  }

  const P& min() const { return min_; }
  const P& max() const { return max_; }

  bbox_<P>& operator()(const P& p)
  {
    min_(p);
    max_(p);
    return *this;
  }

  bool operator==(const self_type& rhs) const
  {
    return P(min_) == P(rhs.min_) and P(max_) == P(rhs.max_);
  }

  bool operator<(const self_type& rhs) const
  {
    for (unsigned i = 0; i < P::dim_type::val; ++i)
      if (P(rhs.min_).coord_(i) < P(min_).coord_(i)
	  or P(rhs.max_).coord_(i) > P(max_).coord_(i))
	return false;
    return true;
  }

protected:
  min_acc<P> min_;
  max_acc<P> max_;
};

set_name_TC(bbox_);

template <typename P>
std::ostream& operator<<(std::ostream& ostr, const bbox_<P>& bb)
{
  return ostr << "[" << bb.min() << ".." << bb.max() << "]";
}

template <class I>
bbox_<oln_type_of(I, point)>
mk_bbox(const I& input) // abstract::binary_image
{
  mlc_is_a(I, abstract::image)::ensure();
  mlc::eq<oln_type_of(I, value), bool>::ensure();

  bbox_<oln_type_of(I, point)> output;

  oln_type_of(I, piter) p(input.info());
  for_all_p(p)
    if (input[p])
      output(p);

  return output;  
}



// ----------------------------------------  stl_< std::container<P> >



// fwd decls
template <class container_P> struct fwd_stl_piter_;
template <class container_P> struct bkd_stl_piter_;


template <class container_P>
struct stl_ : public abstract::points< stl_<container_P> >
{
  // basic properties:
  typedef typename container_P::value_type point_type;
  typedef fwd_stl_piter_<container_P> fwd_piter_type;
  typedef bkd_stl_piter_<container_P> bkd_piter_type;

  stl_(const container_P& pts)
    : pts_(pts)
  {}

  const container_P& data() const
  {
    return pts_;
  }

protected:
  container_P pts_;
};

set_name_TC(stl_);

template <typename container_P>
std::ostream& operator<<(std::ostream& ostr, const stl_<container_P>& pts)
{
  typename container_P::const_iterator i;
  ostr << "{";
  for (i = pts.data().begin();
       i != pts.data().end();
       ++i)
    {
      if (i != pts.data().begin())
	ostr << ", ";
      ostr << *i;
    }
  ostr << "}";
  return ostr;
}

template <class I>
stl_< std::vector<oln_type_of(I, point)> >
mk_pvec(const I& input) // abstract::binary_image
{
  mlc_is_a(I, abstract::image)::ensure();
  mlc::eq<oln_type_of(I, value), bool>::ensure();

  typedef std::vector<oln_type_of(I, point)> vector_t;
  vector_t v;

  oln_type_of(I, piter) p(input.info());
  for_all_p(p)
    if (input[p])
      v.push_back(p);

  stl_<vector_t> output(v);
  return output;
}

template <class I>
stl_< std::list<oln_type_of(I, point)> >
mk_plist(const I& input) // abstract::binary_image
{
  mlc_is_a(I, abstract::image)::ensure();
  mlc::eq<oln_type_of(I, value), bool>::ensure();

  typedef std::list<oln_type_of(I, point)> list_t;
  list_t l;

  oln_type_of(I, piter) p(input.info());
  for_all_p(p)
    if (input[p])
      l.push_back(p);

  stl_<list_t> output(l);
  return output;
}

template <class I>
stl_< std::set<oln_type_of(I, point)> >
mk_pset(const I& input) // abstract::binary_image
{
  mlc_is_a(I, abstract::image)::ensure();
  mlc::eq<oln_type_of(I, value), bool>::ensure();

  typedef std::set<oln_type_of(I, point)> set_t;
  set_t s;

  oln_type_of(I, piter) p(input.info());
  for_all_p(p)
    if (input[p])
      s.insert(p);

  stl_<set_t> output(s);
  return output;  
}



template <class container_P>
struct fwd_stl_piter_ : public abstract::piter
{
  typedef typename container_P::value_type point_type;
  // FIXME bad: typedef self_type to_1d;
  
  fwd_stl_piter_(const stl_<container_P>& pts)
    : pts_(pts.data())
  {
    invalidate();
  }

  template <class Info>
  fwd_stl_piter_(const abstract_info<Info>& info)
    : pts_(info.exact().pmask().data())
  {
  }

  bool is_valid() const {
    return i_ != pts_.end();
  }
  void invalidate() {
    i_ = pts_.end();
    postcondition(not is_valid());
  }
  void start() {
    i_ = pts_.begin();
    postcondition(is_valid());
  }
  void next() {
    precondition(is_valid());
    ++i_;
  }
  operator point_type() const {
    precondition(is_valid());
    return *i_;
  }

protected:
  const container_P& pts_;
  typename container_P::const_iterator i_;
  point_type p_;
};

set_name_TC(fwd_stl_piter_);



template <class container_P>
struct bkd_stl_piter_ : public abstract::piter
{
  typedef typename container_P::value_type point_type;
  // FIXME bad: typedef self_type to_1d;
  
  bkd_stl_piter_(const stl_<container_P>& pts)
    : pts_(pts.data())
  {
    invalidate();
  }

  template <class Info>
  bkd_stl_piter_(const abstract_info<Info>& info)
    : pts_(info.exact().pmask().data())
  {
  }

  bool is_valid() const {
    return i_ != pts_.rend();
  }
  void invalidate() {
    i_ = pts_.rend();
    postcondition(not is_valid());
  }
  void start() {
    i_ = pts_.rbegin();
    postcondition(is_valid());
  }
  void next() {
    precondition(is_valid());
    ++i_;
  }
  operator point_type() const {
    precondition(is_valid());
    return *i_;
  }

protected:
  const container_P& pts_;
  typename container_P::const_reverse_iterator i_;
  point_type p_;
};

set_name_TC(bkd_stl_piter_);


// ----------------------------------------  rle_ < P >
//             for run-length endoded points


// fwd decls
template <class P> struct fwd_rle_piter_;
template <class P> struct bkd_rle_piter_;


template <class P>
struct rle_ : public abstract::points< rle_<P> >
{
  // basic properties:
  typedef P point_type;
  typedef fwd_rle_piter_<P> fwd_piter_type;
  typedef bkd_rle_piter_<P> bkd_piter_type;

  typedef std::vector< std::pair<point_type, unsigned> > storage_type;

  rle_()
  {}

  void add(const point_type& p, unsigned len)
  {
    std::pair<point_type, unsigned> tmp(p, len);
    data_.push_back(tmp);
  }
  
  const storage_type& data() const
  {
    return data_;
  }

protected:
  storage_type data_;
};

set_name_TC(rle_);

template <typename P>
std::ostream& operator<<(std::ostream& ostr, const rle_<P>& pts)
{
  typename rle_<P>::storage_type::const_iterator i;
  ostr << "{";
  for (i = pts.data().begin();
       i != pts.data().end();
       ++i)
    {
      if (i != pts.data().begin())
	ostr << ", ";
      P p  = i->first;
      unsigned n = i->second; 
      if (n == 1)
	ostr << p;
      else
	{
	  P p_end = p;
	  p_end.coord_0() += n - 1;
	  ostr << p << ".." << p_end;
	}
    }
  ostr << "}";
  return ostr;
}



template <class I>
rle_<oln_type_of(I, point)>
mk_rlep(const I& input) // abstract::binary_image
{
  mlc_is_a(I, abstract::image)::ensure();
  mlc::eq<oln_type_of(I, value), bool>::ensure();
  typedef oln_type_of(I, point) point_t;

  rle_<point_t> output;

  oln_type_of(I, fwd_piter) p(input.info());
  p.start();
  if (not p.is_valid())
    return output;

  // init
  oln_type_of(I, coord) curr_coord_1 = point_t(p).coord_1();
  bool running = false;
  // no default value for:
  oln_type_of(I, point) start;
  unsigned n;

  for_all_p(p)
    {
      // new line
      if (point_t(p).coord_1() != curr_coord_1)
	{
	  curr_coord_1 = point_t(p).coord_1();
	  if (running)
	    // close the run-length...
	    {
	      output.add(start, n);
	      running = false;
	    }
	  // ...and process p as usual
	}

      if (running)
	if (not input[p])
	  {
	    output.add(start, n);
	    running = false;
	  }
	else // input[p]
	  ++n;
      else // not running
	if (input[p])
	  {
	    start = p;
	    n = 1;
	    running = true;
	  }
    }
  return output;
}


template <class P>
struct fwd_rle_piter_ : public abstract::piter
{
  typedef P point_type;
  typedef std::vector< std::pair<point_type, unsigned> > storage_type;

  fwd_rle_piter_(const rle_<P>& pts)
    : v_(pts.data())
  {
    precondition(not pts.empty());
    invalidate();
  }

  template <class Info>
  fwd_rle_piter_(const abstract_info<Info>& info)
    : v_(info.exact().pmask().data())
  {
  }

  bool is_valid() const {
    return iv_ != v_.end();
  }
  void invalidate() {
    iv_ = v_.end();
    postcondition(not is_valid());
  }
  void start() {
    iv_ = v_.begin();
    p_ = iv_->first;
    n_ = iv_->second;
    i_ = 0;
    postcondition(is_valid());
  }
  void next() {
    precondition(is_valid());
    ++i_;
    if (i_ == n_)
      {
	++iv_;
	if (iv_ == v_.end())
	  return;
	p_ = iv_->first;
	n_ = iv_->second;
	i_ = 0;
      }
    else
      p_.coord_0() += 1;
  }
  operator point_type() const {
    precondition(is_valid());
    return p_;
  }
protected:
  const storage_type& v_;
  typename storage_type::const_iterator iv_;
  unsigned i_, n_;
  point_type p_;
};

set_name_TC(fwd_rle_piter_);


template <class P>
struct bkd_rle_piter_ : public abstract::piter
{
  typedef P point_type;
  typedef std::vector< std::pair<point_type, unsigned> > storage_type;

  bkd_rle_piter_(const rle_<P>& pts)
    : v_(pts.data())
  {
    precondition(not pts.empty());
    invalidate();
  }

  template <class Info>
  bkd_rle_piter_(const abstract_info<Info>& info)
    : v_(info.exact().pmask().data())
  {
  }

  bool is_valid() const {
    return iv_ != v_.rend();
  }
  void invalidate() {
    iv_ = v_.rend();
    postcondition(not is_valid());
  }
  void start() {
    iv_ = v_.rbegin();
    p_ = iv_->first;
    n_ = iv_->second;
    p_.coord_0() += n_ - 1;
    i_ = 0;
    postcondition(is_valid());
  }
  void next() {
    precondition(is_valid());
    ++i_;
    if (i_ == n_)
      {
	++iv_;
	if (iv_ == v_.rend())
	  return;
	p_ = iv_->first;
	n_ = iv_->second;
	p_.coord_0() += n_ - 1;
	i_ = 0;
      }
    else
      p_.coord_0() += -1;
  }
  operator point_type() const {
    precondition(is_valid());
    return p_;
  }
protected:
  const storage_type& v_;
  typename storage_type::const_reverse_iterator iv_;
  unsigned i_, n_;
  point_type p_;
};

set_name_TC(bkd_rle_piter_);


#endif
