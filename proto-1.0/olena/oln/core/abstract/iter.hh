#ifndef PROTO_OLN_CORE_ABSTRACT_ITER_HH
# define PROTO_OLN_CORE_ABSTRACT_ITER_HH

# include <mlc/any.hh>

# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/size.hh>

namespace oln {

  namespace abstract {

    template <typename E>
    struct iter : mlc::any__best_memory<E>
    {

      void
      operator++()
      {
	this->exact().next_impl();
      }

      void
      begin()
      {
	this->exact().begin_impl();
      }

      bool
      end()
      {
	return this->exact().end_impl();
      }


      template <typename P>
      bool
      operator!=(const point<P> &p) const
      {
	return this->exact().noteq_impl(p.exact());
      }

      template <typename P>
      bool
      operator==(const point<P> &p) const
      {
	return this->exact().eq_impl(p.exact());
      }

      template <typename P>
      operator point<P>() const
      {
	return this->exact().to_point();
      }

      template <typename S>
      void operator() (const size<S> &s)
      {
	this->exact().op_par_impl(s.exact());
      }

    protected:

      iter()
      {}

    };
  }
}


#endif // ndef PROTO_OLN_CORE_ABSTRACT_ITER_HH
