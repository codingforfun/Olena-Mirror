#ifndef MLN_DATA_PARAGRAPH
#define MLN_DATA_PARAGRAPH
#include <my/data/style.hh>
#include <my/data/line.hh>
namespace mymln
{
  namespace data
  {
    class paragraph
    {
      public:
	
	paragraph()
	{
	  ID_ = 0;
	  bbox_ = mln::box2d();
	  style_ = style();
	  LineC = 1;
	  mln::util::array<line>();
	}
	paragraph(mln::box2d bbox, int ID)
	{
	  ID_ = ID;
	  bbox_ = bbox;
	  style_ = style();
	   LineC = 1;
	   mln::util::array<line>();
	}
	void set_line_count(int count)
	{LineC = count;lines.reserve(count);}
	void add_line(line Line)
	{lines.append(Line);}
	int line_count(int count)
	{return LineC;}
	void set_font_size(int px)
	{style_.set_font_size(px);}
	int get_font_size()
	{ style_.get_font_size(); }
	int ID(){return ID_;}
	std::string To_HTML_Paragraph_Style()
	{
	  return style_.To_HTML_Style(ID_, bbox_.pmin()[1] / 2, bbox_.pmin()[0] / 2);
	}

	std::string To_HTML_Paragraph_Body()
	{
	  std::string output = "";
	  output += "<st" + itoa(ID_) + ">\n";
	  output +=  "X:";
	  output +=  itoa(bbox_.pmin()[1]);
	  output +=  "Y:";
	  output +=  itoa(bbox_.pmin()[0]);
	  for(int N = 0; N < LineC; N++)
	  {
	    output += lines[N].get_value();
	    output += "</br> \n";	    
	  }
	  
	  
	  output += "</st" + itoa(ID_) + ">\n";
	  return output;
	}
	
	mln::box2d bbox()
	{return bbox_;}
      private:
	mln::box2d bbox_;
	mln::util::array<line> lines;
	style style_;
	int LineC;
	int ID_;
    };
  }
}
#endif