#ifndef OLENA_CORE_ABSTRACT_DATA_STORAGE_HH
# define OLENA_CORE_ABSTRACT_DATA_STORAGE_HH

# include <mlc/any.hh>

# include <oln/core/macros.hh>

namespace oln {

  namespace abstract {

    template <typename E>
    struct data_storage : public mlc::any__best_speed<E>
    {
      // abstract methods

      bool has_data() const
      {
	return this->exact().impl_has_data();
      }

      void clear_data()
      {
	this->exact().impl_clear_data();
	postcondition(this->has_data() == false);
      }

      const oln_size_type(E)& size() const
      {
	return this->exact().impl_size(this->s);
      }

      void resize(const oln_size_type(E)& s)
      {
	this->exact().impl_resize(s);
      }

      unsigned long npoints() const
      {
	precondition(this->has_data());
	return this->exact().impl_npoints();
      }

      bool hold(const oln_point_type(E)& p) const
      {
	precondition(this->has_data());
	return this->exact().impl_hold(p);
      }

      const oln_data_type(E) get(const oln_point_type(E)& p) const
      {
	precondition(this->hold_large(p));
	return this->exact().impl_get(p);
      }

      void set(const oln_point_type(E)& p,
	       const oln_data_type(E)& v)
      {
	precondition(this->hold_large(p));
	this->exact().impl_set(p, v);
      }

      void set_data(const oln_data_type(E)& v)
      {
	precondition(this->has_data());
	this->exact().impl_set_data(v);
      }

      // polymorphic method with default

      bool hold_large(const oln_point_type(E)& p) const
      {
	precondition(this->has_data());
	return this->exact().impl_hold_large(p);
      }

      bool impl_hold_large(const oln_point_type(E)& p) const
      {
	return this->exact().impl_hold(p);
      }

    protected:
      data_storage() {}
    };

  } // end of namespace abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_DATA_STORAGE_HH
