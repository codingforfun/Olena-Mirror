#ifndef MINIOLN_TYPE_OF_HH
# define MINIOLN_TYPE_OF_HH


# include "misc.hh"


// tags


namespace cat { // category for short

  struct coord;
  struct grid;
  struct point;
  struct points;
  struct dpoint;
  struct piter;
  struct niter;
  struct nbh;
  struct info;
  struct image;
  // WARNING: ensure that we have here the same list as in abstract.hh


} // end of namespace cat


namespace tag { // for typedefs

  struct bkd_niter;
  struct bkd_piter;
  struct concrete;
  struct coord;
  struct dim;
  struct dpoint;
  struct fwd_niter;
  struct fwd_piter;
  struct grid;
  struct info;
  struct nbh;
  struct niter;
  struct piter;
  struct point;
  struct self;
  struct size;
  struct super;
  struct super;
  struct value_get;
  struct value_set;
  struct value;
  // WARNING: ensure that we have here the same list as in mlc.hh

} // end of namespace tag


template <class category, class what, class from>
struct set_type_of
{
  typedef mlc::undefined ret;
};



namespace internal
{


  // code dedicated to static check that a typedef is
  // properly defined (this code replaces some mlc meta-codes)
  template <class ret1, class ret2>
  struct is_ok {
    enum { val = false };
  };
  template <>
  struct is_ok <mlc::undefined, mlc::undefined> {
    enum { val = false };
  };
  template <class ret>
  struct is_ok <ret, mlc::undefined> {
    enum { val = true };
  };
  template <class ret>
  struct is_ok <mlc::undefined, ret> {
    enum { val = true };
  };
  template <class ret>
  struct is_ok <ret, ret> {
    enum { val = true };
  };

  template <class ret1, class ret2, bool ok>
  struct helper_f_get_type_of {
  };
  template <class T>
  struct helper_f_get_type_of <T, mlc::undefined, true> {
    typedef T ret;
  };
  template <class T>
  struct helper_f_get_type_of <mlc::undefined, T, true> {
    typedef T ret;
  };
  template <class T>
  struct helper_f_get_type_of <T, T, true> {
    typedef T ret;
  };
  // end of dedicated code



  template <class category, class what, class from, class typedef_ret>
  struct f_get_type_of
  {
    typedef typename set_type_of<category, what, from>::ret set_type_ret;

    // we have to check that:
    // 1. either typedef_ret or set_type_ret is defined
    //    (otherwise a definition has been forgotten)
    // 2. typedef_ret and set_type_ret cannot be simultaneously
    //    defined (otherwise two definitions for the same thing
    //    are given)

    // these constraints are *not* too strict; they force to bring
    // to the fore ambiguities in definitions of deduced types

    // for instance, a problem occurs with mixins when we inherit
    // a property through a superior class and still want to
    // (re-)define it; e.g., have a look to with_nbh_info::niter_type.

    // FIXME: rewrite and give details for the following:
    // but say that we have relaxed the multi-"same"-definition(s)
    // to allow for writing "typedef something point_type" in a
    // class for use in method sig...

    enum { ok = is_ok<typedef_ret, set_type_ret>::val };
    typedef typename helper_f_get_type_of<typedef_ret, set_type_ret, ok>::ret ret;
  };


} // end of namespace internal



// macros
// ---------------------------------

# define internal_type_of(Category, From, What)				\
typename internal::f_get_type_of< cat::Category,			\
				  tag::What,				\
                                  From,					\
                                  typename From::What##_type >::ret


// with category being given
//   (warning: the client should write cat::category_name)

# define type_of(Category, From, What)					\
typename internal::f_get_type_of< Category,				\
				  tag::What,				\
                                  From,					\
                                  typename From::What##_type >::ret


// for images

# define oln_type_of(From, What) internal_type_of(image, From, What)


// for other categories

# define oln_grd_type_of(From, What)  internal_type_of(grid, From, What)
# define oln_pt_type_of(From, What)   internal_type_of(point, From, What)
# define oln_pts_type_of(From, What)  internal_type_of(points, From, What)
# define oln_info_type_of(From, What) internal_type_of(info, From, What)


#endif
