#ifndef FUNCTION_HXX
#define FUNCTION_HXX

# include <cassert>
# include <string>

namespace dyn
{

    <%- NB_MAX_ARGUMENTS.times do |i| -%>

      <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" }.join(', ') -%>
      <%- objects   = (0 .. i - 1).map { |j| "arg#{j}" } -%>

      data
      generic_fun::operator() (<%= arguments %>) const
      {
        typedef data (*func_t)(<%= (['const dyn::data&'] * i).join(', ') %>);
        typedef data (*func_t2)(<%= (['const dyn::data*'] + ['const dyn::data&'] * i).join(', ') %>);
        std::string arguments_types;

      <%- i.times do |j| -%>
        arguments_types += <%= (j.zero?)? '' : '"\", \"" + ' %>data_type(arg<%= j %>);
      <%- end -%>

        void* ptr = load_function(kind_, name_, arguments_types, header_path_, options_);
        assert(ptr != 0);

        if (kind_ == "method_proc2" || kind_ == "method_fun2")
        {
          assert(obj_ptr_ != 0);
          return ((func_t2)ptr)(<%= (['obj_ptr_'] + objects).join(', ') %>);
        }

        return ((func_t)ptr)(<%= objects.join(', ') %>);
      }

    <%- end -%>

} // end of namespace dyn

#endif
