#ifndef DYN_DATA_HXX
# define DYN_DATA_HXX

# include <string>
# include <cassert>
# include <iostream>

# include "function.hh"
# include "name_of.hh"


namespace dyn {

  template <typename T>
  T data::convert_to() const
  {
    static fun dyn_data_cast(std::string("data_cast< ") +
                             type() + ", " + mlc_name<T>::of() + " >");
    return dyn_data_cast(*this);
  }


  template <typename T>
  T& data::get_ref_on()
  {
    assert(proxy_);
    data_proxy_by_ref<T>* reinterpret_cast_returned_pointer = reinterpret_cast<data_proxy_by_ref<T>*>(proxy_);
    assert(reinterpret_cast_returned_pointer);
    return reinterpret_cast_returned_pointer->obj();
  }


  template <typename T>
  const T& data::get_ref_on() const
  {
    assert(proxy_);
    data_proxy<T>* reinterpret_cast_returned_pointer = reinterpret_cast<data_proxy<T>*>(proxy_);
    assert(reinterpret_cast_returned_pointer);
    return reinterpret_cast_returned_pointer->const_ref();
  }


  const data& data::get_const_ref() const
  {
    return *this;
  }


  template <typename T>
  data& data::operator=(const T& rhs)
  {
    assert(proxy_);
    static fun dyn_data_assign(std::string("data_assign<") + proxy()->type() + ", " + mlc_name<T>::of() + ">");
    dyn_data_assign(*this, rhs);
    return *this;
  }


  <%- (NB_MAX_ARGUMENTS - 1).times do |i| -%>

    <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" } -%>
    <%- objects   = (0 .. i - 1).map { |j| "arg#{j}" } -%>

    data
    data::send (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const
    {
      fun meth(meth_name, "method");
      return meth(<%= (['*this'] + objects).join(', ') %>);
    }

  <%- end -%>

  fun data::method(const std::string& method_name)
  {
    fun m(method_name, "method", "", this);
    return m;
  }

  namespace internal {
    op operator_push("<<");
    op operator_pop(">>");
    op operator_incr("++");
    op operator_decr("--");
    op operator_plus("+");
    op operator_star("*");
    op operator_equal("==");
    op operator_not_equal("!=");
    fun operator_square_brackets("operator[]", "method");
  }

  const data& data::operator*() const
  {
    return internal::operator_star(*this).get_const_ref();
  }

  data data::operator*()
  {
    return internal::operator_star(*this);
  }

  data data::operator[](const data& at)
  {
    return internal::operator_square_brackets(*this, at);
  }

  data::data(const language::var& rhs) : proxy_(0), INITIALIZE_METHODS_ATTRIBUTES
  {
    logger << "data(const language::var& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
    assign(rhs);
  }

  data::data(const language::val& rhs) : proxy_(0), INITIALIZE_METHODS_ATTRIBUTES
  {
    logger << "data(const language::val& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
    assign(rhs);
  }

  data::data(language::var& rhs) : proxy_(0), INITIALIZE_METHODS_ATTRIBUTES
  {
    logger << "data(language::var& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
    assign(rhs);
  }

  data::data(language::val& rhs) : proxy_(0), INITIALIZE_METHODS_ATTRIBUTES
  {
    logger << "data(language::val& rhs) [ rhs.type() = " << rhs.type() << " ]" << std::endl;
    assign(rhs); // FIXME should copy it's contents not just the proxy
  }
}

std::ostream& operator<<(std::ostream& ostr, const dyn::data& d)
{
  return dyn::internal::operator_push(ostr, d).get_ref_on<std::ostream>();
}

std::istream& operator>>(std::istream& istr, dyn::data& d)
{
  return dyn::internal::operator_pop(istr, d).get_ref_on<std::istream>();
}

dyn::data& operator++(dyn::data& d)
{
  dyn::internal::operator_incr(d);
  return d;
}

dyn::data& operator--(dyn::data& d)
{
  dyn::internal::operator_decr(d);
  return d;
}

bool operator!=(const dyn::data& lhs, const dyn::data& rhs)
{
  return dyn::internal::operator_not_equal(lhs, rhs);
}

bool operator==(const dyn::data& lhs, const dyn::data& rhs)
{
  return dyn::internal::operator_equal(lhs, rhs);
}


dyn::data operator+(const dyn::data& lhs, const dyn::data& rhs)
{
  return dyn::internal::operator_plus(lhs, rhs);
}


#endif
