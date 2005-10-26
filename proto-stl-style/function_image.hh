#include "2d.hh"
#include "misc.hh"
#include "abstract.hh"

struct checker
{

  checker() {}

  typedef bool return_type;

  const return_type operator()(const point2d& p) const
  {
    if (p.row() % 2 == 0)
      if (p.col() % 2 == 0)
	return true;
      else
	return false;
    else
      if (p.col() % 2 == 0)
	return false;
      else
	return true;
  }
};




template <typename Info, typename Fun>
struct function_image :
  public abstract::lightweight_image<function_image<Info, Fun> >
{
  typedef function_image<Info, Fun> self_type;

  // alias:

  typedef self_type concrete_type;

  typedef typename Info::self_type  info_type;
  typedef typename Info::point_type point_type;

  typedef typename Fun::return_type value_type;
  typedef typename Fun::return_type value_get_type;
  typedef typename mlc::undefined   value_set_type;

  typedef typename Info::fwd_piter_type piter_type;
  typedef typename Info::fwd_piter_type fwd_piter_type;
  typedef typename Info::bkd_piter_type bkd_piter_type;

  typedef mlc::no_type nbh_type;
  typedef mlc::no_type niter_type;

  function_image(const Info& info,
		 const Fun& fun) :
    info_ (info),
    fun_(fun)
  {}

  const value_get_type operator[](const point_type& p) const
  {
    assert(info_.has(p));
    return fun_(p);
  }

  abstract_info<info_type> info() const
  {
    return info_;
  }

  const info_type info_;
  const Fun fun_;

};

