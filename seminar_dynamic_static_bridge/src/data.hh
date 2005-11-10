#include <string>
#include <cassert>

#include "mlc.hh"

namespace dyn {

  // data  -->  abstract_data
  //                 ^
  //                 |
  //           data_proxy<T>  -->  T


  // abstract_data

  struct abstract_data
  {
    virtual abstract_data* clone() const = 0;
    virtual void print(std::ostream& ostr) const = 0;
    // virtual void assign_to(void* lhs) const = 0;
    virtual std::string type() const = 0;
    virtual ~abstract_data() {}
  };

  // data_proxy<T>

  template <class T>
  struct data_proxy : public abstract_data
  {
    data_proxy(const T& obj) : p_obj_(&obj) {}

    virtual data_proxy<T>* clone() const
    {
      return new data_proxy<T>(*p_obj_);
    }

    template <typename V>
    operator V() const
    {
      assert(p_obj_ != 0);
      V ret(*p_obj_);
      return ret;
    }

    virtual void print(std::ostream& ostr) const
    {
      assert(p_obj_ != 0);
      ostr << (*p_obj_);
    }

    template <class U>
    void assign(const U& rhs) const
    {
      assert(p_obj_ != 0);
      *(const_cast<T*>(p_obj_)) = rhs;
    }

    std::string type() const
    {
      assert(p_obj_ != 0);
      return mlc_name_of(*p_obj_);
    }

    const T* p_obj_;
  };

  struct NilClass {};

  template <>
  struct data_proxy<NilClass> : public abstract_data
  {
    data_proxy<NilClass>(const NilClass& nil_object) {}

    virtual data_proxy<NilClass>* clone() const
    {
      return const_cast<data_proxy<NilClass>*>(this);
    }

    virtual void print(std::ostream& ostr) const
    {
      ostr << std::string("nil");
    }

    std::string type() const
    {
      return "data_nil";
    }

  };

  struct fun;

  template <typename T1, typename T2>
  T2
  data_cast(const T1& src, const T2& dest)
  {
    T2 ret(src);
    return ret;
  }

  // data

  struct data
  {
    data() : proxy_(0), type_("unkown") {}

    template <class T>
    data(const T& obj)
    {
      proxy_ = new data_proxy<T>(obj);
      type_ = mlc_name<T>::of();
    }

    data(const data& rhs)
    {
      if (rhs.proxy_ != 0)
        proxy_ = rhs.proxy_->clone();
      else
        proxy_ = 0;
      type_ = rhs.type_;
    }

    data& operator=(const data& rhs)
    {
      assert(rhs.proxy_ != 0);
      assert(rhs.proxy_ != proxy_);
      if (proxy_ != 0)
        delete proxy_;
      proxy_ = rhs.proxy_->clone();
      type_ = rhs.type_;
      return *this;
    }


    template <typename T>
    operator T() const
    {
      assert(proxy_ != 0);
      data_proxy<T>* dynamic_cast_returned_pointer = dynamic_cast<data_proxy<T>*>(proxy_);
      if (dynamic_cast_returned_pointer != 0)
      {
        assert(dynamic_cast_returned_pointer->p_obj_ != 0);
        return *dynamic_cast_returned_pointer->p_obj_;
      }
      else
      {
        fun dyn_data_cast("data_cast");
        T tmp;
        T ret(dyn_data_cast(*this, tmp));
        return ret;
      }
    }


    ~data()
    {
      if (proxy_ != 0) {
        delete proxy_;
        proxy_ = 0; // safety
      }
    }

    void print(std::ostream& ostr) const
    {
      assert(proxy_ != 0);
      proxy_->print(ostr);
    }

    std::string type() const
    {
      return type_;
    }

    abstract_data* proxy() const
    {
      assert(proxy_ != 0);
      return proxy_;
    }

    protected:
    abstract_data* proxy_;
    std::string type_;
  };

  NilClass nil_object;
  data nil(nil_object);
}

std::ostream& operator<<(std::ostream& ostr, const dyn::data& d)
{   
  d.print(ostr);
  return ostr;
}     


typedef dyn::data var;
