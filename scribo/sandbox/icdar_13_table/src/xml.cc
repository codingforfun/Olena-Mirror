#include "xml.hh"

XML::XML(const char* name, const char* pdf)
  : _name(name), _pdf(pdf)
{
  _xml.open(_name);
  _xml << "<?xml version\"1.0\" encoding=\"UTF-8\"?>" << std::endl
    << "<document filename='" << _pdf << "'>" << std::endl;
}

XML::~XML()
{
  _xml << "</document>" << std::endl;
  _xml.close();
}

void  XML::write_table(const point2d& start, const point2d& end)
{
  static unsigned table = 0;
  static unsigned region = 0;
  static unsigned page = 1;

  _xml << "\t<table id='" << table << "'>" << std::endl
    << "\t\t<region id='" << region << "' page='" << page << "'>" << std::endl
    << "\t\t<bounding-box x1='" << start[1] << "' y1='" << start[0] << "' "
    << "x2='" << end[1] << "' y2='" << end[0] << "'/>" << std::endl
    << "\t\t</region>" << std::endl
    << "\t</table>" << std::endl;

  ++table;
}
