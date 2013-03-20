#ifndef XML_HH
# define XML_HH
# define MLN_WO_GLOBAL_VARS

# include <iostream>
# include <fstream>
# include <mln/core/alias/point2d.hh>

using namespace mln;

class XML
{
  public:
    XML(const char* name, const char* pdf);
    ~XML();
    void  write_table(const point2d& start, const point2d& end);

  private:
    std::ofstream _xml;
    const char*   _name;
    const char*   _pdf;
};

#endif /* !XML_HH */
