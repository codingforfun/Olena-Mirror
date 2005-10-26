#ifndef MINIOLN_GEN_HH
# define MINIOLN_GEN_HH

# include <iostream>
# include <utility>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>

# include "abstract.hh"
# include "misc.hh"




template <class Grid>
struct size_gen_ : public abstract::info
{
};


template <class DPoint>
struct neighb_gen_ : public abstract::nbh< neighb_gen_<DPoint> >
{
  typedef neighb_gen_<DPoint> self_type;
  self_type& add(const DPoint& dp)
  {
    dps_.insert(dp);
    dps_.insert(-dp);
    dps.clear();
    std::copy(dps_.begin(), dps_.end(), std::back_inserter(dps));
    return *this;
  }
  const DPoint& operator[](unsigned i) const
  {
    precondition(i < dps.size());
    return dps[i];
  }
  std::vector<DPoint> dps;
protected:
  std::set<DPoint> dps_;
};

set_name_TC(neighb_gen_);

template <class DPoint>
std::ostream&
operator<<(std::ostream& ostr,
	   const neighb_gen_<DPoint>& n)
{
  ostr << "[";
  for (unsigned i = 0; i < n.dps.size(); ++i)
    {
      if (i != 0) ostr << ",";
      ostr << n.dps[i];
    }
  return ostr << "]";
}


template <class Point>
struct fwd_niter_gen_ : public abstract::niter
{
  void ensure_niter() const {} // security

  typedef Point point_type;
  typedef_deduced(point_type, dpoint_type); 
  typedef neighb_gen_<dpoint_type> nbh_type;

  // ctor
  template <class Info>
  fwd_niter_gen_(const abstract_info<Info>& info)
  {
    this->nbh_ = info.exact().nbh();
  }

  void start()
  {
    i = 0;
    postcondition(is_valid());
  }

  bool is_valid() const
  {
    return i < nbh_.dps.size();
  }

  void invalidate()
  {
    i = nbh_.dps.size();
    postcondition(not is_valid());
  }

  void next()
  {
    precondition(is_valid());
    ++i;
  }

  const nbh_type nbh() const { return nbh_; }
  
  operator point_type() const
  {
    precondition(is_valid());
    return p_ + nbh_[i];
  }

  void center_at(const point_type& p)
  {
    p_ = p;
  }
    
protected:
  nbh_type nbh_;
  Point p_;
  unsigned i;
};

set_name_TC(fwd_niter_gen_);





template <class Point>
struct bkd_niter_gen_ : public abstract::niter
{
  void ensure_niter() const {} // security

  typedef Point point_type;
  typedef_deduced(point_type, dpoint_type); 
  typedef neighb_gen_<dpoint_type> nbh_type;

  // ctor
  template <class Info>
  bkd_niter_gen_(const abstract_info<Info>& info)
  {
    this->nbh_ = info.exact().nbh();
  }

  void start()
  {
    i = nbh_.dps.size() - 1;
    postcondition(is_valid());
  }

  bool is_valid() const
  {
    return i > -1;
  }

  void invalidate()
  {
    i = -1;
    postcondition(not is_valid());
  }

  void next()
  {
    precondition(is_valid());
    --i;
  }

  const nbh_type nbh() const { return nbh_; }
  
  operator point_type() const
  {
    precondition(is_valid());
    return p_ + nbh_[i];
  }

  void center_at(const point_type& p)
  {
    p_ = p;
  }
    
protected:
  nbh_type nbh_;
  Point p_;
  int i;
};

set_name_TC(bkd_niter_gen_);


#endif
