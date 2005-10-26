#ifndef MINIOLN_MLC_HH
# define MINIOLN_MLC_HH

# include "misc.hh"



struct mlc_FIXME;
set_name(mlc_FIXME);



// --------------------------------------------------------  mlc/value.hh


namespace mlc {


  /// Class that defines a mlc value as a type (for instance mlc::value<int,3>).
  template <typename T, T v>
  struct value
  {
    static const T val = v;
    value() {}
    operator T() const { return v; }
  };


} // end of namespace mlc


template <class T, T v>
struct name < mlc::value<T, v> > {
  static std::string of() {
    std::ostringstream ostr_;
    ostr_ << std::string("mlc::value<") << name<T>::of()
	  << "," << v << ">";
    return ostr_.str();
  }
};


// --------------------------------------------------------  mlc/bool.hh


// macro
# define mlc_bool(Type)  mlc::internal::helper_get_bool< Type >::ret


namespace mlc
{

  // Specializations of mlc::value for T = bool
  // when v = true, ensure() is provided.

  template <>
  struct value <bool, true>
  {
    static const bool Boolean_value = true;
    static const bool val = true;
    static void ensure() {}
  protected:
    value() {}
  };

  template <>
  struct value <bool, false>
  {
    static const bool Boolean_value = false;
    static const bool val = false;
  protected:
    value() {}
  };


  // Typedefs of true_type and false_type.
  typedef value<bool, true>  true_type;
  typedef value<bool, false> false_type;


  namespace internal
  {

    template <typename T>
    struct helper_get_bool
    {
      // FIXME: static check that T derives from either true_type or false_type...
      static const bool ret = T::Boolean_value;
    };

  } // end of namespace mlc::internal


  // Logical unary not of a Boolean type

  template <typename T>
  struct not_ : public value<bool, !mlc_bool(T)>
  {
  };

  // Logical binary operators between a couple of Boolean types

  template <typename L, typename R> struct and_
    : public value <bool,   (mlc_bool(L) && mlc_bool(R)) > {};
  template <typename L, typename R> struct nand_
    : public value <bool, (!(mlc_bool(L) && mlc_bool(R)))> {}; 
  template <typename L, typename R> struct or_   
    : public value <bool,   (mlc_bool(L) || mlc_bool(R)) > {};
  template <typename L, typename R> struct nor_  
    : public value <bool, (!(mlc_bool(L) || mlc_bool(R)))> {};
  template <typename L, typename R> struct xor_  
    : public value <bool,   (mlc_bool(L) != mlc_bool(R)) > {};
  template <typename L, typename R> struct xnor_ 
    : public value <bool, (!(mlc_bool(L) != mlc_bool(R)))> {};


} // end of namespace mlc

set_name(mlc::true_type);
set_name(mlc::false_type);



// --------------------------------------------------------  mlc/if.hh


// macro
# define mlc_if(Cond, Then, Else)  typename mlc::if_<(Cond), (Then), (Else)>::ret


namespace mlc
{

  namespace internal
  {

    template <bool b, typename then_type, typename else_type>
    struct fun_if_;

    template <typename then_type, typename else_type>
    struct fun_if_ <true, then_type, else_type>
    {
      typedef then_type ret;
    };
    
    template <typename then_type, typename else_type>
    struct fun_if_ <false, then_type, else_type>
    {
      typedef else_type ret;
    };
    
  } // end of namespace mlc::internal


  template <typename cond_type, typename then_type, typename else_type>
  struct if_ 
    : public internal::fun_if_ < mlc_bool(cond_type), then_type, else_type >
  {
  };


} // end of namespace mlc



// --------------------------------------------------------  mlc/cmp.hh


// Macros mlc_eq and mlc_neq.
# define mlc_eq(T1, T2) mlc::eq<T1, T2>
# define mlc_neq(T1, T2) mlc::neq<T1, T2>


namespace mlc
{

  // Equality test between a couple of types.

  template <typename T1, typename T2>
  struct eq : public value<bool, false>
  {
  };

  template <typename T>
  struct eq <T, T> : public value<bool, true>
  {
  };

  // Inequality test between a couple of types.

  template <typename T1, typename T2>
  struct neq : public value<bool, true>
  {
  };

  template <typename T>
  struct neq <T, T> : public value<bool, false>
  {
  };


} // end of namespace mlc



// ------------------------------------------------  *simplified* mlc/any.hh


namespace mlc
{

  template <class E>
  struct any
  {
    E& exact() { return *(E*)(void*)(this); }
    const E& exact() const { return *(const E*)(const void*)(this); }
  protected:
    any() {}
    virtual ~any() {} // in this proto speed is not required
  };

} // end of namespace mlc

set_name_TC(mlc::any);




// ------------------------------------------------  *simplified* mlc/is_a.hh

//                                           used only for C, TC, and TCC

# if __GNUC__ >= 3

#  define mlc_is_a(T,U) \
mlc::internal::wrap<typename mlc::internal::is_a_<sizeof(mlc::form::get<U>())>::check<T,U> >

