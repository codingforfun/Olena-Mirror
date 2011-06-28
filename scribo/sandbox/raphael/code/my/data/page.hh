#ifndef MLN_DATA_PAGE
#define MLN_DATA_PAGE
#include <my/data/style.hh>
#include <my/data/paragraph.hh>
#include <my/data/line.hh>
#include <my/document/document.hh>
namespace mymln
{
  namespace data
  {
    template<typename L, typename F, typename D>
    class page
    {
      public:
	page()
	{}
	page(const document::document<L,F,D>& doc)
	{
	  doc_ = doc;
	  paragraphs = mln::util::array<paragraph>(0);
	  paragraphs.reserve(doc_.paragraph_count());
	  unsigned int paragraph_count = doc_.paragraph_count();
	  for(int N = 0; N < paragraph_count; N++)
	  {
	    if(doc_.get_paragraph_length_direct(N))
	    {
	      paragraphs.append(paragraph(doc_.get_paragraph_bbox_direct(N), N));
	      paragraphs[paragraphs.size() - 1].set_font_size(doc_.get_paragraph_middle_width_direct(N));
	      paragraphs[paragraphs.size() - 1].set_line_count(doc_.get_paragraph_length_direct(N));
	    }
	  }
	  unsigned int lines_count = doc_.line_count();
	  lines.reserve(lines_count);
	  for(int N = 0; N < lines_count; N++)
	  {
	    lines.append(line(doc_.get_line_bbox_direct(N), N));
	  }
	}
	
	inline void export_HTML(std::string file)
	{
	  fstream filestream(file.c_str(), std::fstream::out);
	  
	  if(filestream.is_open())
	  std::cout << "EXPORT : " + file;
	    filestream << "<html>\n";
	    filestream << "<head>\n";
	    filestream << "<style type=\"text/css\">\n";
	    for(int N = 0; N < paragraphs.size(); N++)
	    {
	      filestream << paragraphs[N].To_HTML_Paragraph_Style();
	        filestream.flush();
	    }
	    filestream << "</style>\n";
	    filestream << "</head>\n";
	    filestream << "<body>\n";
	    for(int N = 0; N < paragraphs.size(); N++)
	    {
	      filestream << paragraphs[N].To_HTML_Paragraph_Body();
	        filestream.flush();
	    } 
	    filestream << "</body>\n";
	    filestream << "</html>\n";
	    filestream.flush();
	    
	  
	    filestream.close();
	    std::cout << "EXPORTED : " + file;
	}
	inline unsigned int paragraph_count()
	{ return paragraphs.size();}
	inline unsigned int line_count()
	{ return lines.size();}

	inline paragraph get_paragraph(unsigned int ID)
	{ return paragraphs[ID];}
	inline line get_line(unsigned int ID)
	{ return lines[ID];}
      private:
	document::document<L,F,D> doc_;
	mln::util::array<paragraph> paragraphs;
	mln::util::array<line> lines;
	template<typename I>
	inline mln::util::array<image2d<I> > extract_lines_(image2d<I>& source)
	{
	   mln::util::array<image2d<I> > array(0);
	   array.reserve(lines.size());
	   
	  for(int N = 0; N < lines.size();N++)
	  {
	   array.append((source | lines[N].bbox()).rw());
	  }
	  return array;
	}
    };
  }
}
#endif