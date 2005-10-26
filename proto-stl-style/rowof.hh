#ifndef MINIOLN_ROWOF_HH
# define MINIOLN_ROWOF_HH

# include "1d.hh"
# include "2d.hh"
# include "misc.hh"


// . . . . . . . . . . . . . . . . . . . . . . . .  row_of(image2d-like)


template <class Info>
struct rowof_info : public Info
{
  typedef Info super;

  typedef_super(coord_type);
  typedef  oln_info_type_of(Info, fwd_piter)::to_1d  fwd_piter_type;
  typedef  oln_info_type_of(Info, bkd_piter)::to_1d  bkd_piter_type;

  rowof_info() : super()
  {}
  rowof_info(const Info& info)
    : super(info)
  {}
  
  const coord_type min_index() const { return this->Info::min_col(); }
  const coord_type max_index() const { return this->Info::max_col(); }

  const coord_type nindices() const { return this->Info::ncols(); }
  const coord_type border() const { return this->border(); }

private:
  void min_row() const;
  void max_row() const;
  void nrows() const;
  void min_col() const;
  void max_col() const;
  void ncols() const;
};

set_name_TC(rowof_info);


template <class I>
struct rowof_ : public abstract::morphed_image< const I,
						rowof_<I> >
{
  typedef rowof_<I> self_type;
  typedef abstract::morphed_image<const I, self_type> super;

  // inherited
  typedef_super(value_type);
  typedef_super(value_get_type);
  typedef_super(value_set_type);

  typedef rowof_info<oln_type_of(I, info)> info_type;
  typedef point1d point_type;

  // FIXME: (ugly; awful) the following typedef is wrong
  // we should have something like "I::to_1d"
  // e.g., if I is masked, the result also should be masked...
  typedef image1d<oln_type_of(I, value)> concrete_type;

  // FIXME: (ugly; awful) we need funs to ch grid of types
  typedef oln_type_of(I, fwd_piter)::to_1d fwd_piter_type;
  typedef oln_type_of(I, bkd_piter)::to_1d bkd_piter_type;


  // attr
  coord row_;

  // ctor
  rowof_(const abstract::image<I>& ima, // IDEA: relax
	 coord row)
    : super(ima),
      row_(row)
  {}

  const bbox1d& bbox() const
  {
    // FIXME: bad code
    info_type tmp(this->image_.info());
    return tmp.bbox();
  }
  
  const abstract_info<info_type> info() const
  {
    // FIXME: bad code
    info_type tmp(this->image_.info());
    return tmp;
  }

  bool has(const point_type& p) const
  {
    point2d p2d(row_, p.index());
    return this->image_.has(p2d);
  }

  bool has_large(const point_type& p) const
  {
    point2d p2d(row_, p.index());
    return this->image_.has_large(p2d);
  }

  const value_get_type operator[](const point_type& p) const
  {
    point2d p2d(row_, p.index());
    return this->image_[p2d];
  }

  value_set_type& operator[](const point_type& p)
  {    
    point2d p2d(row_, p.index());
    return this->image_[p2d];
  }

};

set_name_TC(rowof_);


template <class I, class C>
rowof_<I> rowof(const abstract::image<I>& input,
		const C& row)
{
  rowof_<I> tmp(input, row);
  return tmp;
}


#endif
