#ifndef DYN_HH
# define DYN_HH

# include "config.hh"

# include "data.hh"

# include "function.hxx"

namespace dyn
{
  namespace language
  {
    struct val;

    struct var : public data
    {
      var() : data() { logger << "var()" << std::endl; }

      template <class T>
      var(const T& obj) : data((abstract_data*)new data_proxy_by_cpy<T>(obj), (proxy_tag*)0)
      {
        logger << "var(const T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl;
      }

      template <class T>
      var(T& obj) : data(obj) { logger << "var(T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl; }

      var(const data& rhs) : data(rhs) { logger << "var(const data& rhs)" << std::endl; }
      var(const var& rhs)  : data(rhs) { logger << "var(const var& rhs)" << std::endl; }
      var(const val& rhs)  : data(rhs) { logger << "var(const val& rhs)" << std::endl; }
      var(data& rhs) : data(rhs) { logger << "var(data& rhs)" << std::endl; }
      var(var& rhs)  : data(rhs) { logger << "var(var& rhs)" << std::endl; }
      var(val& rhs)  : data(rhs) { logger << "var(val& rhs)" << std::endl; }
    };

    struct val : public data
    {
      template <class T>
      val(const T& obj) : data(obj) { logger << "val(const T& obj) [ T = " << mlc_name<T>::of() << " ]" << std::endl; }

      val(const data& rhs) : data(rhs) { logger << "val(const data& rhs)" << std::endl; }
      val(const var& rhs)  : data(rhs) { logger << "val(const var& rhs)" << std::endl; }
      val(const val& rhs)  : data(rhs) { logger << "val(const val& rhs)" << std::endl; }
    };

    typedef ::dyn::fun  fun;
    typedef ::dyn::ctor ctor;
  }
}


# include "data.hxx"

# include "function_loader.hh"

#endif

