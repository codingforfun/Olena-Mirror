#ifndef METALIC_BOOL_HH
# define METALIC_BOOL_HH


namespace mlc
{


  struct false_type;

  struct true_type  {
    static void is_true() {}
    typedef false_type not_type;
    typedef void is_true_type;
  };

  struct false_type {
    static void is_false() {}
    typedef true_type not_type;
    typedef void is_false_type;
  };



  /*----.
  | if_ |
  `----*/

  template <bool Cond, class if_true_type, class if_false_type>
  struct if_ {
    typedef if_true_type ret;
  };

  template<class if_true_type, class if_false_type>
  struct if_<false, if_true_type, if_false_type>
  {
    typedef if_false_type ret;
    typedef false_type ensure_type;
  };

  /*--------.
  | switch_ |
  `--------*/

  struct invalid {};

  template<unsigned Cond, class Ret, class Cases = invalid>
  struct case_ {};

  template<unsigned Cond, class Cases, class Default = invalid>
  struct switch_;

  template<unsigned Cond, unsigned Compare, class Ret, class Default>
  struct switch_<Cond, case_<Compare, Ret>, Default >
  {
    typedef typename if_< (Cond == Compare), Ret, Default >::ret ret;
  };

  template<unsigned Cond,
      unsigned Compare,
      class Ret,
      class Cases,
      class Default>
  struct switch_<Cond, case_<Compare, Ret, Cases>, Default >
  {
    typedef typename
      if_< (Cond == Compare),
      Ret,
      typename switch_<Cond, Cases, Default>::ret>::ret ret;
  };

  template<bool Cond, class Ret, class Cases = invalid>
  struct bool_case_ {};

  template<class Cases, class Default = invalid>
  struct bool_switch_;

  template<bool Cond, class Ret, class Default>
  struct bool_switch_<bool_case_<Cond, Ret>, Default >
  {
    typedef typename if_< Cond, Ret, Default >::ret ret;
  };

  template<bool Cond,class Ret, class Cases, class Default>
  struct bool_switch_<bool_case_<Cond, Ret, Cases>, Default >
  {
    typedef typename
    if_< Cond,
	 Ret,
	 typename bool_switch_<Cases, Default>::ret >::ret ret;
  };

  /*-----.
  | misc |
  `-----*/

  template<bool> struct is_true;
  template<> struct is_true<true>
  {
    static void ensure() {};
    typedef true_type ensure_type;
  };

  template<bool> struct is_false;
  template<> struct is_false<false> { static void ensure() {}; };

  // FIXME: find a good name for this struct.

  // base class for meta-types returning Boolean values

  template<bool> struct returns_bool_;

  template<>
  struct returns_bool_<true> : public true_type
  {
    typedef void ensure_type;
    static const bool ret = true;
    static void ensure() {}
  };

  template<>
  struct returns_bool_<false> : public false_type
  {
    static const bool ret = false;
  };

} // end of namespace mlc


#endif // ! METALIC_BOOL_HH
