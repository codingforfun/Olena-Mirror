#ifndef PROTO_OLN_CORE_ABSTRACT_ITER_HH
# define PROTO_OLN_CORE_ABSTRACT_ITER_HH


# define for_all(p) for(p.begin(); ! p.is_at_end(); p.next()) 
# define for_all_remaining(p) for(p.next(); ! p.is_at_end(); p.next()) 


namespace oln {

  namespace abstract {

    template <typename E>
    struct iter : public abstract::point_like<E>
    {
      const oln_point_type(E) op_point_impl() const
      {
	return p_;
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

      // hook:
      oln_point_type(E)& point() { return p_; }

    protected:
      iter() {}
      oln_point_type(E) p_;
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_ABSTRACT_ITER_HH
