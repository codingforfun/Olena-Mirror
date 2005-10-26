#ifndef MINIOLN_2D_HH
# define MINIOLN_2D_HH

# include "misc.hh"
# include "mlc.hh"
# include "abstract.hh"
# include "2d_fwd_decls.hh"
# include "gen.hh"
# include "points.hh"




template <typename C>
struct grid2d_ : public abstract::grid < grid2d_<C> >
{
  typedef mlc::value<unsigned, 2> dim_type;
  typedef point2d_<C> point_type;
protected:
  grid2d_() {}
};


template <typename C>
struct grid2d_hex_ : public abstract::grid < grid2d_hex_<C> >
{
  typedef mlc::value<unsigned, 2> dim_type;
  typedef point2d_<C> point_type;
protected:
  grid2d_hex_() {}
};





template <typename C>
struct point2d_ : public abstract::point
{
  typedef point2d_<C> self_type;

  typedef mlc::value<unsigned, 2> dim_type;
  typedef C coord_type;
  typedef dpoint2d_<coord> dpoint_type;

  // C is a coord type; we factor for
  // point2d_<float>, point2d_<coord_hexa>, point2d_<very_large_int>

  point2d_(const C& row, const C& col)
    : row_(row), col_(col)
  {}

  point2d_()
  {}

  bool operator==(const self_type& rhs) const
  {
    return row_ == rhs.row_ and col_ == rhs.col_;
  }

  bool operator< (const self_type& rhs) const
  {
    return row_ < rhs.row_ or (row_ == rhs.row_ and col_ < rhs.col_);
  }

  C& row() { return row_; }
  C& col() { return col_; }

  const C row() const { return row_; }
  const C col() const { return col_; }

  C& coord_0() { return col_; } // temp method
  const C coord_1() const { return row_; } // temp method

  const C coord_(unsigned i) const
  {
    precondition(i < 2);
    return i == 0 ? col_ : row_;
  }

protected:
  C row_, col_;
};

const point2d mk_point2d(coord row, coord col)
{
  point2d tmp(row, col);
  return tmp;
}

bbox2d mk_bbox2d(coord min_row, coord min_col,
		 coord max_row, coord max_col)
{
  bbox2d tmp(mk_point2d(min_row, min_col),
	     mk_point2d(max_row, max_col));
  return tmp;
}

template <typename C>
std::ostream& operator<<(std::ostream& ostr, const point2d_<C>& p)
{
  return ostr << "(" << p.row() << "," << p.col() << ")";
}



template <typename C>
struct dpoint2d_ : public abstract::dpoint
{
  typedef dpoint2d_<C> self_type;

  typedef C coord_type;
  typedef point2d_<C> point_type;

  dpoint2d_(const C& row, const C& col)
    : row_(row), col_(col)
  {}

  dpoint2d_()
  {}

  bool operator==(const self_type& rhs) const {
    return row_ = rhs.row_ and col_ = rhs.col_;
  }

  bool operator< (const self_type& rhs) const
  {
    return row_ < rhs.row_ or (row_ == rhs.row_ and col_ < rhs.col_);
  }

  self_type operator-() const {
    self_type tmp(-row_, -col_);
    return tmp;
  }

  C& row() { return row_; }
  C& col() { return col_; }

  const C row() const { return row_; }
  const C col() const { return col_; }

protected:
  C row_, col_;
};

const dpoint2d mk_dpoint2d(coord drow, coord dcol)
{
  dpoint2d tmp(drow, dcol);
  return tmp;
}

template <typename C>
std::ostream& operator<<(std::ostream& ostr, const dpoint2d_<C>& dp)
{
  return ostr << "(" << dp.row() << "," << dp.col() << ")";
}

template <typename C>
point2d_<C> operator+(const point2d_<C>& p, const dpoint2d_<C>& dp) {
  point2d_<C> tmp(p.row() + dp.row(), p.col() + dp.col());
  return tmp;
}

template <typename C>
point2d_<C> operator-(const point2d_<C>& p, const dpoint2d_<C>& dp) {
  point2d_<C> tmp(p.row() - dp.row(), p.col() - dp.col());
  return tmp;
}

