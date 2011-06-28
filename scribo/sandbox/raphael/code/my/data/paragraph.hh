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
	}
	paragraph(mln::box2d bbox, int ID)
	{
	  ID_ = ID;
	  bbox_ = bbox;
	  style_ = style();
	   LineC = 1;
	}
	void set_line_count(int count)
	{LineC = count;}
	void set_font_size(int px)
	{style_.set_font_size(px);}
	int ID(){return ID_;}
	std::string To_HTML_Paragraph_Style()
	{
	  return style_.To_HTML_Style(ID_, bbox_.pmin()[1] / 2, bbox_.pmin()[0] / 2);
	}

	std::string To_HTML_Paragraph_Body()
	{
	  std::string output = "";
	  output += "<st" + itoa(ID_) + ">\n";
	  for(int N = 0; N < LineC; N++)
	  {
	    output += "!!--FIXME--!! <br> \n";
	  }
	  
	  
	  output += "</st" + itoa(ID_) + ">\n";
	  return output;
	}
	
	mln::box2d bbox()
	{return bbox_;}
      private:
	mln::box2d bbox_;
	style style_;
	int LineC;
	int ID_;
    };
  }
}
#endif