#ifndef PROTO_OLN_CORE_ABSTRACT_POINT_LIKE_HH
# define PROTO_OLN_CORE_ABSTRACT_POINT_LIKE_HH


namespace oln {

  namespace abstract {

    template <typename E>
    struct point_like : public mlc::any__best_memory<E>
    {
      operator oln_point_type(E)() const
      {
	return this->exact().op_point_impl();
      }

      // FIXME: useful?
      const oln_point_type(E) point() const
      {
	return this->exact().op_point_impl();
      }
      
    protected:
      point_like() {}
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_ABSTRACT_POINT_LIKE_HH
