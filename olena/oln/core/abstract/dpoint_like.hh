#ifndef PROTO_OLN_CORE_ABSTRACT_DPOINT_LIKE_HH
# define PROTO_OLN_CORE_ABSTRACT_DPOINT_LIKE_HH


namespace oln {

  namespace abstract {

    template <typename E>
    struct dpoint_like : public mlc::any__best_memory<E>
    {
      operator oln_dpoint_type(E)() const
      {
	return this->exact().op_dpoint_impl();
      }
    protected:
      dpoint_like() {}
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_ABSTRACT_DPOINT_LIKE_HH
