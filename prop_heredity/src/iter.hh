#ifndef PROTO_ITER_HH
# define PROTO_ITER_HH

# include "dim2d.hh"

# define for_all(P) for(p.begin(); ! p.end(); ++p)


namespace abstract {

  template <typename E>
  struct iter : public mlc::any__best_memory<E>
  {
    operator Point_type(E)() const {
      return p_;
    }
    void begin() {
      this->exact().begin_impl();
    }
    bool end() const {
      return this->exact().end_impl();
    }
    void operator++() {
      return this->exact().inc_impl();
    }
  protected:
    iter() {}
    Point_type(E) p_;
  };

} // end of namespace abstract



// iter2d


struct iter2d;

template <>
struct props <iter2d> : public default_props<dim2d>
{
  typedef dim2d dim_type;
};

struct iter2d : public abstract::iter< iter2d >
{
  iter2d() {}

  iter2d(const image2d_size& size) :
    size(size)
  {}
  void begin_impl() {
    p_.row() = p_.col() = 0;
  }
  bool end() const {
    return p_.row() == size.nrows();
  }

  // IDEA: T::op= calls ::assign_impl(T,Trhs) and T::op=(Trhs) is renammed assign

  void inc_impl() {
    ++p_.col();
    if (p_.col() == size.ncols())
      {
	p_.col() = 0;
	++p_.row();
      }
  }
protected:
  image2d_size size;
};




#endif // ndef PROTO_ITER_HH
