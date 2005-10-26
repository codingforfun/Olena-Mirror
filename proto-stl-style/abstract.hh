#ifndef MINIOLN_ABSTRACT_HH
# define MINIOLN_ABSTRACT_HH

# include <iostream>
# include "misc.hh"
# include "type_of.hh"


// The "::info() const" methods have to return
// an "abstract_info<info_type>" object so that we can constrain
// templated method argument and thus have overloading.

// Another way to provide such features is SCOOP inheritance
// (a concrete info class, say I, derives from abstract::info<I>);
// however, it does *not* work since we have mixins of
// info types.  For instance, "size2d : abstract::info<size2d>"
// and "mixin<size2d> : size2d" lead to "mixin<size2d>"
// inherits from "abstract::info<size2d>" which is wrong!!!
// the proper inheritance should be in that case
// abstract::info< mixin<size2d> >...

// so we turn to "constraining by composition" with the following
// class:

template <class Info>
struct abstract_info
{
  abstract_info(const Info& info) : info_(info) { info_.ensure_info(); }
  const Info& exact() const { return info_; }
protected:
  const Info info_; // cpy
};

set_name_TC(abstract_info);

template <typename Info>
std::ostream&
operator<<(std::ostream& ostr,
	   const abstract_info<Info>& info)
{
  return ostr << "ai." << info.exact();
}




// abstractions



// ------------------------------------------------------  coord

namespace abstract {

  struct coord
  {
    // TODO: interface?
  protected:
    coord() {}
  };

} // abstract

set_name(abstract::coord);


// ------------------------------------------------------  grid

namespace abstract {

  template <class G>
  struct grid
  {
    // basic properties
    typedef mlc::undefined dim_type;
    typedef mlc::undefined point_type;
    // end of basic properties

    // derived property
    typedef mlc::undefined dpoint_type;
    typedef mlc::undefined coord_type;
    // end of derived property

  protected:
    grid() {}
  };

} // abstract

set_name_TC(abstract::grid);


// grid::dpoint_type <- grid::point_type::dpoint_type
template <class G>
struct set_type_of < cat::grid, tag::dpoint, G >
{
  typedef oln_grd_type_of(G, point) P;
  typedef oln_pt_type_of(P, dpoint) ret;
};

// grid::coord_type <- grid::point_type::coord_type
template <class G>
struct set_type_of < cat::grid, tag::coord, G >
{
  typedef oln_grd_type_of(G, point) P;
  typedef oln_pt_type_of(P, coord) ret;
};


# define oln_grd_name_of(What) name<oln_grd_type_of(G, What)>::of()

template <class G>
void echo(const abstract::grid<G>&,
	  std::ostream& ostr = std::cout)
{
  ostr << name<G>::of() << ":" << std::endl
       << "  dim_type:    " << oln_grd_name_of(dim)    << std::endl
       << "  point_type:  " << oln_grd_name_of(point)  << std::endl
       << "  dpoint_type: " << oln_grd_name_of(dpoint) << std::endl
       << "  coord_type:  " << oln_grd_name_of(coord)  << std::endl;
}



// ------------------------------------------------------  point

namespace abstract {

  struct point
  {
    // basic properties:
    typedef mlc::undefined coord_type;
    typedef mlc::undefined dpoint_type;
    // end of basic properties

    // FIXME: how to check that (coord_type == dpoint_type::coord_type)?

    void ensure_point() const {}
    typedef point self_type;

    bool operator==(const self_type&) const;
    bool operator< (const self_type&) const;

    // this routine is temporary; it is equivalent to
    // "p.coord<0>()" for we have not yet a meta-vector as
    // attribute
    coord_type& coord_0();

    const coord_type coord_(unsigned i) const;

  protected:
    point() {}
  };

} // abstract

/*
  std::ostream& operator<<(std::ostream&, const point&);
*/

set_name(abstract::point);


// ------------------------------------------------------  dpoint

namespace abstract {

  struct dpoint
  {
    void ensure_dpoint() const {}
    typedef dpoint self_type;

    typedef mlc::undefined coord_type; // ok?
    typedef mlc::undefined point_type;

    bool operator==(const self_type&) const;
    bool operator< (const self_type&) const;

    self_type operator-() const;
    // TODO: ...

  protected:
    dpoint() {}
  };

} // abstract

/*
  std::ostream& operator<<(std::ostream&, const dpoint&);
  point  operator+(const point&, const dpoint&);
  point  operator-(const point&, const dpoint&);
  dpoint operator-(const point&, const  point&);
*/

