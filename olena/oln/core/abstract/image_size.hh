#ifndef PROTO_OLN_CORE_ABSTRACT_SIZE_HH
# define PROTO_OLN_CORE_ABSTRACT_SIZE_HH


namespace oln {

  namespace abstract {

    template <typename E>
    struct image_size : public mlc::any__best_memory<E>
    {
      bool operator==(const image_size& rhs) const {
	return this->exact().op_eq_impl(rhs.exact());
      }
      bool operator!=(const image_size& rhs) const {
	return ! this->operator==(rhs);
      }
    protected:
      image_size() {}

    protected:
      coord	data_[oln_dim_value(E)];
    };

  } // end of namespace abstract

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_ABSTRACT_SIZE_HH
