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
//     virtual const std::string mangle() const = 0;
    virtual abstract_data* clone() const = 0;
//     virtual void assign_to(void* lhs) const = 0;
    virtual void print(std::ostream& ostr) const = 0;
    virtual std::string type() const = 0;
    virtual ~abstract_data() {}
  };

  // data_proxy<T>

  template <class T>
  struct data_proxy : public abstract_data
  {
    data_proxy(const T& obj) : p_obj_(&obj) {}

//     virtual const std::string mangle() const
//     {
//       return mangle_arg(*p_obj_);
//     }

    virtual data_proxy<T>* clone() const
    {
      return new data_proxy<T>(*p_obj_);
    }

//     virtual void assign_to(void* lhs) const
//     {
//       *(T*)lhs = *p_obj_;
//     }

    virtual void print(std::ostream& ostr) const
    {
      assert(p_obj_ != 0);
      ostr << (*p_obj_);
    }

    template <class U>
    void assign(const U& rhs) const
    {
      std::cout << "assign " << p_obj_ << std::endl;
      assert(p_obj_ != 0);
      *(const_cast<T*>(p_obj_)) = rhs;
    }

    std::string type() const
    {
      assert(p_obj_ != 0);
      mlc_name_of(*p_obj_);
    }

    const T* p_obj_;
  };

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

    ~data()
    {
      if (proxy_ != 0) {
        delete proxy_;
        proxy_ = 0; // safety
      }
    }

//    bool set_type(const std::string& type);

//     template <class T>
//     operator T() const
//     {
//       assert(proxy_ != 0);
//       // here also check that proxy_ contains a T*
//       T tmp;
//       proxy_->assign_to(&tmp);
//       return tmp;
//     }

//     const std::string mangle() const
//     {
//       assert(proxy_ != 0);
//       return proxy_->mangle();
//     }

    void print(std::ostream& ostr) const
    {
      assert(proxy_ != 0);
      proxy_->print(ostr);
    }

    std::string type() const
    {
      return type_;
    }

    abstract_data* proxy_;
    std::string type_;
  };

}

typedef dyn::data var;