set_name(abstract::dpoint);


// ------------------------------------------------------  *iter

namespace abstract {

  struct iter
  {
    void start();
    bool is_valid() const;
    void invalidate();
    void next();

  protected:
    iter() {}
  };

} // abstract

set_name(abstract::iter);


// ------------------------------------------------------  piter


namespace abstract {

  struct piter : public iter
  {
    void ensure_piter() const {}

    typedef mlc::undefined to_1d;

    typedef mlc::undefined point_type;
    operator point_type() const;

  protected:
    piter() {}
  };

} // abstract

set_name(abstract::piter);



// for all type T,

//   T::piter_type <- T::fwd_piter_type

template <class category, class T>
struct set_type_of < category, tag::piter, T >
{
  typedef type_of(category, T, fwd_piter) ret;
};

//   and
//   T::niter_type <- T::fwd_niter_type

template <class category, class T>
struct set_type_of < category, tag::niter, T >
{
  typedef type_of(category, T, fwd_niter) ret;
};



// ------------------------------------------------------  niter


namespace abstract {

  struct niter : public iter
  {
    void ensure_niter() const {}

    typedef mlc::undefined nbh_type;
    typedef mlc::undefined point_type;

    const nbh_type nbh() const;
    operator point_type() const;
    void center_at(const point_type&);

  protected:
    niter() {}
  };

} // abstract

set_name(abstract::niter);



// ------------------------------------------------------  points


namespace abstract {

  // this class is just like a set of points
  // as one can see, it is merely an image...

  template <class E>
  struct points : public mlc::any<E>
  {
    // basic properties
    typedef mlc::undefined point_type;
    typedef mlc::undefined fwd_piter_type;
    typedef mlc::undefined bkd_piter_type;
    typedef mlc::undefined piter_type;
    // end of basic properties

    const point_type& operator[](unsigned i) const;
    unsigned card() const;

  protected:
    points() {}
  };

} // abstract

set_name_TC(abstract::points);


// fwd decl:
template <class P> struct bbox_;



// ------------------------------------------------------  nbh

namespace abstract {

  template <class E>
  struct nbh : public mlc::any<E>
  {
    // TODO: interface?
  protected:
    nbh() {}
  };

} // abstract

set_name_TC(abstract::nbh);



// ------------------------------------------------------  info

namespace abstract {

  struct info
  {
    void ensure_info() const {}

    // basic properties
    typedef mlc::undefined grid_type;
    typedef mlc::undefined nbh_type;
    // iterators below are either defined in info class or function-defined
    typedef mlc::undefined fwd_piter_type;
    typedef mlc::undefined bkd_piter_type;
    typedef mlc::undefined fwd_niter_type;
    typedef mlc::undefined bkd_niter_type;
    // end of basic properties

    // derived properties
    typedef mlc::undefined coord_type;
    typedef mlc::undefined point_type;
    // end of derived properties

    const bbox_<point_type> bbox() const;

    // TODO: ...
  protected:
    info() {}
  };

} // abstract

set_name(abstract::info);


// info::coord  <-  info::grid::coord
template <class I>
struct set_type_of < cat::info, tag::coord, I >
{
  typedef oln_info_type_of(I, grid) G;
  typedef oln_grd_type_of(G, coord) ret;
};

// info::point  <-  info::grid::point
template <class I>
struct set_type_of < cat::info, tag::point, I >
{
  typedef oln_info_type_of(I, grid) G;
  typedef oln_grd_type_of(G, point) ret;
};

template <class I> struct set_fwd_piter_of { typedef mlc::undefined ret; };
template <class I> struct set_bkd_piter_of { typedef mlc::undefined ret; };
template <class I> struct set_fwd_niter_of { typedef mlc::undefined ret; };
template <class I> struct set_bkd_niter_of { typedef mlc::undefined ret; };

// info::<iter>  <-  set_<iter>_of(info)
template <class I>
struct set_type_of < cat::info, tag::fwd_piter, I > : public set_fwd_piter_of <I>
{};
template <class I>
struct set_type_of < cat::info, tag::bkd_piter, I > : public set_bkd_piter_of <I>
{};
template <class I>
struct set_type_of < cat::info, tag::fwd_niter, I > : public set_fwd_niter_of <I>
{};
template <class I>
struct set_type_of < cat::info, tag::bkd_niter, I > : public set_bkd_niter_of <I>
{};



