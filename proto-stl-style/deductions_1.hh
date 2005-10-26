#ifndef MINIOLN_DEDUCTIONS_1_HH
# define MINIOLN_DEDUCTIONS_1_HH

# include "mlc.hh"
# include "abstract.hh"

# include "1d_fwd_decls.hh"
# include "2d_fwd_decls.hh"


// fwd decls
// (to avoid file inclusions)




// -> fwd_piter

# define fwd_piter_from(T) typename def_fwd_piter_from_<T>::ret

template <class T>
struct def_fwd_piter_from_
{
  typedef mlc::undefined ret;
};

template <> struct def_fwd_piter_from_< point1d > { typedef fwd_piter1d ret; };
template <> struct def_fwd_piter_from_< point2d > { typedef fwd_piter2d ret; };


// -> bkd_piter

# define bkd_piter_from(T) typename def_bkd_piter_from_<T>::ret

template <class T>
struct def_bkd_piter_from_
{
  typedef mlc::undefined ret;
};

template <> struct def_bkd_piter_from_< point1d > { typedef bkd_piter1d ret; };
template <> struct def_bkd_piter_from_< point2d > { typedef bkd_piter2d ret; };



#endif
