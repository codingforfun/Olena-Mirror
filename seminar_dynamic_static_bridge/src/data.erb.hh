#ifndef DYN_DATA_HH
# define DYN_DATA_HH

# include <string>
# include <cassert>
# include <istream>
# include <ostream>

# include "function.hh"

# include "name_of.hh"

// FIXME
#include <iostream>

namespace dyn {


  // static void my_print(std::ostream& ostr, const T& t)
  // {
    // assert(0);
    // // ostr << *(proxy.obj());
  // }
  // template <typename T>
    // struct data_proxy;

  // data  -->  abstract_data
  //                 ^
  //                 |
  //           data_proxy<T>  -->  T


  // abstract_data

  struct abstract_data
  {
    virtual abstract_data* clone() const = 0;
    virtual void print(std::ostream&) const = 0;
    // virtual void read(std::istream&) = 0;
    // virtual bool is_printable() const = 0;
    virtual std::string type() const = 0;
    virtual ~abstract_data() {}
  };

  // data_proxy<T>

  template <class T>
  struct data_proxy2 : public abstract_data
  {

    data_proxy2(const T& obj) : p_obj_(&obj) {
    }

    data_proxy2(const T* obj) : p_obj_(obj) {
    }

    template <typename V>
    operator V() const
    {
      assert(p_obj_ != 0);
      V ret(*obj());
      return ret;
    }

    T* obj() const
    {
      assert(p_obj_ != 0);
      return const_cast<T*>(p_obj_);
    }

    std::string type() const
    {
      assert(p_obj_ != 0);
      return mlc_name_of(*p_obj_);
    }

    protected:
    const T* p_obj_;
  };

    struct printable {};
    struct not_printable {};
    template <typename printability, typename T>
    // template <typename P>
    struct printer
    {
      static void print(std::ostream& ostr, const data_proxy2<T>& proxy)
      {
        assert(0);
        // ostr << *(proxy.obj());
      }
    };

    template <typename T>
    // template <>
    struct printer<printable, T>
    {
      static void print(std::ostream& ostr, const data_proxy2<T>& proxy)
      {
        ostr << *(proxy.obj());
      }
    };

    template <typename T>
    // template <>
    struct printer<not_printable, T>
    {
      static void print(std::ostream& ostr, const data_proxy2<T>& proxy)
      {
        ostr << "#(" << mlc_name_of(proxy) << ":" << proxy.obj() << ")";
      }
    };

  template <class T>
  struct printability { typedef printable ret; };

# define set_not_printable(TYPE) \
   template <> struct printability< TYPE > { typedef not_printable ret; }

  set_not_printable(std::ostream);
  set_not_printable(std::istream);
  set_not_printable(std::istringstream);
  // template <typename T1, typename T2> struct printability< std::basic_istream<T1, T2> > { typedef std::basic_istream<T1, T2> ret; };
  // and so one ...
  template <class T>
  struct data_proxy : public data_proxy2<T>
  {
    data_proxy(const T& obj) : data_proxy2<T>(obj) {}
    virtual data_proxy<T>* clone() const
    {
      return new data_proxy<T>(*this->p_obj_);
    }

    virtual void print(std::ostream& ostr) const
    {
      // typedef printability<T>::ret T_printability;
      // printer<T_printability, T>::print(ostr, d);
      printer<typename printability<T>::ret, T>::print(ostr, *this);
      // print<typename printability<T>::ret, T>(ostr, d);
      // return ostr;
    }
  };

  // template <class T>
  // struct readable {};

  // template <class T>
  // struct not_readable {};

  // template <class T>
  // struct data_proxy2< not_printable<T> > : public data_proxy2<T>
  // {
    // data_proxy2(const T& obj) : data_proxy2<T>(obj) {}

    // virtual data_proxy2< not_printable<T> >* clone() const
    // {
      // return new data_proxy2< not_printable<T> >(*this->p_obj_);
    // }

    // virtual void print(std::ostream&) const
    // {
      // assert(!"Not printable");
    // }

    // virtual bool is_printable() const
    // {
      // return false;
    // }
  // };

