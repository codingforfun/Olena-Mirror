#ifndef PROTO_ANY_HH
# define PROTO_ANY_HH


# include <assert.h>

// abrev
#define any_mem(E) any <E, dispatch_policy::best_memory>


// exact stuff

template <typename T> struct exact_type;

# define Exact_type(T) typename exact_type<T>::ret

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
      assert(this_ != 0);
      return *this_;
    }
    const E& exact() const {
      assert(this_ != 0);
      return *this_;
    }
    E* this_;
  protected:
    any() : this_(0) {}
    virtual ~any() {}
  };

  template <typename E>
  struct any__best_speed : public any<E, dispatch_policy::best_speed>
  {};

  template <typename E>
  inline void
  set_this(E* this_)
  {
    this_->this_ = this_;
  }

  template <typename E>
  inline void
  set_this(any<E, dispatch_policy::best_memory>* this_)
  {}

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
    virtual ~any() {}
  };

  template <typename E>
  struct any__best_memory : public any<E, dispatch_policy::best_memory>
  {};

  template <typename E> const E any_mem(E)::exact_obj = E();
  template <typename E> const any_mem(E)& any_mem(E)::ref_exact_obj = any_mem(E)::exact_obj;
  template <typename E> const int any_mem(E)::exact_offset =
                                     (const char*)(&any_mem(E)::ref_exact_obj)
                                       - (const char*)(&any_mem(E)::exact_obj);


  // "compromise" version of 'any'

  template <typename E>
  struct any <E,
              dispatch_policy::compromise>; // FIXME: not yet impled


} // end of namespace mlc


#endif // ndef PROTO_ANY_HH
