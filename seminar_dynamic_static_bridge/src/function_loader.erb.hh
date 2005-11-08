#include <string>
#include <iostream>
#include <sstream>
#include <ruby.h>
#include "dl.h"
#include "ruby_stream.hh"
#include "config.hh"
#include "dyn.hh"

/*#define dyn_include(file) \
\#include file \
function_loader.include(file);
*/

namespace dyn {

  <% NB_MAX_ARGUMENTS = 10 %>

  struct fun;

  struct function_loader_t
  {
    typedef void (*fun_t)();

    function_loader_t()
    {
      ruby_init();
      ruby_script("function_loader.hh");
      ruby_init_loadpath();
      ruby << "require \""
           << SOURCE_DIR
           << "/function_loader.rb"
           << "\"" << ruby::eval;
    }

    void
    include(const std::string& file)
    {
      ruby << "FunctionLoader.include \"" << file << "\"" << ruby::eval;
    }

    void
    include_dir(const std::string& dir)
    {
      ruby << "FunctionLoader.include_dir \"" << dir << "\"" << ruby::eval;
    }

    void* load(const std::string& inc, const std::string& name,
               const std::string& arguments_types, const std::string& ret_type)
    {
      
      ruby << "FunctionLoader.call \"" << inc << "\", \""
           << name << "\", [\"" <<  arguments_types << "\"], \""
           << ret_type << "\"" << ruby::eval;
      return RDLPTR(ruby.last_value())->ptr;
    }

    ruby::stream ruby;
  };

  function_loader_t function_loader;

  data nil;

  struct fun
  {
    fun(const std::string& header_path, const std::string& name) :
      header_path_(header_path), name_(name) {}

    <%- NB_MAX_ARGUMENTS.times do |i| -%>

      <%- typenames = (0 .. i).map { |j| "typename T#{j}" }.join(', ') -%>
      <%- arguments = (0 .. i).map { |j| "const T#{j}& arg#{j}" }.join(', ') -%>
      <%- objects   = (0 .. i).map { |j| "object#{j}" }.join(', ') -%>

      template < <%= typenames %> >
      data
      call_ret(<%= arguments %>) const
      {
        typedef void (*func_t)(<%= (['const dyn::data&'] * (i + 1)).join(', ') %>);

        std::string arguments_types;

      <%- i.times do |j| -%>
        const data object<%= j %> = arg<%= j %>;
        arguments_types += <%= (j.zero?)? '' : '"\", \"" + ' %>object<%= j %>.type();
      <%- end -%>
        const data object<%= i %> = arg<%= i %>;

        func_t ptr = (func_t)function_loader.load(header_path_, name_,
                                                  arguments_types,
                                                  arg<%= i %>.type());
        assert(ptr != 0);
        ptr(<%= objects %>);
        std::cout << "coucou" << std::endl;
        return object<%= i %>;
      }

/*      template < <%= typenames %> >
      T<%= i %>
      operator() (<%= arguments %>) const
      {
        typedef void (*func_t)(<%= (['const dyn::data&'] * i).join(', ') %>);

        std::string arguments_types;

      <%- i.times do |j| -%>
        const data object<%= j %> = arg<%= j %>;
        arguments_types += object<%= j %>.type()<%= (j.zero?)? '' : ' + "\", \""' %>;
      <%- end -%>
        const data_proxy<T<%= i %>> object<%= i %>; // ret value

        func_t ptr = (func_t)function_loader.load(header_path_, name_,
                                                  arguments_types,
                                                  "void"); // mlc_name_of(arg<%= i %>));
        assert(ptr != 0);
        ptr(<%= objects %>);
        return object<%= i %>;
      }
*/
      <%- typenames = (0 .. i - 1).map { |j| "typename T#{j}" }.join(', ') -%>
      <%- arguments = (0 .. i - 1).map { |j| "const T#{j}& arg#{j}" }.join(', ') -%>
      <%- objects   = (0 .. i - 1).map { |j| "object#{j}" }.join(', ') -%>

      <%- unless typenames.empty? -%>
      template < <%= typenames %> >
      <%- end -%>
      void
      operator() (<%= arguments %>) const
      {
        typedef void (*func_t)(<%= (['const dyn::data&'] * i).join(', ') %>);
        std::string arguments_types;

      <%- i.times do |j| -%>
        const data object<%= j %> = arg<%= j %>;
        arguments_types += <%= (j.zero?)? '' : '"\", \"" + ' %>object<%= j %>.type();
      <%- end -%>

        func_t ptr = (func_t)function_loader.load(header_path_, name_,
                                                  arguments_types, "void");
        assert(ptr != 0);
        ptr(<%= objects %>);
      }

    <%- end -%>

    const std::string header_path_;
    const std::string name_;
  };

} // end of namespace dyn