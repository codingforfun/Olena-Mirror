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