template <typename C>
dpoint2d_<C> operator-(const point2d_<C>& p1, const point2d_<C>& p2) {
  dpoint2d_<C> tmp(p1.row() - p2.row(), p1.col() - p2.col());
  return tmp;
}



template <typename C>
struct size_gen_< grid2d_<C> > : public abstract::info
{
  typedef size_gen_< grid2d_<C> > self_type;


  // basic properties:
  typedef grid2d_<C> grid_type;
  typedef fwd_piter2d fwd_piter_type;
  typedef bkd_piter2d bkd_piter_type;
  typedef mlc::no_type nbh_type;
  typedef mlc::no_type fwd_niter_type;
  typedef mlc::no_type bkd_niter_type;

  // derived properties:
  typedef C coord_type;
  typedef point2d_<C> point_type;


  size_gen_()
    : nrows_(0),
      ncols_(0),
      border_(0)
  {}

  size_gen_(const coord_type& nrows,
	    const coord_type& ncols,
	    const coord_type& border = 1)
    : nrows_(nrows),
      ncols_(ncols),
      border_(border)
  {}

  template <typename Info>
  size_gen_(const abstract_info<Info>& info)
    : nrows_(info.exact().nrows()),
      ncols_(info.exact().ncols()),
      border_(info.exact().border())
  {}

  const coord_type min_row() const { return 0; }
  const coord_type max_row() const { return nrows_ - 1; }

  const coord_type min_col() const { return 0; }
  const coord_type max_col() const { return ncols_ - 1; }

  const bbox_<point_type> bbox() const
  {
    bbox_<point_type> tmp(point_type(0, 0),
			  point_type(nrows_ - 1, ncols_ - 1));
    return tmp;
  }

  bool has(const point_type& p) const
  {
    return p.col() >= min_col() and p.col() <= max_col() and
      p.row() >= min_row() and p.row() <= max_row();
  }




  const coord_type nrows() const { return nrows_; }
  const coord_type ncols() const { return ncols_; }
  const coord_type border() const { return border_; }

protected:
  coord_type nrows_, ncols_, border_;
};

template <typename C>
std::ostream& operator<<(std::ostream& ostr, const size_gen_<grid2d_<C> >& s)
{
  return ostr << "(nrows=" << s.nrows() << ", ncols=" << s.ncols()
	      << ", border=" << s.border() << ")";
}



template <typename C>
struct fwd_piter2d_ : public abstract::piter
{
  typedef fwd_piter2d_<C> self_type;

  typedef C coord_type;
  typedef point2d_<C> point_type;

  // function -> 1d
  typedef fwd_piter1d_<C> to_1d;

  template <typename Info>
  fwd_piter2d_(const abstract_info<Info>& info)
    : bb_(info.exact().bbox())
  {
    invalidate();
  }

  fwd_piter2d_(const bbox_<point_type>& bb)
    : bb_(bb)
  {
    invalidate();
  }

  bool is_valid() const {
    return p_.row() <= bb_.max().row();
  }
  void invalidate() {
    p_.row() = bb_.max().row() + 1;
    postcondition(not is_valid());
  }
  void start() {
    p_.row() = bb_.min().row();
    p_.col() = bb_.min().col();
    if (not is_valid())
      {
	std::cerr << "oops!" << std::endl;
      }
    postcondition(is_valid());
  }
  void next() {
    precondition(is_valid());
    ++p_.col();
    if (p_.col() <= bb_.max().col())
      return;
    p_.col() = bb_.min().col();
    ++p_.row();
  }
  operator point_type() const {
    precondition(is_valid());
    return p_;
  }

  friend
  std::ostream& operator<<(std::ostream& ostr, const self_type& p)
  {
    return ostr << "(" << p.bb_ << ", @" << p.p_ << ")";
  }

protected:
  bbox_<point_type> bb_;
  point_type p_;
};


template <typename C>
struct bkd_piter2d_ : public abstract::piter
{
  typedef fwd_piter2d self_type;

  typedef C coord_type;
  typedef point2d_<C> point_type;

  // function -> 1d
  typedef bkd_piter1d to_1d;

  template <typename Info>
  bkd_piter2d_(const abstract_info<Info>& info)
    : bb_(info.exact().bbox())
  {
    invalidate();
  }

  bkd_piter2d_(const bbox_<point_type>& bb)
    : bb_(bb)
  {
    invalidate();
  }

