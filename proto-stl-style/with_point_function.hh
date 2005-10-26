#include "2d.hh"
#include "misc.hh"
#include "abstract.hh"


template <typename Info>
struct vflip
{
};

template <>
struct vflip<size2d>
{
  typedef size2d info_type;
  typedef info_type::point_type point_type;

  vflip(const info_type& info) :
    info_(info)
  {}

  const info_type info_;

  const point_type operator()(const point_type& p) const
  {
    return point_type(info_.max_row() - p.row(), p.col());
  }

};



template <typename Ima, typename Fun>
struct image_with_point_function :
  public abstract::decorated_image<Ima, image_with_point_function<Ima, Fun> >
{
  typedef image_with_point_function<Ima, Fun> self_type;
  typedef abstract::decorated_image<Ima, self_type> super_type;
  typedef self_type concrete_type;

  typedef typename Ima::info_type  info_type;
  typedef typename Ima::point_type point_type;

  typedef typename Ima::value_type value_type;
  typedef typename Ima::value_get_type value_get_type;
  typedef typename Ima::value_set_type value_set_type;

  typedef typename Ima::piter_type piter_type;
  typedef typename Ima::fwd_piter_type fwd_piter_type;
  typedef typename Ima::bkd_piter_type bkd_piter_type;

  typedef mlc::no_type nbh_type;
  typedef mlc::no_type niter_type;

  image_with_point_function(const Ima& ima,
		 const Fun& fun) :
    super_type(ima),
    fun_(fun)
  {}


  const Fun fun_;

  const value_get_type operator[](const point_type& p) const
  {
    assert(this->image_.info().exact().has(p));
    return this->image_[fun_(p)];
  }

  abstract_info<info_type> info() const
  {
    return this->image_.info();
  }

};
