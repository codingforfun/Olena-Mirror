#ifndef OLN_CORE_ABSTRACT_DIM_HH
# define OLN_CORE_ABSTRACT_DIM_HH

# include <mlc/type.hh>


namespace oln {
  namespace abstract {

    template <typename E>
    struct dim : public mlc_hierarchy::any<E>
    {
    protected:
      dim() {}
    };

  } // abstract
} // oln


#endif // !OLN_CORE_ABSTRACT_DIM_HH
