#ifndef MINIOLN_WITH_LOG_HH
# define MINIOLN_WITH_LOG_HH

# include "misc.hh"
# include "mlc.hh"
# include "abstract.hh"
# include "value_box.hh"



// . . . . . . . . . . . . . . . . . . . . . . . .  with_log


// global variable

struct rw_counter
{
  unsigned nreads, nwrites;
  rw_counter()
    : nreads(0), nwrites(0)
  {}
} log_data;



template <class I>
struct with_log_ : public abstract::decorated_image< I, with_log_<I> >
{
  typedef with_log_<I> self_type;
  typedef abstract::decorated_image<I, self_type> super;

  typedef with_log_<oln_type_of(I, concrete)> concrete_type;

  typedef_super(value_type);
  typedef_super(point_type);
  typedef_super(value_get_type);

  typedef typename mlc::if_<  mlc::eq<oln_type_of(I, value_set), mlc::no_type>,
			      mlc::no_type,
			      value_box<self_type>  >::ret value_set_type;


  // only attr:
  value_box<self_type> vb_;


  // ctors

  with_log_()
    : super()
  {
    vb_.this_ = this;
  }

  with_log_(const self_type& rhs)
    : super(rhs)
  {
    vb_.this_ = this;
  }

  with_log_(const abstract::image<I>& ima)
    : super(ima)
  {
    vb_.this_ = this;
  }

  template <class Info>
  with_log_(const abstract_info<Info>& info)
    : super(info)
  {
    vb_.this_ = this;
  }

  // op[]

  const value_get_type operator[](const point_type& p) const
  {
    ++log_data.nreads;
    return this->image_[p];
  }

  value_set_type& operator[](const point_type& p)
  {
    vb_.p_ = p;
    return vb_;
  }

  template <class V>
  void set(const V& value)
  {
    ++log_data.nwrites;
    this->image_[vb_.p_] = value;
  }

  const value_type get() const
  {
    ++log_data.nreads;
    return this->image_[vb_.p_];
  }

};

set_name_TC(with_log_);



// FIXME: such as below, we should always (?) provide procs that
// instantiate non-primary images with two versions?

template <class I>
with_log_<I> with_log(abstract::image<I>& input)
{
  with_log_<I> tmp(input);
  return tmp;
}

template <class I>
with_log_<const I> with_log(const abstract::image<I>& input)
{
  with_log_<const I> tmp(input);
  return tmp;
}


#endif