  // template <class T>
  // struct data_proxy2< printable<T> > : public data_proxy2<T>
  // {
    // data_proxy2(const T& obj) : data_proxy2<T>(obj) {}

    // virtual data_proxy2< printable<T> >* clone() const
    // {
      // return new data_proxy2< printable<T> >(*this->p_obj_);
    // }

    // virtual void print(std::ostream& ostr) const
    // {
      // assert(this->p_obj_ != 0);
      // ostr << (*this->p_obj_);
    // }

    // virtual bool is_printable() const
    // {
      // return true;
    // }

  // };

  // template <class T>
  // struct data_proxy2< not_readable<T> > : public data_proxy2<T>
  // {
    // data_proxy2(const T& obj) : data_proxy2<T>(obj) {}

    // virtual data_proxy2< not_readable<T> >* clone() const
    // {
      // return new data_proxy2< not_readable<T> >(*this->p_obj_);
    // }

    // virtual bool is_readable() const
    // {
      // return false;
    // }

    // virtual void read(std::istream&)
    // {
      // assert(!"Not readable");
    // }

  // };

  // template <class T>
  // struct data_proxy2< readable<T> > : public data_proxy2<T>
  // {
    // data_proxy2(const T& obj) : data_proxy2<T>(obj) {}

    // virtual data_proxy2< readable<T> >* clone() const
    // {
      // return new data_proxy2< readable<T> >(*this->p_obj_);
    // }

    // virtual bool is_readable() const
    // {
      // return true;
    // }

    // virtual void read(std::istream& istr)
    // {
      // assert(this->p_obj_ != 0);
      // istr >> (*this->p_obj_);
    // }

  // };


  // template <class T>
  // struct readablity { typedef not_readable<T> ret; };

  //# define set_readable(TYPE)
   // template <> struct readablity< TYPE > { typedef readable< TYPE > ret; }

  // set_readable(std::string);
  // // and so one ...

  // template <class T>
  // struct data_proxy : public data_proxy2< typename printability<T>::ret >,
                      // public data_proxy2< typename readablity<T>::ret >
  // {
    // data_proxy(const T& obj) : data_proxy2< typename printability<T>::ret >(obj),
                               // data_proxy2< typename readablity<T>::ret >(obj) {}
  // };


  // template <class T>
  // struct data_proxy : public data_proxy2< typename printability<T>::ret >
  // {
    // data_proxy(const T& obj) : data_proxy2< typename printability<T>::ret >(obj) {}
  // };


  template <class T>
  struct data_proxy<const T>: public data_proxy<T>
  {
    data_proxy<const T>(const T& obj) : data_proxy<T>(obj) {}

    const T* obj() const
    {
      assert(this->p_obj_ != 0);
      return this->p_obj_;
    }
  };



  struct NilClass {};

  template <>
  struct data_proxy<NilClass> : public abstract_data
  {
    data_proxy(const NilClass&) {}
    data_proxy() {}

    virtual data_proxy<NilClass>* clone() const
    {
      return new data_proxy<NilClass>();
    }

    // virtual void read(std::istream&)
    // {
    // }

    virtual void print(std::ostream& ostr) const
    {
      ostr << "nil";
    }

    // virtual bool is_printable() const
    // {
      // return true;
    // }

    std::string type() const
    {
      return "data_nil";
    }

  };

  template <typename T1, typename T2>
  T2
  data_cast(const T1& src)
  {
    return src;
  }

  struct by_cpy {};

  // data

  struct data
  {

    data& operator=(const data& rhs)
    {
      if (&rhs == this) return *this;
      assert(rhs.proxy_ != 0);
      assert(rhs.proxy_ != proxy_);
      if (proxy_ != 0)
      {
//	  std::cout << "deleting " << proxy_ << std::endl;
// 	  delete proxy_;
      }
      proxy_ = rhs.proxy_->clone();
      type_ = rhs.type_;
      return *this;
    }


    template <typename T>
    T convert_to() const;

    template <typename T>
    operator T() const
    {
      return convert_to<T>();
    }


