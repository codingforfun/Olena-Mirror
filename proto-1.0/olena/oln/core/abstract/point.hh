#ifndef PROTO_OLN_CORE_ABSTRACT_POINT_HH
# define PROTO_OLN_CORE_ABSTRACT_POINT_HH

# include <mlc/any.hh>

namespace oln {

  namespace abstract {

    template <typename E>
    struct point : public mlc::any__best_memory<E>
    {
      bool operator==(const point& rhs) const
      {
	return this->exact().impl_eq(rhs.exact());
      }

      bool operator!=(const point& rhs) const
      {
	return not this->operator==(rhs);
      }

      bool operator<(const point& rhs) const
      {
	return this->exact().impl_less(rhs.exact());
      }

    protected:
      point() {}
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_ABSTRACT_POINT_HH
