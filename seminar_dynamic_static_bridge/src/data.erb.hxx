#ifndef DYN_DATA_HXX
# define DYN_DATA_HXX

# include <string>
# include <cassert>

# include "function.hh"

# include "name_of.hh"

namespace dyn {

  template <typename T>
  T data::convert_to() const
  {
    assert(proxy_);
    data_proxy<T>* dynamic_cast_returned_pointer = reinterpret_cast<data_proxy<T>*>(proxy_);
    if (dynamic_cast_returned_pointer != 0)
    {
      return *dynamic_cast_returned_pointer->obj();
    }
    else
    {
      static fun dyn_data_cast(std::string("data_cast<") + mlc_name<T>::of() + ">");
      return dyn_data_cast(*this);
    }
  }

  <%- (NB_MAX_ARGUMENTS - 1).times do |i| -%>

    <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" } -%>
    <%- objects   = (0 .. i - 1).map { |j| "arg#{j}" } -%>

    data*
    data::operator() (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const
    {
      method_fun meth(meth_name);
      return meth(<%= (['*this'] + objects).join(', ') %>);
    }

    data*
    data::v (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const
    {
      method_proc meth(meth_name);
      return meth(<%= (['*this'] + objects).join(', ') %>);
    }

  <%- end -%>

}

#endif
