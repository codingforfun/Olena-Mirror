#ifndef DYN_DATA_HH
# define DYN_DATA_HH

# include <string>
# include <cassert>

# include "function.hh"

# include "name_of.hh"

template <typename T>
struct dyn_choose_data_proxy;

# ifdef DYNDEBUG
#  include <iostream>
# else
#  include <fstream>
# endif

namespace dyn {

# ifdef DYNDEBUG
  std::ostream& logger(std::cerr);
# else
#  ifdef NDEBUG
  std::ofstream logger("/dev/null");
#  else
  std::ofstream logger("dyn.log");
#  endif
# endif

  namespace language
  {
    struct val;
    struct var;
  }

  // data  -->  abstract_data
  //                 ^
  //                 |
  //            data_proxy<T>  -->  T


  // abstract_data

  struct abstract_data
  {
    virtual abstract_data* clone() const = 0;
    virtual std::string proxy_type() const = 0;
    virtual std::string type() const = 0;
    virtual ~abstract_data() {}
  };

  // data_proxy<T>

  template <class T>
  struct data_proxy : public abstract_data
  {

    template <typename V>
    operator V() const
    {
      V ret(const_ref());
      return ret;
    }

    virtual std::string type() const
    {
      return mlc_name_of(const_ref());
    }

    virtual const T& const_ref() const = 0;

    protected:
    data_proxy() {}
  };


  template <class T>
  struct data_proxy_by_ptr : public data_proxy<T>
  {
    data_proxy_by_ptr(const T* obj) : data_proxy<T>(obj) {
      logger << "data_proxy by ptr " << mlc_name_of(obj) << ", ptr: " << obj << std::endl;
    }

    virtual data_proxy_by_ptr<T>* clone() const
    {
      return new data_proxy_by_ptr<T>(p_obj_);
    }

    virtual std::string proxy_type() const
    {
      return mlc_name_of(*this);
    }

    virtual const T& const_ref() const
    {
      assert(p_obj_);
      return *p_obj_;
    }

    const T& obj() const { return const_ref(); }

    protected:
    const T* p_obj_;
  };


  template <class T>
  struct data_proxy_by_const_ref : public data_proxy<T>
  {
    data_proxy_by_const_ref(const T& obj) : obj_(obj) {
      logger << "data_proxy by const ref " << mlc_name_of(obj) << std::endl;
    }

    virtual data_proxy_by_const_ref<T>* clone() const
    {
      return new data_proxy_by_const_ref<T>(obj_);
    }

    virtual const T& const_ref() const { return obj_; }
    const T& obj() const { return obj_; }

    virtual std::string proxy_type() const
    {
      return mlc_name_of(*this);
    }

    protected:
    const T& obj_;
  };


  template <class T>
  struct data_proxy_by_ref : public data_proxy<T>
  {
    data_proxy_by_ref(T& obj) : obj_(obj) {
      logger << "data_proxy by ref " << mlc_name_of(obj) << std::endl;
    }

    virtual data_proxy_by_ref<T>* clone() const
    {
      return new data_proxy_by_ref<T>(obj_);
    }

    virtual const T& const_ref() const { return obj_; }
    const T& obj() const { return obj_; }
    T& obj() { return obj_; }

    virtual std::string proxy_type() const
    {
      return mlc_name_of(*this);
    }

    protected:
    T& obj_;
  };

  template <class T>
  struct data_proxy_by_cpy : public data_proxy<T>
  {
    data_proxy_by_cpy(const T obj) : obj_(T(obj)) {
      logger << "data_proxy_by_cpy " << mlc_name_of(obj) << std::endl;
    }

    virtual data_proxy_by_cpy<T>* clone() const
    {
      return new data_proxy_by_cpy<T>(obj_);
    }

    virtual const T& const_ref() const { return obj_; }
    const T& obj() const { return obj_; }
    T& obj() { return obj_; }

    virtual std::string proxy_type() const
    {
      return mlc_name_of(*this);
    }

    protected:
    T obj_;
  };

  struct NilClass
  {
    NilClass(int) {}
  };

  struct data_nil : public data_proxy<NilClass>
  {
    data_nil(const NilClass& nil_object) : nil_object_(nil_object) {}
    const NilClass& const_ref() const { return nil_object_; }
    const NilClass& obj() const { return nil_object_; }
    virtual data_nil* clone() const { return new data_nil(nil_object_); }
    virtual std::string proxy_type() const
    {
      return mlc_name_of(*this);
    }
    const NilClass nil_object_;
  };

  const NilClass nil_object(0);
  data_nil* nil_proxy = new data_nil(nil_object);

  template <typename T1, typename T2>
  T2
  data_cast(const T1& src)
  {
    T2 tmp(src);
    return tmp;
  }

  template <typename T1, typename T2>
  void
  data_assign(T1& lhs, const T2& rhs)
  {
    lhs = rhs;
  }

  namespace tag
  {
    struct by_copy;
    struct lvalue;
  }

  template <typename T>
  struct by_copy;

  template <typename T>
  struct lvalue;

  template <typename T>
  struct readonly;

  // data

  struct data
  {

    data& operator=(const data& rhs)
    {
      if (&rhs == this) return *this;
      assert(rhs.proxy_);
      assert(rhs.proxy_ != proxy_);
      // if (proxy_ != 0)
        // delete proxy_;
      proxy_ = rhs.proxy_->clone();
      return *this;
    }


    template <typename T>
    data& operator=(const T& rhs);


    template <typename T>
    T convert_to() const;


    template <typename T>
    T& get_ref_on();

