#ifndef PROTO_OLN_CORE_ABSTRACT_NITER_HH
# define PROTO_OLN_CORE_ABSTRACT_NITER_HH



namespace oln {

  namespace abstract {

    template <typename E>
    struct niter : public abstract::point_like<E>
    {
      const oln_point_type(E) op_point_impl() const
      {
	return center_ + dps_[i_];
      }

      void begin()
      {
	this->exact().begin_impl();
      }

      E& first()
      {
	this->begin();
	return this->exact();
      }

      bool is_at_end() const
      {
	return this->exact().is_at_end_impl();
      }

      void next()
      {
	return this->exact().next_impl();
      }

    protected:

      niter() {}

      template <typename P, typename D>
      niter(const point<P>& center,
	    const struct_elt<D>& dps) :
	center_(center.exact()),
	dps_(dps.vector())
      {
      }

      template <typename P, typename D>
      niter(const point_like<P>& center,
	    const struct_elt<D>& dps) :
	center_(center.point()),
	dps_(dps.vector())
      {
      }

      oln_point_type(E) center_;
      std::vector<oln_dpoint_type(E)> dps_; // FIXME: const &
      unsigned i_;
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_ABSTRACT_NITER_HH
