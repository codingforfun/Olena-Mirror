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
    data_ret(const fun& fun, const std::vector<data>* arguments, const std::string& arguments_types) :
      fun_(fun), arguments_(arguments), arguments_types_(arguments_types) {}

    template <typename T>
    operator T() const;

    const fun& fun_;
    const std::vector<data>* arguments_;
    const std::string arguments_types_;
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

  struct fun
  {
    fun(const std::string& header_path, const std::string& name) :
      header_path_(header_path), name_(name) {}

    template <int N>
    data
    call_ret(const std::vector<data>&, const std::string&, const std::string&)
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
      data_ret
      operator() (<%= arguments %>) const
      {

        std::vector<data>* arguments = new std::vector<data>;
        std::string arguments_types;

      <%- i.times do |j| -%>
        const data object<%= j %> = arg<%= j %>;
        arguments->push_back(object<%= j %>);
        arguments_types += <%= (j.zero?)? '' : '"\", \"" + ' %>object<%= j %>.type();
      <%- end -%>
        return data_ret(*this, arguments, arguments_types);
      }

    <%- end -%>

    void
    call_ret(const std::vector<data>* arguments,
             const data& ret,
             const std::string& arguments_types,
             const std::string& ret_type) const
    {
      void* ptr = function_loader.load(header_path_, name_, arguments_types, ret_type);

      assert(ptr != 0);
      switch (arguments->size())
      {
      <%- NB_MAX_ARGUMENTS.times do |i| -%>
      <%- objects = ((0 .. i - 1).map { |j| "(*arguments)[#{j}]" } << 'ret').join(', ') -%>
        case <%= i %>:
        {
          typedef void (*func_t)(<%= (['const dyn::data&'] * (i + 1)).join(', ') %>);
          assert(ptr != 0);
          ((func_t)ptr)(<%= objects %>);
          break;
        }
      <%- end -%>
        default:
          std::cerr << arguments->size() << std::endl;
          assert(0);
      }
    }

    const std::string header_path_;
    const std::string name_;
  };


  struct proc
  {
    proc(const std::string& header_path, const std::string& name) :
      header_path_(header_path), name_(name) {}

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

    <%- end -%>

    const std::string header_path_;
    const std::string name_;
  };

  template <typename T>
  data_ret::operator T() const
  {
    T ret;
    fun_.call_ret(arguments_, ret, arguments_types_, mlc_name_of(ret));
    return ret;
  }


} // end of namespace dyn
