#ifndef PROTO_OLN_CORE_ABSTRACT_SIZE_HH
# define PROTO_OLN_CORE_ABSTRACT_SIZE_HH

# include <mlc/any.hh>

namespace oln {

  namespace abstract {

    template <typename E>
    struct size : public mlc::any__best_memory<E>
    {

      unsigned long npoints() const
      {
	return this->exact().impl_npoints();
      }

      template <typename Ep>
      bool operator==(const size<Ep>& rhs) const
      {
	return this->exact().impl_eq(rhs.exact());
      }

      template <typename Ep>
      bool operator!=(const size<Ep>& rhs) const
      {
	return not this->operator==(rhs);
      }

    protected:
      size() {}
    };

  } // end of namespace abstract

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_ABSTRACT_SIZE_HH
