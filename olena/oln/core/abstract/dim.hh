#ifndef PROTO_OLN_CORE_ABSTRACT_DIM_HH
# define PROTO_OLN_CORE_ABSTRACT_DIM_HH

# include <mlc/type.hh>


namespace oln {
  namespace abstract {

    template <typename E>
    struct dim : public mlc::any__best_memory<E>
    {
    protected:
      dim() {}
    };

  } // abstract
} // oln


#endif // ndef PROTO_OLN_CORE_ABSTRACT_DIM_HH