// ------------------------------------------------------  image

namespace abstract {

  template <class I>
  struct image : public mlc::any<I>
  {

    // basic properties
    typedef mlc::undefined info_type;

    typedef mlc::undefined value_type;
    typedef mlc::undefined value_get_type;
    typedef mlc::undefined value_set_type;

    typedef mlc::undefined concrete_type;
    // end of basic properties


    // derived properties
    //      from info_type:
    typedef mlc::undefined coord_type;
    typedef mlc::undefined point_type;
    typedef mlc::undefined fwd_piter_type;
    typedef mlc::undefined bkd_piter_type;
    typedef mlc::undefined nbh_type;
    typedef mlc::undefined fwd_niter_type;
    typedef mlc::undefined bkd_niter_type;
    //      from fwd_*iter_type:
    typedef mlc::undefined piter_type;
    typedef mlc::undefined niter_type;
    // end of derived properties


    const abstract_info<info_type> info() const;
    const bbox_<point_type>& bbox() const;

    bool has(const point_type&) const;
    bool has_large(const point_type&) const;

    // TODO: check that sigs conform to the following ones
    const value_get_type  operator[](const point_type&) const;
          value_set_type& operator[](const point_type&);

  protected:
    image() {}
  };

} // abstract

set_name_TC(abstract::image);


// image::coord  <-  image::info::coord
template <class I>
struct set_type_of < cat::image, tag::coord, I >
{
  typedef oln_type_of(I, info) info_t;
  typedef oln_info_type_of(info_t, coord) ret;
};

// image::point  <-  image::info::point
template <class I>
struct set_type_of < cat::image, tag::point, I >
{
  typedef oln_type_of(I, info) info_t;
  typedef oln_info_type_of(info_t, point) ret;
};

// image::fwd_piter  <-  image::info::fwd_piter
template <class I>
struct set_type_of < cat::image, tag::fwd_piter, I >
{
  typedef oln_type_of(I, info) info_t;
  typedef oln_info_type_of(info_t, fwd_piter) ret;
};

// image::bkd_piter  <-  image::info::bkd_piter
template <class I>
struct set_type_of < cat::image, tag::bkd_piter, I >
{
  typedef oln_type_of(I, info) info_t;
  typedef oln_info_type_of(info_t, bkd_piter) ret;
};

// image::nbh  <-  image::info::nbh
template <class I>
struct set_type_of < cat::image, tag::nbh, I >
{
  typedef oln_type_of(I, info) info_t;
  typedef oln_info_type_of(info_t, nbh) ret;
};

// image::fwd_niter  <-  image::info::fwd_niter
template <class I>
struct set_type_of < cat::image, tag::fwd_niter, I >
{
  typedef oln_type_of(I, info) info_t;
  typedef oln_info_type_of(info_t, fwd_niter) ret;
};

// image::bkd_niter  <-  image::info::bkd_niter
template <class I>
struct set_type_of < cat::image, tag::bkd_niter, I >
{
  typedef oln_type_of(I, info) info_t;
  typedef oln_info_type_of(info_t, bkd_niter) ret;
};



// ----------------------------------------  from entry_image


namespace abstract
{

  template <class I>
  struct entry_image : public image<I>
  {
  protected:
    entry_image() {}
  };

  template <class I>
  struct primary_image : public entry_image<I>
  {
  protected:
    primary_image() {}
  };


  template <class I>
  struct data_image : public primary_image<I>
  {
    typedef I concrete_type;
  protected:
    data_image() {}
  };


  template <class I>
  struct lightweight_image : public primary_image<I>
  {
  protected:
    lightweight_image() {}
  };


  template <class I, class E>
  struct delegate_image : public entry_image<E>
  {
    typedef delegate_image<I, E> self_type;

    // specific NEW typedef
    typedef I delegated_type;


    // basic properties:
    typedef oln_type_of(I, info) info_type;
    typedef oln_type_of(I, value)     value_type;
    typedef oln_type_of(I, value_get) value_get_type;
    typedef oln_type_of(I, value_set) value_set_type;
    typedef oln_type_of(I, concrete) concrete_type;
    // end of basic properties

    // derived property in use:
    typedef oln_type_of(I, point) point_type;


    // methods

    const abstract_info<info_type> info() const { return this->image_.info(); }
    const bbox_<point_type>& bbox() const       { return this->image_.bbox(); }

