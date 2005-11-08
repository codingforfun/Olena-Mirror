#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <ruby.h>
#include "dl.h"
#include "ruby_stream.hh"
#include "config.hh"
#include "dyn.hh"
#include "mlc.hh"

/*#define dyn_include(file) \
\#include file \
function_loader.include(file);
*/

namespace dyn {

  <% NB_MAX_ARGUMENTS = 10 %>

  struct fun;

  struct data_ret
  {
    data_ret(const fun& fun, const std::vector<data>& arguments, const std::string& arguments_types) :
      fun_(fun), arguments_(arguments), arguments_types_(arguments_types) {}

    template <typename T>
    operator T() const;

    const std::vector<data>& arguments_;
    const std::string& arguments_types_;
    const fun& fun_;
  };


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

    template <int N>
    data
    call_ret2(const std::vector<data>&, const std::string&, const std::string&)
    {
      assert(0);
    }

    <%- NB_MAX_ARGUMENTS.times do |i| -%>

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

      <%- typenames = (0 .. i - 1).map { |j| "typename T#{j}" }.join(', ') -%>
      <%- arguments = (0 .. i - 1).map { |j| "const T#{j}& arg#{j}" }.join(', ') -%>
      <%- objects   = (0 .. i - 1).map { |j| "object#{j}" }.join(', ') -%>

      <%- unless typenames.empty? -%>
      template < <%= typenames %> >
      <%- end -%>
      data_ret
      operator() (<%= arguments %>) const
      {

        std::vector<data> arguments;
        std::string arguments_types;

      <%- (i - 1).times do |j| -%>
        const data object<%= j %> = arg<%= j %>;
        arguments.push_back(object<%= j %>);
        arguments_types += <%= (j.zero?)? '' : '"\", \"" + ' %>object<%= j %>.type();
      <%- end -%>
        return data_ret(*this, arguments, arguments_types);
      }

    <%- end -%>

    void
    call_ret2(const std::vector<data>& arguments,
              const std::string& arguments_types,
              const std::string& ret_type) const
    {
      void* ptr = function_loader.load(header_path_, name_, arguments_types, ret_type);

      assert(ptr != 0);
      switch (arguments.size())
      {
      <%- NB_MAX_ARGUMENTS.times do |i| -%>
      <%- objects = (0 .. i - 1).map { |j| "arguments[#{j}]" }.join(', ') -%>
        case <%= i %>:
        {
          typedef void (*func_t)(<%= (['const dyn::data&'] * i).join(', ') %>);
          ((func_t)ptr)(<%= objects %>);
          break;
        }
      <%- end -%>
      }
    }

    const std::string header_path_;
    const std::string name_;
  };

  template <typename T>
  data_ret::operator T() const
  {
    fun_.call_ret2(arguments_, arguments_types_, mlc_name_of(T()));
  }


} // end of namespace dyn