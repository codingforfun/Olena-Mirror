#ifndef FUNCTION_HH
#define FUNCTION_HH

# include <cassert>

namespace dyn
{


  struct data;

  void*
  load_function(const std::string& kind,
                const std::string& name,
                const std::string& arguments_types,
                const std::string& header_path);

  std::string data_type(const data&);

  struct generic_fun
  {

    generic_fun(const data* obj_ptr,
                const std::string& kind,
                const std::string& name,
                const std::string& header_path="") :
      kind_(kind), name_(name), header_path_(header_path) {}

    <%- NB_MAX_ARGUMENTS.times do |i| -%>

      <%- arguments = (0 .. i).map { |j| "const data& arg#{j}" }.join(', ') -%>
      <%- objects   = (0 .. i).map { |j| "arg#{j}" } -%>

      data
      operator() (<%= arguments %>) const
      {
        typedef data (*func_t)(<%= (['const dyn::data&'] * (i + 1)).join(', ') %>);
        typedef data (*func_t2)(<%= (['const dyn::data*'] + ['const dyn::data&'] * (i + 1)).join(', ') %>);
        std::string arguments_types;

      <%- (i + 1).times do |j| -%>
        arguments_types += <%= (j.zero?)? '' : '"\", \"" + ' %>data_type(arg<%= j %>);
      <%- end -%>

        void* ptr = load_function(kind_, name_, arguments_types, header_path_);
        assert(ptr != 0);

        if (kind_ == "method_proc2" || kind_ == "method_fun2")
          return ((func_t2)ptr)(<%= (['obj_ptr_'] + objects).join(', ') %>);

        return ((func_t)ptr)(<%= objects.join(', ') %>);
      }

    <%- end -%>

    const data* obj_ptr_;
    const std::string kind_;
    const std::string name_;
    const std::string header_path_;
  };

  <% %w[ fun proc ctor method_proc method_fun ctor2 ].each do |name| %>
  struct <%= name %> : public generic_fun
  {
    <%= name %>(const std::string& name, const std::string& header_path="") :
      generic_fun(0, ":<%= name %>", name, header_path) {}
  };
  <% end %>

  <% %w[ method_fun2 method_proc2 ].each do |name| %>
  struct <%= name %> : public generic_fun
  {
    <%= name %>(const data* obj_ptr, const std::string& name, const std::string& header_path="") :
      generic_fun(obj_ptr, ":<%= name %>", name, header_path) {}
  };
  <% end %>

} // end of namespace dyn

#endif
