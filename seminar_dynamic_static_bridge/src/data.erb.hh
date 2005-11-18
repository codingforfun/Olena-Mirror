#ifndef DYN_DATA_HH
# define DYN_DATA_HH

# include <string>
# include <cassert>
# include <istream>
# include <ostream>

# include "function.hh"

# include "name_of.hh"

namespace dyn {

  // data  -->  abstract_data
  //                 ^
  //                 |
  //            data_proxy<T>  -->  T


  // abstract_data

  struct abstract_data
  {
    virtual abstract_data* clone() const = 0;
    virtual std::string type() const = 0;
    virtual std::string type2() const = 0;
    virtual ~abstract_data() {}
  };

  // data_proxy<T>

  template <class T>
  struct data_proxy : public abstract_data
  {

    data_proxy(const T& obj) : p_obj_(&obj) {}

    data_proxy(const T* obj) : p_obj_(obj) {}

    virtual data_proxy<T>* clone() const
    {
      assert(p_obj_);
      return new data_proxy<T>(*p_obj_);
    }

    template <typename V>
    operator V() const
    {
      assert(p_obj_);
      V ret(*obj());
      return ret;
    }

    T* obj() const
    {
      assert(p_obj_);
      return const_cast<T*>(p_obj_);
    }

    std::string type() const
    {
      assert(p_obj_);
      return mlc_name_of(*p_obj_);
    }

    std::string type2() const
    {
      assert(p_obj_);
      return typeid(*p_obj_).name();
    }

    protected:
    const T* p_obj_;
  };


  // FIXME constness
  // template <class T>
  // struct data_proxy<const T>: public data_proxy<T>
  // {
    // data_proxy<const T>(const T& obj) : data_proxy<T>(obj) {}

    // const T* obj() const
    // {
      // assert(this->p_obj_ != 0);
      // return this->p_obj_;
    // }
  // };


  template <class T>
  struct data_proxy_by_cpy : public data_proxy<T>
  {
    // data_proxy_by_cpy(T& obj) : data_proxy<T>(new T(obj)) {}
    // data_proxy_by_cpy(const T& obj) : data_proxy<const T>(new T(obj)) {}
    data_proxy_by_cpy(const T& obj) : data_proxy<T>(new T(obj)) {}

    ~data_proxy_by_cpy()
    {
      delete this->p_obj_;
    }
  };


#define has_not_cpy_ctor(TYPE)                              \
  template <>                                               \
  struct data_proxy_by_cpy<TYPE> : public data_proxy<TYPE>  \
  {                                                         \
    data_proxy_by_cpy(TYPE& obj) : data_proxy<TYPE>(obj) {} \
  }

  has_not_cpy_ctor(std::ostream);
  has_not_cpy_ctor(std::istream);

  struct NilClass {};

  template <>
  struct data_proxy<NilClass> : public abstract_data
  {
    data_proxy(const NilClass& nil_object) : nil_object_(nil_object) {}
    data_proxy() {}

    virtual data_proxy<NilClass>* clone() const
    {
      return new data_proxy<NilClass>();
    }

    std::string type() const
    {
      return "data_nil";
    }

    std::string type2() const
    {
      return typeid(nil_object_).name();
    }

    NilClass nil_object_;
  };

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

  struct by_cpy;

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
      type_ = rhs.type_;
      return *this;
    }


    template <typename T>
    data& operator=(const T& rhs);


    template <typename T>
    T convert_to() const;


    template <typename T>
    T& get_ref_on() const;

    const data& get_const_ref() const;

    template <typename T>
    operator T() const
    {
      std::string str(type_);
      if (str.compare(0, 6, "const ") == 0)
        str.erase(0, 6);
      if (mlc_name<T>::of() == str)
        return get_ref_on<T>();
      else
        return convert_to<T>();
    }


    ~data()
    {
      delete proxy_;
    }


    std::string type() const
    {
      return type_;
    }


    std::string type2() const
    {
      assert(proxy_);
      return proxy_->type2();
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
      data*
      operator() (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const;

      <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" } -%>
      data*
      v (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const;

    <%- end -%>

    protected:
    abstract_data* proxy_;
    std::string type_;

    public:

    #include "data_gen.hh"

#   ifndef INITIALIZE_METHODS_ATTRIBUTES
#   define INITIALIZE_METHODS_ATTRIBUTES fake_method("fake_method", "method")
    fun fake_method;
#   endif

    data() : proxy_(0), type_("unkown"), INITIALIZE_METHODS_ATTRIBUTES {}

    template <class T>
    data(const T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      // proxy_ = new data_proxy<const T>(obj); // FIXME constness
      proxy_ = new data_proxy<T>(obj);
      type_ = mlc_name_of(obj);
    }

    template <class T>
    data(const T& obj, const by_cpy*) : INITIALIZE_METHODS_ATTRIBUTES
    {
      // proxy_ = new data_proxy_by_cpy<const T>(obj); // FIXME constness
      proxy_ = new data_proxy_by_cpy<T>(obj);
      type_ = mlc_name_of(obj);
    }

    template <class T>
    data(T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      proxy_ = new data_proxy<T>(obj);
      type_ = mlc_name_of(obj);
    }

    template <class T>
    data(T& obj, const by_cpy*) : INITIALIZE_METHODS_ATTRIBUTES
    {
      proxy_ = new data_proxy_by_cpy<T>(obj);
      type_ = mlc_name_of(obj);
    }

    data(const data& rhs) : INITIALIZE_METHODS_ATTRIBUTES
    {
       if (rhs.proxy_ != 0)
        proxy_ = rhs.proxy_->clone();
      else
        proxy_ = 0;
      type_ = rhs.type_;
    }
  };

  NilClass nil_object;
  data nil(nil_object);

  std::string data_type(const data& d)
  {
    return d.type();
  }

}

mlc_set_name(dyn::abstract_data);
mlc_set_name_TC(dyn::data_proxy);
mlc_set_name(dyn::data);
mlc_set_name(dyn::NilClass);

template <>
struct mlc_name2 <dyn::data>
{
  static std::string of(const dyn::data& d) { return d.type(); }
};

#endif
