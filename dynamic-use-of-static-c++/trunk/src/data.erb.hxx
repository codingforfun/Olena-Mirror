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
  T& data::get_ref_on() const
  {
    assert(proxy_);
    data_proxy<T>* reinterpret_cast_returned_pointer = reinterpret_cast<data_proxy<T>*>(proxy_);
    assert(reinterpret_cast_returned_pointer);
    return *reinterpret_cast_returned_pointer->obj();
  }


  const data& data::get_const_ref() const
  {
    return *this;
  }


  template <typename T>
  data& data::operator=(const T& rhs)
  {
    assert(proxy_);
    static proc dyn_data_assign(std::string("data_assign<") + type() + ", " + mlc_name<T>::of() + ">");
    type_ = mlc_name_of(rhs);
    dyn_data_assign(*this, rhs);
    return *this;
  }


  <%- (NB_MAX_ARGUMENTS - 1).times do |i| -%>

    <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" } -%>
    <%- objects   = (0 .. i - 1).map { |j| "arg#{j}" } -%>

    data*
    data::operator() (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const
    {
      fun meth(meth_name, "method");
      return meth(<%= (['*this'] + objects).join(', ') %>);
    }

    data*
    data::v (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const
    {
      proc meth(meth_name, "method");
      return meth(<%= (['*this'] + objects).join(', ') %>);
    }

  <%- end -%>


  namespace internal {
    op operator_push("<<");
    op operator_pop(">>");
    op operator_incr("++");
    op operator_decr("--");
    op operator_star("*");
    op operator_not_equal("!=");
    fun operator_square_brackets("operator[]", "method, lvalue");
  }

  // FIXME
  const data& data::operator*() const
  {
    return internal::operator_star(*this).get_const_ref();
  }

  data data::operator*()
  {
    return internal::operator_star(*this);
  }

  // FIXME is the ref good ?
  const data& data::operator[](const data& at) const
  {
    return internal::operator_square_brackets(*this, at).get_const_ref();
  }

  data data::operator[](const data& at)
  {
    return internal::operator_square_brackets(*this, at);
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


#endif