  bool is_valid() const {
    return p_.row() >= bb_.min().row();
  }

  void invalidate() {
    p_.row() = bb_.min().row() - 1;
    postcondition(not is_valid());
  }

  void start() {
    p_.row() = bb_.max().row();
    p_.col() = bb_.max().col();
  }

  void next() {
    precondition(is_valid());
    --p_.col();
    if (p_.col() >= bb_.min().col())
      return;
    p_.col() = bb_.max().col();
    --p_.row();
  }

  operator point_type() const {
    precondition(is_valid());
    return p_;
  }

protected:
  bbox_<point_type> bb_;
  point_type p_;
};





// . . . . . . . . . . . . . . . . . . . . . . . .  image2d


template <typename T>
struct image2d : public abstract::data_image< image2d<T> >
{
  typedef image2d<T> self_type;


  // basic properties
  typedef size2d  info_type;
  typedef T value_type;
  typedef T value_get_type;
  typedef T value_set_type;
  // end of basic properties

  // derived property in use:
  typedef point2d point_type;


  // interface:

  const bbox2d& bbox() const
  {
    return info_.bbox();
  }

  abstract_info<info_type> info() const
  {
    return info_;
  }

  bool has(const point_type& p) const
  {
    return p.row() >= 0 and p.col() >= 0
      and p.row() < info_.nrows() and p.col() < info_.ncols();
  }

  bool has_large(const point_type& p) const
  {
    return
      p.row() >= - info_.border()
      and p.col() >= - info_.border()
      and p.row() < info_.nrows() + info_.border()
      and p.col() < info_.ncols() + info_.border();
  }

  const value_get_type operator[](const point_type& p) const
  {
    precondition(has_large(p));
    return data_[(p.row() + info_.border()) * info_.ncols()
		 + (p.col() + info_.border())];
  }

  value_set_type& operator[](const point_type& p)
  {
    precondition(has_large(p));
    return data_[(p.row() + info_.border()) * info_.ncols()
		 + (p.col() + info_.border())];
  }


  // dedicated methods are evil!
  // TODO: explain why...

  const value_get_type operator()(coord row, coord col) const
  {
    point2d p(row, col);
    return this->operator[](p);
  }

  value_set_type& operator()(coord row, coord col)
  {
    point2d p(row, col);
    return this->operator[](p);
  }


  // classic equipment
  image2d() {
    data_ = 0; // no data is possible...
  }
  image2d(const image2d& rhs) {
    data_ = rhs.data_; // ...data is shared
    info_ = rhs.info_;
  }
  image2d& operator=(const image2d& rhs) {
    data_ = rhs.data_; // ...likewise
    info_ = rhs.info_;
    return *this;
  }

  // ctors

  image2d(const coord& nrows, const coord& ncols)
    : info_(nrows, ncols)
  {
    data_ = new T[(info_.nrows() + 2 * info_.border())
		  * (info_.ncols() + 2 * info_.border())];
  }

  template <typename Info>
  image2d(const abstract_info<Info>& info)
  {
    info_ = info_type(info.exact().nrows(),
		      info.exact().ncols(),
		      info.exact().border());
    data_ = new T[(info_.nrows() + 2 * info_.border())
		  * (info_.ncols() + 2 * info_.border())];
  }

  template <typename II>
  image2d(const abstract::image<II>& rhs)
  {
    std::cerr << "not yet impled!" << std::endl;
  }

  template <typename II>
  self_type& operator=(const abstract::image<II>& rhs)
  {
    std::cerr << "not yet impled!" << std::endl;
    return *this;
  }

  T* data_;
protected:
  info_type info_;
};


set_name_TC(image2d);



neighb2d c4 = neighb2d()
  .add(dpoint2d(-1, 0))
  .add(dpoint2d( 0,-1))
  .add(dpoint2d( 0, 1))
  .add(dpoint2d( 1, 0));

neighb2d c8 = neighb2d()
  .add(dpoint2d(-1,-1)).add(dpoint2d(-1, 0)).add(dpoint2d(-1, 1))
  .add(dpoint2d( 0,-1))                      .add(dpoint2d( 0, 1))
  .add(dpoint2d( 1,-1)).add(dpoint2d( 1, 0)).add(dpoint2d( 1, 1));


#endif
