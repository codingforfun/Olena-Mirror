#ifndef OLENA_META_TYPE_HH
# define OLENA_META_TYPE_HH


# include <oln/core/type.hh>
# include <oln/meta/basics.hh>


// private macro so do _not_ use it
# define _is_a__check_result \
sizeof(helper<T,U>::check( (T*) helper<T,U>::makeT() )) == sizeof(oln::type::internal::_yes)


namespace oln {

  namespace type {

    //
    //  wrap
    //
    //  FIXME: This type is a workaround for g++-2.95 problem with implicit 
    //  typename in <foo<T>::ret::dim>, 
    //  write <wrap<typename foo<T>::ret>::dim instead.
    //
    ///////////////////////////////////////////////////////////////////////
    
    template <class T> struct wrap : public T {};


    //
    //  is_a 
    //
    /////////

    struct form
    {
      enum {
	_class_                          = 1,
	_template_l_class_g_class_       = 2,
	_template_l_class_class_g_class_ = 3
	//      _template_l_unsigned_class_g_class_    = 4
	// ...
      };

      template<unsigned id> struct desc { char _dummy[id]; };

      template<class T>
      static desc< _class_ > get();

      template<template<class> class T>
      static desc< _template_l_class_g_class_ > get();

      template<template<class,class> class T>
      static desc< _template_l_class_class_g_class_ > get();

      // template<template<unsigned,class> class T>
      // static desc < _template_l_unsigned_class_g_class_ > get();

      // ...
    };

    namespace internal
    {
    
      typedef char _yes;
      struct _no { char dummy[2]; };


      // dev note : below, _is_a<T,id> is a better factorization
      //            but g++ 2.95.4 has some trouble with it

      template<unsigned id>
      struct _is_a;

      // _class_
    
  
      template<>
      struct _is_a< form::_class_ >
      {
	typedef _is_a< form::_class_ > self;

	template<class T, class U>
	struct helper
	{
	  static _yes check(U*);
	  static _no  check(...);
	  static T* makeT();
	};
      
	template<class T, class U>
	struct check
	  : public oln::meta::returns_bool_<( _is_a__check_result )>
	{
	};
      };
    
      // _template_l_class_g_class_

      template<>
      struct _is_a< form::_template_l_class_g_class_ >
      {
	typedef _is_a< form::_template_l_class_g_class_ > self;

	template<class T, template<class> class U>
	struct helper
	{
	  template<class V>
	  static _yes check(U<V>*);
	  static _no  check(...);
	  static T* makeT();
	};

	template<class T, template<class> class U>
	struct check
	  : public meta::returns_bool_<( _is_a__check_result )>
	{
	};
      };

      // _template_l_class_class_g_class_

      template<>
      struct _is_a< form::_template_l_class_class_g_class_ >
      {
	typedef _is_a< form::_template_l_class_class_g_class_ > self;

	template<class T, template<class,class> class U>
	struct helper
	{
	  template<class V, class W>
	  static _yes check(U<V,W>*);
	  static _no  check(...);
	  static T* makeT();
	};

	template<class T, template<class,class> class U>
	struct check
	  : public meta::returns_bool_<( _is_a__check_result )>
	{
	};
      };

    } // end of namespace oln::type::internal

  } // end of namespace oln::type

} // end of namespace oln

// private macros: do _not_ use them
# define is_a__2nd_is_meta(T,U) oln::type::internal::_is_a< id_of_typeform(U) >::check<T, U>

// client macros

# define id_of_typeform(T) sizeof(oln::type::form::get<T>())
//# define is_a(T,U) oln::type::internal::_is_a< id_of_typeform(U) >::check<T, U>

template <class T>
struct id_ { typedef T ret; };


# define is_a(T, U) oln::type::wrap<typename oln::type::internal::_is_a< id_of_typeform(U) >::check<T, U> >

# endif // ndef OLENA_META_TYPE_HH
