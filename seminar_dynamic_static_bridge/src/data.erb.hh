#ifndef DYN_DATA_HH
# define DYN_DATA_HH

# include <string>
# include <cassert>

# include "function.hh"

# include "name_of.hh"


namespace dyn {

  // data  -->  abstract_data
  //                 ^
  //                 |
  //           data_proxy<T>  -->  T


  // abstract_data

  struct abstract_data
  {
    virtual abstract_data* clone() const = 0;
    virtual void print(std::ostream&) const = 0;
    virtual bool is_printable() const = 0;
    virtual std::string type() const = 0;
    virtual ~abstract_data() {}
  };

  // data_proxy<T>

  template <class T>
  struct data_proxy2 : public abstract_data
  {
    data_proxy2() {}
  };

  template <class T>
  struct printable {};

  template <class T>
  struct not_printable {};

  template <class T>
  struct data_proxy2< not_printable<T> > : public data_proxy2<T>
  {
    data_proxy2(const T& obj) : p_obj_(&obj) {
    }

    virtual data_proxy2< not_printable<T> >* clone() const
    {
      return new data_proxy2< not_printable<T> >(*p_obj_);
    }

    template <typename V>
    operator V() const
    {
      assert(p_obj_ != 0);
      V ret(*obj());
      return ret;
    }

    virtual void print(std::ostream&) const
    {
      assert(!"Not printable");
    }

    virtual bool is_printable() const
    {
      return false;
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

  template <class T>
  struct data_proxy2< printable<T> > : public data_proxy2< not_printable<T> >
  {
    data_proxy2(const T& obj) : data_proxy2< not_printable<T> >(obj) {}

    virtual void print(std::ostream& ostr) const
    {
      assert(this->p_obj_ != 0);
      ostr << (*this->p_obj_);
    }

    virtual bool is_printable() const
    {
      return true;
    }

  };

  template <class T>
  struct printablity { typedef printable<T> ret; };

# define set_not_printable(TYPE) \
   template <> struct printablity< TYPE > { typedef not_printable< TYPE > ret; }

  set_not_printable(std::ostream);
  // and so one ...

  template <class T>
  struct data_proxy : public data_proxy2< typename printablity<T>::ret >
  {
    data_proxy(const T& obj) : data_proxy2< typename printablity<T>::ret >(obj) {}
  };


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

    virtual void print(std::ostream& ostr) const
    {
      ostr << std::string("nil");
    }

    virtual bool is_printable() const
    {
      return true;
    }

    std::string type() const
    {
      return "data_nil";
    }

  };

  template <typename T1, typename T2>
  T2
  data_cast(const T1& src, const T2*)
  {
    return src;
  }

  // data

  struct data
  {

    data& operator=(const data& rhs)
    {
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

    bool is_printable() const
    {
      assert(proxy_ != 0);
      return proxy_->is_printable();
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
#   define INITIALIZE_METHODS_ATTRIBUTES fake_method("fake_method")
    method_fun fake_method;
#   endif

    data() : proxy_(0), type_("unkown"), INITIALIZE_METHODS_ATTRIBUTES {}

    template <class T>
    data(const T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      proxy_ = new data_proxy<const T>(obj);
      type_ = mlc_name<T>::of();
    }

    template <class T>
    data(T& obj) : INITIALIZE_METHODS_ATTRIBUTES
    {
      proxy_ = new data_proxy<T>(obj);
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

#endif
