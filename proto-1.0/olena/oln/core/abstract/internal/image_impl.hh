#ifndef PROTO_OLN_CORE_ABSTRACT_INTERNAL_IMAGE_IMPL_HH
# define PROTO_OLN_CORE_ABSTRACT_INTERNAL_IMAGE_IMPL_HH

# include <mlc/any.hh>

# include <oln/core/macros.hh>

namespace oln {

  namespace abstract {

    namespace internal {

      // FIXME: doc!

      template <typename A, typename D, typename E>
      struct get_image_impl_helper;

      // entry point:
      template <typename A, typename E>
      struct get_image_impl : public get_image_impl_helper <A, oln_delegated_type(E), E>
      {
      };

      template <typename A, typename E>
      struct get_image_impl_helper <A, mlc::no_type, E> : public virtual mlc::any__best_speed<E>
      {
	// no impl
      };

      template <typename A, typename E>
      struct set_image_impl; // to be specialized...

      template <typename A, typename D, typename E>
      struct get_image_impl_helper : public set_image_impl <A, E>
      {
	// impl comes from  internal::set_image_impl <A, E>
      };

      template <typename E>
      struct image_impl : public mlc::any__best_speed<E>
      {
	typedef oln_delegated_type(E) D;
	D& delegate() { return this->exact().impl_delegate(); }
	const D& delegate() const { return this->exact().impl_delegate(); }
      };

    } // end of namespace internal

  } // end of namespace abstract

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_ABSTRACT_IMAGE_HH
