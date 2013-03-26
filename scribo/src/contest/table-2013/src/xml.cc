#include "xml.hh"

XML::XML(const char* name,
         const char* pdf)
  : _name(name), _pdf(pdf), _table(0), _region(0), _first_time(true)
{
  _xml.open(_name);
  _xml << "<?xml version\"1.0\" encoding=\"UTF-8\"?>" << std::endl
       << "<document filename='" << _pdf << "'>" << std::endl;
}

XML::~XML(void)
{
  _xml << "</document>" << std::endl;
  _xml.close();
}

void  XML::table(const point2d& start,
                 const point2d& end,
                 const unsigned page,
                 const bool     connect)
{
  if (_first_time)
  {
    _xml << "\t<table id='" << _table << "'>" << std::endl;
    ++_table;
    _first_time = false;
  }
  else
  {
    if (!connect)
    {
      _xml << "\t</table>" << std::endl;
      _xml << "\t<table id='" << _table << "'>" << std::endl;
      _region = 0;
      ++_table;
    }
  }

  _xml << "\t\t<region id='" << _region << "' page='" << page + 1 << "'>" << std::endl
       << "\t\t\t<bounding-box x1='" << start[1]
                         << "' y1='" << start[0]
                         << "' x2='" << end[1]
                         << "' y2='" << end[0] << "'/>" << std::endl
       << "\t\t</region>" << std::endl;

  ++_region;
}
