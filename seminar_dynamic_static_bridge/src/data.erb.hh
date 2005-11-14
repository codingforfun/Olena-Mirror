#ifndef DYN_DATA_HH
# define DYN_DATA_HH

# include <string>
# include <cassert>

# include "mlc.hh"

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
    virtual std::string type() const = 0;
    virtual ~abstract_data() {}
  };

  // data_proxy<T>

  template <class T>
  struct data_proxy : public abstract_data
  {
    data_proxy(const T& obj) : p_obj_(&obj) {
    }

    virtual data_proxy<T>* clone() const
    {
      return new data_proxy<T>(*p_obj_);
    }

    template <typename V>
    operator V() const
    {
      assert(p_obj_ != 0);
      V ret(*obj());
      return ret;
    }

    virtual void print(std::ostream& ostr) const
    {
      assert(p_obj_ != 0);
      ostr << (*p_obj_);
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
  struct data_proxy<const T>: public abstract_data
  {
    data_proxy(const T& obj) : p_obj_(&obj) {
    }

    virtual data_proxy<T>* clone() const
    {
      return new data_proxy<T>(*p_obj_);
    }

    template <typename V>
    operator V() const
    {
      assert(p_obj_ != 0);
      V ret(*obj());
      return ret;
    }

    const T* obj() const
    {
      assert(p_obj_ != 0);
      return p_obj_;
    }

    virtual void print(std::ostream& ostr) const
    {
      assert(p_obj_ != 0);
      ostr << (*p_obj_);
    }

    std::string type() const
    {
      assert(p_obj_ != 0);
      return mlc_name_of(*p_obj_);
    }

    protected:
    const T* p_obj_;
  };


  struct NilClass {};

  template <>
  struct data_proxy<NilClass> : public abstract_data
  {
    data_proxy(const NilClass& nil_object) {}
    data_proxy() {}

    virtual data_proxy<NilClass>* clone() const
    {
      return new data_proxy<NilClass>();
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

  template <typename T1, typename T2>
  T2
  data_cast(const T1& src, const T2*)
  {
    return src;
  }

  // data

  struct data
  {
    data() : proxy_(0), type_("unkown") {
    }

    template <class T>
    data(const T& obj)
    {
      proxy_ = new data_proxy<const T>(obj);
      type_ = mlc_name<T>::of();
    }

    template <class T>
    data(T& obj)
    {
      proxy_ = new data_proxy<T>(obj);
      type_ = mlc_name_of(obj);
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

    std::string type() const
    {
      return type_;
    }

    abstract_data* proxy() const
    {
      assert(proxy_ != 0);
      return proxy_;
    }

    <%- NB_MAX_ARGUMENTS.times do |i| -%>

      <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" } -%>
      data*
      data::operator() (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const;

      <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" } -%>
      data*
      data::v (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const;

    <%- end -%>

    #include "data_gen.hh"

    protected:
    abstract_data* proxy_;
    std::string type_;
  };

  NilClass nil_object;
  data nil(nil_object);

  std::string data_type(const data& d)
  {
    return d.type();
  }

}

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