    ~data()
    {
      // delete ptr_save_;
      // ptr_save_ = 0;
      delete proxy_;
      proxy_ = 0; // safety
    }

    void print(std::ostream& ostr) const
    {
      assert(proxy_ != 0);
      proxy_->print(ostr);
    }

    // bool is_printable() const
    // {
      // assert(proxy_ != 0);
      // return proxy_->is_printable();
    // }

    // void read(std::istream& istr)
    // {
      // assert(proxy_ != 0);
      // // proxy_->read(istr);
      // istr >> *proxy_;
    // }

    std::string type() const
    {
      return type_;
    }

    abstract_data* proxy() const
    {
      assert(proxy_ != 0);
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
    void* ptr_save_;

    public:

    #include "data_gen.hh"

#   ifndef INITIALIZE_METHODS_ATTRIBUTES
#   define INITIALIZE_METHODS_ATTRIBUTES fake_method("fake_method")
    method_fun fake_method;
#   endif

    data() : proxy_(0), type_("unkown"), INITIALIZE_METHODS_ATTRIBUTES {}

    template <class T>
    data(const T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      ptr_save_ = 0;
      proxy_ = new data_proxy<const T>(obj);
      type_ = mlc_name_of(obj);
    }

    template <class T>
    data(const T& obj, const by_cpy*) : INITIALIZE_METHODS_ATTRIBUTES
    {
      // proxy_ = new data_proxy<by_cpy<const T> >(obj);
      ptr_save_ = new T(obj);
      proxy_ = new data_proxy<const T>(*(T*)ptr_save_);
      // proxy_ = new data_proxy<by_cpy<const T> >(obj);
      type_ = mlc_name_of(obj);
    }

    template <class T>
    data(T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      ptr_save_ = 0;
      proxy_ = new data_proxy<T>(obj);
      type_ = mlc_name_of(obj);
    }

    template <class T>
    data(T& obj, const by_cpy*) : INITIALIZE_METHODS_ATTRIBUTES
    {
      // proxy_ = new data_proxy<by_cpy<const T> >(obj);
      ptr_save_ = new T(obj);
      proxy_ = new data_proxy<T>(*(T*)ptr_save_);
      // proxy_ = new data_proxy<by_cpy<const T> >(obj);
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
mlc_set_name_TC(dyn::data_proxy2);
mlc_set_name(dyn::data);
mlc_set_name(dyn::NilClass);

template <>
struct mlc_name2 <dyn::data>
{
  static std::string of(const dyn::data& d) { return d.type(); }
};

std::ostream& operator<<(std::ostream& ostr, const dyn::data& d)
{
  d.print(ostr);
  return ostr;
}

  // template <class T>
  // struct printable {};

  // template <class T>
  // {
    // // typedef T value_type;
    // private:
      // // not_printable<T>() {}
      // not_printable() {}
  // };


namespace dyn {


  // template <typename T>
  // void my_print(std::ostream& ostr, const data_proxy< typename meta_eq<printability<T>::ret, true_type> >& proxy)
  // // void my_print(std::ostream& ostr, const data_proxy< T >& proxy)
  // {
    // ostr << *(proxy.obj());
  // }

  // template <typename T>
  // void my_print(std::ostream& ostr, const data_proxy< typename not_printables<T>::ret >& proxy)
  // {
    // ostr << "#(" << mlc_name_of(proxy) << ":" << proxy.obj() << ")";
  // }

}


// template <typename T>
// std::istream& operator>>(std::istream& istr, dyn::data_proxy<T>& d)
// {
  // return istr;
  // istr >> *d.p_obj_;
// }

// std::istream& operator>>(std::istream& istr, dyn::data_proxy<std::strT>& d)
// {
  // return istr >> *d.p_obj_;
// }

// std::istream& operator>>(std::istream& istr, dyn::data& d)
// {
  // d.read(istr);
  // return istr;
// }

// template <class T>
// std::ostream& operator<<(std::ostream& ostr, const dyn::data_proxy<T>& d)
// {
  // // assert(!"Not printable");
  // dyn::printer<typename printability<T>::ret, T>::print(ostr, d);
  // return ostr;
// }

#endif
