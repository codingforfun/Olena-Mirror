#ifndef MINIOLN_2D_FWD_DECLS_HH
# define MINIOLN_2D_FWD_DECLS_HH

# include "misc.hh"


struct coord;

template <typename C> struct grid2d_;
template <typename C> struct grid2d_hex_;
template <typename C> struct point2d_;
template <typename C> struct dpoint2d_;
template <typename C> struct fwd_piter2d_;
template <typename C> struct bkd_piter2d_;

set_name_TC(grid2d_);
set_name_TC(grid2d_hex_);
set_name_TC(point2d_);
set_name_TC(dpoint2d_);
set_name_TC(fwd_piter2d_);
set_name_TC(bkd_piter2d_);

typedef grid2d_<coord> grid2d;
typedef grid2d_hex_<coord> grid2d_hex; // FIXME: not 'coord' here!
typedef point2d_<coord>  point2d;
typedef point2d_<float> fpoint2d;
typedef dpoint2d_<coord> dpoint2d;
typedef dpoint2d_<float> fdpoint2d;
typedef fwd_piter2d_<coord> fwd_piter2d;
typedef bkd_piter2d_<coord> bkd_piter2d;

set_name(grid2d);
set_name(grid2d_hex);
set_name(point2d);
set_name(fpoint2d);
set_name(dpoint2d);
set_name(fdpoint2d);
set_name(fwd_piter2d);
set_name(bkd_piter2d);



// generic classes

template <class G> struct size_gen_;
template <typename P> struct bbox_;
template <typename DP> struct neighb_gen_;
template <class P> struct fwd_niter_gen_;
template <class P> struct bkd_niter_gen_;

typedef size_gen_<grid2d> size2d;
typedef bbox_<point2d> bbox2d;
typedef neighb_gen_<dpoint2d> neighb2d;
typedef fwd_niter_gen_<point2d> fwd_niter2d;
typedef bkd_niter_gen_<point2d> bkd_niter2d;

set_name(size2d);
set_name(bbox2d);
set_name(neighb2d);
set_name(fwd_niter2d);
set_name(bkd_niter2d);


#endif
