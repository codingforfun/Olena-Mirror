#ifndef XML_HH
# define XML_HH

# include <iostream>
# include <fstream>
# include <mln/core/alias/point2d.hh>

using namespace mln;

class XML
{
  public:
    XML(const char* name, const char* pdf);
    ~XML();
    void  table(const point2d& start,
                const point2d& end,
                const unsigned page,
                const bool     connect);

  private:
    std::ofstream _xml;
    const char*   _name;
    const char*   _pdf;
    unsigned      _table;
    unsigned      _region;
    bool          _first_time;
};

#endif /* !XML_HH */