    bool has(const point_type& p) const       { return this->image_.has(p); }
    bool has_large(const point_type& p) const { return this->image_.has_large(p); }

    const value_get_type  operator[](const point_type& p) const
    {
      precondition(has_large(p));
      return this->image_[p];
    }

    value_set_type& operator[](const point_type& p)
    {
      precondition(has_large(p));
      return this->image_[p];
    }

  protected:

    // ctors

    delegate_image() :
      image_()
    {}

    template <class II>
    delegate_image(const abstract::image<II>& ima)
      : image_(ima.exact()) // both types can be different
    {}

    delegate_image(const self_type& rhs)
      : image_(rhs.image_)
    {}

    // we can create an delegate_image with *no* initial image
    // e.g., in:
    //   template <class I>
    //   I mean_over_nbh(const I& input)
    //   {
    //     I output(input.info());
    //     ...
    //   }
    // I is for instance with_nbh_<image2d.., neighb2d..>
    // output.image_ is created from scratch
    template <class Info>
    delegate_image(const abstract_info<Info>& info)
      : image_(info)
    {}


    // the only attribute:

    I image_; // important: we definitly want a copy as attribute here
    // to mimic what we have in proto-1.0 (images are somehow pure!)
  };


  template <class I, class E>
  struct decorated_image : public delegate_image<I, E>
  {
  protected:
    typedef decorated_image<I,E> self_type;
    typedef delegate_image<I,E>  super;
                          decorated_image() {}
    template <class II>   decorated_image(const abstract::image<II>& ima)  : super(ima)  {}
    template <class Info> decorated_image(const abstract_info<Info>& info) : super(info) {}
  };


  template <class I, class E>
  struct extended_image : public delegate_image<I, E>
  {
  protected:
    typedef extended_image<I,E> self_type;
    typedef delegate_image<I,E> super;
                          extended_image() {}
    template <class II>   extended_image(const abstract::image<II>& ima)  : super(ima)  {}
    template <class Info> extended_image(const abstract_info<Info>& info) : super(info) {}
  };


  template <class I, class E>
  struct morphed_image : public delegate_image<I, E>
  {
  protected:
    typedef morphed_image<I,E> self_type;
    typedef delegate_image<I,E> super;
                          morphed_image() {}
    template <class II>   morphed_image(const abstract::image<II>& ima)  : super(ima)  {}
    template <class Info> morphed_image(const abstract_info<Info>& info) : super(info) {}
  };


} // end of namespace abstraction

set_name_TC(abstract::primary_image);
set_name_TC(abstract::data_image);
set_name_TC(abstract::lightweight_image);
set_name_TCC(abstract::delegate_image);
set_name_TCC(abstract::decorated_image);
set_name_TCC(abstract::extended_image);
set_name_TCC(abstract::morphed_image);


// echo

# define oln_name_of(What) name<oln_type_of(I, What)>::of()

template <class I>
void echo(const abstract::image<I>&,
	  std::ostream& ostr = std::cout)
{
  ostr << name<I>::of() << ":" << std::endl
       << "  concrete_type:  " << oln_name_of(concrete)  << std::endl
       << "  info_type:      " << oln_name_of(info)      << std::endl
       << "  point_type:     " << oln_name_of(point)     << std::endl
       << "  value_type:     " << oln_name_of(value)     << std::endl
       << "  value_get_type: " << oln_name_of(value_get) << std::endl
       << "  value_set_type: " << oln_name_of(value_set) << std::endl
       << "  piter_type:     " << oln_name_of(piter)     << std::endl
       << "  fwd_piter_type: " << oln_name_of(fwd_piter) << std::endl
       << "  bkd_piter_type: " << oln_name_of(bkd_piter) << std::endl
       << "  nbh_type:       " << oln_name_of(nbh)       << std::endl
       << "  niter_type:     " << oln_name_of(niter)     << std::endl
       << "  fwd_niter_type: " << oln_name_of(fwd_niter) << std::endl
       << "  bkd_niter_type: " << oln_name_of(bkd_niter) << std::endl;
}


template <class T>
void echoln(const T& input,
	    std::ostream& ostr = std::cout)
{
  echo(input, ostr);
  ostr << std::endl;
}



// macros

# define for_all_p(p) \
  for(p.ensure_piter(), p.start(); p.is_valid(); p.next())

# define for_all_n_of_p(n, p) \
  for(n.ensure_niter(), n.center_at(p), n.start(); n.is_valid(); n.next())


#endif
