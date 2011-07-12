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
	  first_line_id_ = 0;
	  last_line_id_ = 0;
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
	{LineC = count;lines_.reserve(count);}
	void add_line(line Line)
	{lines_.append(Line);}
	void add_line(line Line, bool first_line, bool last_line)
	{
	  lines_.append(Line);
	  if(first_line){first_line_id_ = lines_.size() - 1;}
	  if(last_line){last_line_id_ = lines_.size() - 1;}
	}
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
	
	style get_style()
	{ return style_;}
	
	void compute_alignements()
	{
	  unsigned int decal_left = 0;
	  unsigned int decal_right = 0;
	  for(int N = 0; N < lines_.size(); N++)
	  {
	    if(N == first_line_id_ || N == last_line_id_) continue;
	    decal_left += lines_[N].bbox().pmin()[1] -  bbox_.pmin()[1];
	    decal_right += bbox_.pmax()[1] - lines_[N].bbox().pmax()[1];
	  }
	  decal_left /= lines_.size();
	  decal_right /= lines_.size();
	  bool dock_left = (decal_left <= bbox_.len(1) / 40);
	  bool dock_right = (decal_right <= bbox_.len(1) / 40);
	  if(dock_left && dock_right)
	    style_.set_text_align(Justify_Left);
	  else if(dock_left)
	    style_.set_text_align(Left);
	  else if(dock_right)
	    style_.set_text_align(Right);
	  else
	    style_.set_text_align(Center);
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
	    output += lines_[N].get_value();
	    output += "</br> \n";	    
	  }
	  
	  
	  output += "</st" + itoa(ID_) + ">\n";
	  return output;
	}
	
	mln::box2d bbox()
	{return bbox_;}
      private:
	mln::box2d bbox_;
	mln::util::array<line> lines_;
	style style_;
	int LineC;
	int ID_;
	unsigned int first_line_id_;
	unsigned int last_line_id_;
    };
  }
}
#endif