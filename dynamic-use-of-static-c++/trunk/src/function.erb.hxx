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
        std::string arguments_types;

        if (obj_ptr_)
        {
          arguments_types += obj_ptr_->type() + "\", \"";
        }

      <%- i.times do |j| -%>
        arguments_types += arg<%= j %>.type()<%= (j == i - 1)? '' : ' + "\", \""' %>;
      <%- end -%>

        void* ptr = load_function(kind_, name_, arguments_types, header_path_, options_);
        assert(ptr);

        if (obj_ptr_)
        {
          typedef data (*func_t2)(<%= (['const dyn::data&'] * (i + 1)).join(', ') %>);
          return ((func_t2)ptr)(<%= (['*obj_ptr_'] + objects).join(', ') %>);
        }

        return ((func_t)ptr)(<%= objects.join(', ') %>);
      }

    <%- end -%>

} // end of namespace dyn

#endif
