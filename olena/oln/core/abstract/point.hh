#ifndef PROTO_OLN_CORE_ABSTRACT_POINT_HH
# define PROTO_OLN_CORE_ABSTRACT_POINT_HH

# include <oln/core/coord.hh>
# include <oln/core/macros.hh>

namespace oln {

  namespace abstract {

    template <typename E>
    struct point : public mlc::any__best_speed<E>
    {
      bool operator==(const point& rhs) const
      {
	return this->exact().op_eq_impl(rhs.exact());
      }

      bool operator!=(const point& rhs) const
      {
	return ! this->operator==(rhs);
      }

      bool operator<(const point& rhs) const
      {
	return this->exact().op_less_impl(rhs.exact());
      }

    protected:
      point() {}

    protected:
      coord	data_[oln_dim_value(E)];
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_ABSTRACT_POINT_HH