// private stuff
#  define is_a__check_result_ \
sizeof(helper<T,U>::check((T*)helper<T,U>::makeT()))==sizeof(mlc::internal::yes_)
namespace mlc {
  namespace internal {
    template <class T> struct wrap : public T {};
  }
  namespace form {
    enum {
      class_                          = 1,
      template_l_class_g_class_       = 2,
      template_l_class_class_g_class_ = 3,
      template_l_template_l_class_g_class_g_class_       = 4,
      template_l_template_l_class_class_g_class_g_class_ = 5
    };
    template<unsigned id> struct desc { char dummy[id]; };
    template   < class T>
    static desc< class_ > get();
    template   < template < class > class T>
    static desc< template_l_class_g_class_ > get();
    template<    template < class,class > class T>
    static desc< template_l_class_class_g_class_ > get();
    template<    template < template < class > class > class T>
    static desc< template_l_template_l_class_g_class_g_class_ > get();
    template<    template < template < class,class > class > class T>
    static desc< template_l_template_l_class_class_g_class_g_class_ > get();
  }
  namespace internal
  {
    typedef char yes_;
    struct no_ { char dummy[2]; };
    template<unsigned id> struct is_a_;
    // class_
    template<>
    struct is_a_< form::class_ > {
      typedef is_a_< form::class_ > self;
      template<class T,    class U>
      struct helper {
	static yes_ check(U*);
	static no_  check(...);
	static T* makeT();
      };
      template<class T,    class U>
      struct check
	: public value<bool,( is_a__check_result_ )>
      {};
    };
    // template_l_class_g_class_
    template<>
    struct is_a_< form::template_l_class_g_class_ > {
      typedef is_a_< form::template_l_class_g_class_ > self;
      template<class T,    template < class > class U>
      struct helper {
	template<class V>
	static yes_ check(U<V>*);
	static no_  check(...);
	static T* makeT();
      };
      template<class T,    template < class > class U>
      struct check
	: public value<bool,( is_a__check_result_ )>
      {};
    };
    // template_l_class_class_g_class_
    template<>
    struct is_a_< form::template_l_class_class_g_class_ >
    {
      typedef is_a_< form::template_l_class_class_g_class_ > self;
      template<class T,    template < class,class > class U>
      struct helper
      {
	template<class V, class W>
	static yes_ check(U<V,W>*);
	static no_  check(...);
	static T* makeT();
      };
      template<class T,    template < class,class > class U>
      struct check
	: public value<bool,( is_a__check_result_ )>
      {};
    };
    // template_l_template_l_class_g_class_g_class_
    template<>
    struct is_a_< form::template_l_template_l_class_g_class_g_class_ > {
      typedef is_a_< form::template_l_template_l_class_g_class_g_class_ > self;
      template<class T,    template < template < class > class > class U>
      struct helper {
	template<template<class> class V>
	static yes_ check(U<V>*);
	static no_  check(...);
	static T* makeT();
      };
      template<class T,    template < template < class > class > class U>
      struct check
	: public value<bool,( is_a__check_result_ )>
      {};
    };
    // template_l_template_l_class_class_g_class_g_class_
    template<>
    struct is_a_< form::template_l_template_l_class_class_g_class_g_class_ > {
      typedef is_a_< form::template_l_template_l_class_class_g_class_g_class_ > self;
      template<class T,    template < template < class,class > class > class U>
      struct helper {
	template<template<class,class> class V>
	static yes_ check(U<V>*);
	static no_  check(...);
	static T* makeT();
      };
      template<class T,    template < template < class,class > class > class U>
      struct check
	: public value<bool,( is_a__check_result_ )>
      {};
    };
  } // end of internal
} // end of mlc
#  define id_of_typeform(T) sizeof(mlc::form::get<T >())

# else // __GNUC__ < 3
#  define mlc_is_a(T,U) mlc::true_type
# endif 


// --------------------------------------------  *simplified* mlc/overload.hh


// FIXME: ...


// -----------------------------------------------------  *like* mlc/types.hh


namespace mlc
{

  // mlc::undefined

  struct undefined
  {
    // FIXME: is the following contents really useful?

    // list of typedefs that can be used
    typedef mlc::undefined bkd_niter_type;
    typedef mlc::undefined bkd_piter_type;
    typedef mlc::undefined concrete_type;
    typedef mlc::undefined coord_type;
    typedef mlc::undefined dim_type;
    typedef mlc::undefined fwd_niter_type;
    typedef mlc::undefined fwd_piter_type;
    typedef mlc::undefined grd_type;
    typedef mlc::undefined info_type;
    typedef mlc::undefined niter_type;
    typedef mlc::undefined piter_type;
    typedef mlc::undefined point_type;
    typedef mlc::undefined self_type;
    typedef mlc::undefined size_type;
    typedef mlc::undefined super;
    typedef mlc::undefined super_type;
    typedef mlc::undefined value_get_type;
    typedef mlc::undefined value_set_type;
    typedef mlc::undefined value_type;
    // end of list

    // the following methods are public to allow "fake" abstract
    // method definitions
    undefined() {}
    undefined(const undefined&) {}
    undefined& operator=(const undefined&) { return *this; }
  };


  // mlc::no_type

  struct no_type
  {
  };


  // mlc::is_set<T>
  // T is neither mlc::undefined nor mlc::no_type

  template <class T>
  struct is_set
  {
    static void ensure() {}
  };
  
  template <>
  struct is_set < mlc::undefined > {
    // no ensure() here
  };
  
  template <>
  struct is_set < mlc::no_type > {
    // no ensure() here
  };

} // end of namespace mlc


set_name(mlc::undefined);
set_name(mlc::no_type);



// --------------------------------------------------------  ...



#endif
