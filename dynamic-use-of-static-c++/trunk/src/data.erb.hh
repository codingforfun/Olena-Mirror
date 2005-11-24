#ifndef DYN_DATA_HH
# define DYN_DATA_HH

# include <string>
# include <cassert>

# include "function.hh"
# include "name_of.hh"

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

  namespace policy
  {
    enum type
    {
      none     = 0,
      is_const = 1,
      is_ref   = 2,
      is_ptr   = 4,
      is_pod   = 8,
      is_void  = 16,
    };
  }
  struct proxy_tag;

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
#   define gen_ctor                                      \
    data_proxy()                                         \
    {                                                    \
      logger << "ctor: " << proxy_type() << std::endl;   \
    }

#   define gen_proxy_type                       \
    virtual std::string proxy_type() const      \
    {                                           \
      return mlc_name_of(*this);                \
    }

    gen_proxy_type

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
  };


  template <class T>
  struct data_proxy_by_ptr : public data_proxy<T>
  {
    data_proxy_by_ptr(const T* obj) { logger << "ctor: " << proxy_type() << std::endl; }

    virtual data_proxy_by_ptr<T>* clone() const
    {
      return new data_proxy_by_ptr<T>(p_obj_);
    }

    gen_proxy_type

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
  struct data_proxy_by_ref : public data_proxy<T>
  {
    data_proxy_by_ref(T& obj) : obj_(obj) { logger << "ctor: " << proxy_type() << std::endl; }

    virtual data_proxy_by_ref<T>* clone() const
    {
      return new data_proxy_by_ref<T>(obj_);
    }

    virtual const T& const_ref() const { return obj_; }
    const T& obj() const { return obj_; }
    T& obj() { return obj_; }

    gen_proxy_type

    protected:
    T& obj_;
  };


  template <class T>
  struct data_proxy_by_cpy : public data_proxy<T>
  {
    data_proxy_by_cpy(const T obj) : obj_(T(obj)) { logger << "ctor: " << proxy_type() << std::endl; }

    virtual data_proxy_by_cpy<T>* clone() const
    {
      return new data_proxy_by_cpy<T>(obj_);
    }

    virtual const T& const_ref() const { return obj_; }
    const T& obj() const { return obj_; }
    T& obj() { return obj_; }

    gen_proxy_type

    protected:
    T obj_;
  };

  struct NilClass
  {
    NilClass(int) {}
  };

  struct data_nil : public data_proxy<NilClass>
  {
    data_nil() : nil_object_(NilClass(0)) { logger << "ctor: " << proxy_type() << std::endl; }
    data_nil(const NilClass& nil_object) : nil_object_(nil_object) {}
    const NilClass& const_ref() const { return nil_object_; }
    const NilClass& obj() const { return nil_object_; }
    virtual data_nil* clone() const { return new data_nil; }
    gen_proxy_type
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

    std::string& const_stripping(std::string& str) const
    {
      size_t pos;
      while ((pos = str.find("const ")) != std::string::npos)
        str.erase(pos, 6);
      while ((pos = str.find(" const")) != std::string::npos)
        str.erase(pos, 6);
      return str;
    }

    template <typename T>
    operator T() const
    {
      std::string src_type(proxy()->type());
      std::string dest_type(mlc_name<T>::of());
      if (const_stripping(src_type) == const_stripping(dest_type))
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

    bool is_const()
    {
      std::string type_(type());
      return type_.find("dyn::data_proxy_by_ref<") == 0
          && type_.rfind("const>") == type_.length() - 6;
    }

    protected:
    abstract_data* proxy_;

    public:

    #include "data_gen.hh"

#   ifndef INITIALIZE_METHODS_ATTRIBUTES
#   define INITIALIZE_METHODS_ATTRIBUTES fake_method("fake_method", "method", "", this)
    fun fake_method;
#   endif

    data() : proxy_(nil_proxy), INITIALIZE_METHODS_ATTRIBUTES {}

    data(abstract_data* proxy, proxy_tag*) : INITIALIZE_METHODS_ATTRIBUTES
    {
      proxy_ = proxy;
    }

    template <class T>
    data(T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      logger << "data(T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      proxy_ = new data_proxy_by_ref<T>(obj);
    }

    template <class T>
    data(const T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      logger << "data(const T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      proxy_ = new data_proxy_by_ref<const T>(obj);
    }

    data(language::var& rhs);
    data(language::val& rhs);
    data(const language::var& rhs);
    data(const language::val& rhs);

    data(data& rhs) : INITIALIZE_METHODS_ATTRIBUTES
    {
      if (rhs.proxy_ == 0)
      {
        logger << "data(data& rhs) [ rhs.proxy_ == 0 ]" << std::endl;
        proxy_ = nil_proxy;
      }
      else
      {
        logger << "data(data& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
        proxy_ = rhs.proxy_->clone();
      }
    }

    data(const data& rhs) : INITIALIZE_METHODS_ATTRIBUTES
    {
      if (rhs.proxy_ == 0)
      {
        logger << "data(const data& rhs) [ rhs.proxy_ == 0 ]" << std::endl;
        proxy_ = nil_proxy;
      }
      else
      {
        logger << "data(const data& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
        proxy_ = rhs.proxy_->clone();
      }
    }

  };

  const data nil;

}

template <typename T, dyn::policy::type policy>
struct dyn_choose_data_proxy
{
  typedef dyn::data_proxy_by_cpy<T> ret;
};

template <typename T>
struct dyn_choose_data_proxy<T, dyn::policy::is_ref>
{
  typedef dyn::data_proxy_by_ref<T> ret;
};

template <typename T>
struct dyn_choose_data_proxy<T, (dyn::policy::type)(dyn::policy::is_ref + dyn::policy::is_const)>
{
  typedef dyn::data_proxy_by_ref<T> ret;
};

# include "policy.hh"

#endif
