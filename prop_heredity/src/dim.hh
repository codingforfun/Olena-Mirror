#ifndef PROTO_DIM_HH
# define PROTO_DIM_HH

# include "any.hh"
# include "props.hh"


namespace abstract {

  template <typename E>
  struct dim : public mlc::any__best_memory<E>
  {
  protected:
    dim() {}
  };

} // end of namespace abstract


#endif // ndef PROTO_DIM_HH
