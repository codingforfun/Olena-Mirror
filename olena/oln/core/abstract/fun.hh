#ifndef PROTO_OLN_CORE_ABSTRACT_FUN_HH
# define PROTO_OLN_CORE_ABSTRACT_FUN_HH



namespace oln {

  namespace abstract {

    template <typename E>
    struct fun : public mlc::any__best_memory<E>
    {
    protected:
      fun() {}
    };

    template <typename E>
    struct unary_fun : public fun<E>
    {
      const oln_return_type(E) operator()(oln_arg_type(E) value) const
      {
	return this->exact().op_paren_impl(value);
      }
    protected:
      unary_fun() {}
    };

    template <typename E>
    struct binary_fun : public fun<E>
    {
      const oln_return_type(E) operator()(oln_arg1_type(E) value1,
					  oln_arg2_type(E) value2) const
      {
	return this->exact().op_paren_impl(value1, value2);
      }
    protected:
      binary_fun() {}
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_ABSTRACT_FUN_HH