    template <typename T>
    const T& get_ref_on() const;

    const data& get_const_ref() const;

    template <typename T>
    operator T() const
    {
      std::string str(proxy()->type());
      if (str.compare(0, 6, "const ") == 0)
        str.erase(0, 6);
      if (mlc_name<T>::of() == str)
        return get_ref_on<T>();
      else
        return convert_to<T>();
    }


    ~data()
    {
      if (proxy_ != nil_proxy)
      {
        logger << "~data [ type() = " << type() << " ]" << std::endl;
        delete proxy_;
        proxy_ = nil_proxy;
      }
    }


    std::string type() const
    {
      return proxy()->proxy_type();
    }


    data operator[](const data&);
    const data& operator[](const data&) const;
    data operator*();
    const data& operator*() const;


    abstract_data* proxy() const
    {
      assert(proxy_);
      return proxy_;
    }


    <%- (NB_MAX_ARGUMENTS - 1).times do |i| -%>

      <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" } -%>
      data
      send(<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const;

    <%- end -%>

    fun method(const std::string& method_name);

    protected:
    abstract_data* proxy_;

    public:

    #include "data_gen.hh"

#   ifndef INITIALIZE_METHODS_ATTRIBUTES
#   define INITIALIZE_METHODS_ATTRIBUTES fake_method("fake_method", "method", "", this)
    fun fake_method;
#   endif

    data() : proxy_(nil_proxy), INITIALIZE_METHODS_ATTRIBUTES {}

    template <class T>
    data(const T& obj, const tag::by_copy*) : INITIALIZE_METHODS_ATTRIBUTES
    {
      logger << "data(const T& obj, const tag::by_copy*) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      proxy_ = new typename dyn_choose_data_proxy< by_copy< readonly<T> > >::ret(obj);
    }

    template <class T>
    data(T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      logger << "data(T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      proxy_ = new typename dyn_choose_data_proxy<T>::ret(obj);
    }

    template <class T>
    data(T& obj, const tag::by_copy*) : INITIALIZE_METHODS_ATTRIBUTES
    {
      logger << "data(T& obj, const tag::by_copy*) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      proxy_ = new typename dyn_choose_data_proxy<by_copy<T> >::ret(obj);
    }

    template <class T>
    data(T& obj, const tag::lvalue*) : INITIALIZE_METHODS_ATTRIBUTES
    {
      logger << "data(T& obj, const tag::lvalue*) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      proxy_ = new typename dyn_choose_data_proxy< lvalue<T> >::ret(obj);
    }

    template <class T>
    data(const T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      logger << "data(const T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      proxy_ = new typename dyn_choose_data_proxy< readonly<T> >::ret(obj);
    }

    data(const language::var& rhs);
    data(const language::val& rhs);

    data(const data& rhs) : INITIALIZE_METHODS_ATTRIBUTES
    {
      logger << "data(const data& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
       if (rhs.proxy_ != nil_proxy)
        proxy_ = rhs.proxy_->clone();
      else
        proxy_ = nil_proxy;
    }

  };

  const data nil;

}

template <typename T>
struct dyn_choose_data_proxy
{
  typedef dyn::data_proxy_by_ref<T> ret;
};

template <typename T>
struct dyn_choose_data_proxy< dyn::readonly< T > >
{
  typedef dyn::data_proxy_by_const_ref<T> ret;
};

template <typename T>
struct dyn_choose_data_proxy< dyn::by_copy<T> >
{
  typedef dyn::data_proxy_by_cpy<T> ret;
};

template <typename T>
struct dyn_choose_data_proxy< dyn::by_copy< dyn::readonly<T> > >
{
  typedef dyn::data_proxy_by_cpy<T> ret;
};

template <typename T>
struct dyn_choose_data_proxy< dyn::lvalue< T > >
{
  typedef dyn::data_proxy_by_ref<T> ret;
};

template <>
struct dyn_choose_data_proxy< dyn::NilClass >
{
  typedef dyn::data_nil ret;
};

#define has_not_cpy_ctor(TYPE)                                          \
  template <>                                                           \
  struct dyn_choose_data_proxy< dyn::by_copy< TYPE > >                  \
  {                                                                     \
    typedef dyn_choose_data_proxy< TYPE >::ret ret;                     \
  };                                                                    \
  template <>                                                           \
  struct dyn_choose_data_proxy< dyn::by_copy< dyn::readonly< TYPE > > > \
  {                                                                     \
    typedef dyn_choose_data_proxy< dyn::readonly< TYPE > >::ret ret;    \
  }

has_not_cpy_ctor(std::ostream);
has_not_cpy_ctor(std::istream);

template <>
struct mlc_name<__gnu_cxx::__normal_iterator<const int*, std::vector<int, std::allocator<int> > > >
{
  static std::string of() { return "__gnu_cxx::__normal_iterator<const int*, std::vector<int, std::allocator<int> > >"; }
};

mlc_set_name(dyn::abstract_data);
mlc_set_name(dyn::data_nil);
mlc_set_name_TC(dyn::data_proxy);
mlc_set_name_TC(dyn::data_proxy_by_cpy);
mlc_set_name_TC(dyn::data_proxy_by_ref);
mlc_set_name_TC(dyn::data_proxy_by_const_ref);
mlc_set_name_TC(dyn::data_proxy_by_ptr);
mlc_set_name(dyn::data);
mlc_set_name(dyn::NilClass);

template <>
struct mlc_name2 <dyn::data>
{
  static std::string of(const dyn::data& d) { return d.type(); }
};

#endif
