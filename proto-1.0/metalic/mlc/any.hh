
#ifndef METALIC_ANY_HH
# define METALIC_ANY_HH




// abrev
#define any_mem(E) any <E, dispatch_policy::best_memory>



// 'any' classes

namespace mlc
{

  struct dispatch_policy
  {
    struct best_speed;
    struct best_memory;
    struct compromise;
  };


  // any

  template <typename E,
	    typename Dispatch_Policy = dispatch_policy::best_memory>
  struct any;


  // "best speed" version of 'any'

  template <typename E>
  struct any <E,
              dispatch_policy::best_speed>
  {
    E& exact() {
      precondition(exact_ptr != 0);
      return *exact_ptr;
    }
    const E& exact() const {
      precondition(exact_ptr != 0);
      return *exact_ptr;
    }
  protected:
    any(E* exact_ptr) : exact_ptr(exact_ptr) {}
    virtual ~any() {}
    E* exact_ptr;
  };

  template <typename E>
  struct any__best_speed : public any<E, dispatch_policy::best_speed>
  {
  protected:
    typedef any<E, dispatch_policy::best_speed> super;
    any__best_speed(E* exact_ptr = 0) : super(exact_ptr) {}
  };

  // "best memory" version of 'any'

  template <typename E>
  struct any <E,
	      dispatch_policy::best_memory>
  {
    E& exact() {
      return *(E*)((char*)this - exact_offset);
    }
    const E& exact() const {
      return *(const E*)((const char*)this - exact_offset);
    }
    
    static const int exact_offset;
    static const E exact_obj;
    static const any_mem(E)& ref_exact_obj;
  
  protected:
    any() {}
    any(E* exact_ptr); // safety: w/o impl
    virtual ~any() {}
  };

  template <typename E>
  struct any__best_memory : public any<E, dispatch_policy::best_memory>
  {
  protected:
    typedef any<E, dispatch_policy::best_memory> super;
    any__best_memory() : super() {}
  };

  template <typename E> const E any_mem(E)::exact_obj = E();
  template <typename E> const any_mem(E)& any_mem(E)::ref_exact_obj = any_mem(E)::exact_obj;
  template <typename E> const int any_mem(E)::exact_offset =
                                     (const char*)(void*)(&any_mem(E)::ref_exact_obj)
                                       - (const char*)(void*)(&any_mem(E)::exact_obj);


  // "compromise" version of 'any'

  template <typename E>
  struct any <E,
              dispatch_policy::compromise>; // FIXME: not yet impled


} // end of namespace mlc


#endif // ! METALIC_ANY_HH
