
#ifndef PROTO_OLN_CORE_APPLY_HH
# define PROTO_OLN_CORE_APPLY_HH

# include <oln/core/ch_value.hh>
# include <oln/core/abstract/fun.hh>

namespace oln {

  template <typename I, typename F>
  oln_ch_value_type(I, oln_return_type(F))
    apply(const abstract::image<I>& input,
	  const abstract::unary_fun<F>& fun)
  {
    oln_ch_value_type(I, oln_return_type(F)) output(input.size());
    oln_iter_type(I) p(input.size());
    for_all(p)
      output[p] = fun(input[p]);
    return output;
  }


} // end of namespace oln




#endif // ndef PROTO_OLN_CORE_APPLY_HH
