
// client macro

# define mlc_is_a(T, U) \
typename mlc::internal::is_a_<sizeof(mlc::form::get<U>())>::check<T,U>




# define mlc_internal_is_a__check_result \
sizeof(helper<T,U>::check((T*)helper<T,U>::makeT())) == sizeof(mlc::internal::yes_)


namespace mlc
{

  namespace form
  {
    template<unsigned id> struct desc { char dummy[id]; };

    enum {
      class_                          = 1,
      template_l_class_g_class_       = 2,
      template_l_class_class_g_class_ = 3
    };

    template   < class T>
    static desc< class_ > get();

    template   < template < class > class T>
    static desc< template_l_class_g_class_ > get();

    template<    template < class,class > class T>
    static desc< template_l_class_class_g_class_ > get();
  }

  namespace internal
  {
    typedef char yes_;
    struct no_ { char dummy[2]; };
    template<unsigned id> struct is_a_;

    // class_
    template<>
    struct is_a_< form::class_ >
    {
      template<class T,    class U>
      struct helper
      {
	static yes_ check(U*);
	static no_  check(...);
	static T* makeT();
      };
      template<class T,    class U>
      struct check
	: public value<bool,( mlc_internal_is_a__check_result )>
      {
      };
    };

    // template_l_class_g_class_
    template<>
    struct is_a_< form::template_l_class_g_class_ >
    {
      template<class T,    template < class > class U>
      struct helper
      {
	template<class V>
	static yes_ check(U<V>*);
	static no_  check(...);
	static T* makeT();
      };
      template<class T,    template < class > class U>
      struct check
	: public value<bool,( mlc_internal_is_a__check_result )>
      {
      };
    };

    // template_l_class_class_g_class_
    template<>
    struct is_a_< form::template_l_class_class_g_class_ >
    {
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
	: public value<bool,( mlc_internal_is_a__check_result )>
      {};
    };

  } // end of internal

} // end of mlc




