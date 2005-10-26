#ifndef MINIOLN_1D_FWD_DECLS_HH
# define MINIOLN_1D_FWD_DECLS_HH

# include "misc.hh"


struct coord;

template <typename C> struct grid1d_;
template <typename C> struct point1d_;
template <typename C> struct dpoint1d_;
template <typename C> struct fwd_piter1d_;
template <typename C> struct bkd_piter1d_;

set_name_TC(grid1d_);
set_name_TC(point1d_);
set_name_TC(dpoint1d_);
set_name_TC(fwd_piter1d_);
set_name_TC(bkd_piter1d_);

typedef grid1d_<coord>   grid1d;
typedef point1d_<coord>  point1d;
typedef point1d_<float>  fpoint1d;
typedef dpoint1d_<coord> dpoint1d;
typedef dpoint1d_<float> fdpoint1d;
typedef fwd_piter1d_<coord> fwd_piter1d;
typedef bkd_piter1d_<coord> bkd_piter1d;

set_name(grid1d);
set_name(point1d);
set_name(fpoint1d);
set_name(dpoint1d);
set_name(fdpoint1d);
set_name(fwd_piter1d);
set_name(bkd_piter1d);



// generic classes

template <class G> struct size_gen_;
template <typename P> struct bbox_;
template <typename DP> struct neighb_gen_;
template <class P> struct fwd_niter_gen_;
template <class P> struct bkd_niter_gen_;

typedef size_gen_<grid1d> size1d;
typedef bbox_<point1d> bbox1d;
typedef neighb_gen_<dpoint1d> neighb1d;
typedef fwd_niter_gen_<point1d> fwd_niter1d;
typedef bkd_niter_gen_<point1d> bkd_niter1d;

set_name(size1d);
set_name(bbox1d);
set_name(neighb1d);
set_name(fwd_niter1d);
set_name(bkd_niter1d);


#endif
