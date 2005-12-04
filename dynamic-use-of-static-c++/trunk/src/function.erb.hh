#ifndef FUNCTION_HH
#define FUNCTION_HH

# include <cassert>
# include <string>

namespace dyn
{


  struct data;

  struct generic_fun
  {

    generic_fun(const std::string kind,
                const std::string name,
                const std::string options="",
                const std::string header_path="",
                const data*       obj_ptr=0) :
      kind_(kind), name_(name), options_(options),
      header_path_(header_path), obj_ptr_(obj_ptr) {}

    <%- DYN_MAX_ARGUMENTS.times do |i| -%>
      data
      operator() (<%= (["const data&"] * i).join(', ') %>) const;
    <%- end -%>

    const std::string kind_;
    const std::string name_;
    const std::string options_;
    const std::string header_path_;
    const data* obj_ptr_;
  };

  <% %w[ fun ctor ].each do |name| %>
  struct <%= name %> : public generic_fun
  {
    <%= name %>(const std::string name,
                const std::string options="",
                const std::string header_path="",
                const data*       obj_ptr=0) :
      generic_fun(":<%= name %>", name, options, header_path, obj_ptr) {}
  };
  <% end %>

  struct op : public generic_fun
  {
    op(const std::string name,
       const std::string options="",
       const std::string header_path="",
       const data*       obj_ptr=0) :
      generic_fun(":op", std::string("operator") + name,
                  options, header_path, obj_ptr) {}
  };

} // end of namespace dyn

#endif
