#ifndef MLN_DATA_PAGE
#define MLN_DATA_PAGE
#include <my/data/style.hh>
#include <my/data/paragraph.hh>
#include <my/data/line.hh>
#include <my/data/image.hh>
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
	  images = mln::util::array<image>(0);
	  paragraphs.reserve(doc_.paragraph_count());
	  unsigned int paragraph_count = doc_.paragraph_count();
	  
	  mln::util::array<int> paragraphs_IDS;
	  paragraphs_IDS.reserve(doc_.paragraph_count());
	  
	  for(int N = 0; N < paragraph_count; N++)
	  {
	    
	    if(doc_.get_paragraph_length_direct(N))
	    {
	      paragraphs.append(paragraph(doc_.get_paragraph_bbox_direct(N), N));
	      paragraphs[paragraphs.size() - 1].set_font_size(doc_.get_paragraph_middle_width_direct(N));
	      paragraphs[paragraphs.size() - 1].set_line_count(doc_.get_paragraph_length_direct(N));
	      paragraphs_IDS.append(paragraphs.size() - 1);
	    }
	    else
	    {
	      paragraphs_IDS.append(-1);
	    }
	  }
	  unsigned int lines_count = doc_.line_count();
	  lines.reserve(lines_count);
	  for(int N = 0; N < lines_count; N++)
	  {
	    if(doc_.get_line_length_direct(N))
	    {
	      lines.append(line(doc_.get_line_bbox_direct(N), N));
	      if(paragraphs_IDS[doc_.get_line_parent(N)] >= 0)
	      {
		paragraphs[paragraphs_IDS[doc_.get_line_parent(N)]].add_line(
						lines[lines.size() - 1],
						doc_.contain_start_paragraph_direct(N),
						doc_.contain_end_paragraph_direct(N)
									    );
	      }
	    }
	  }
	}
	
	void set_source_image(image2d<value::rgb8> source)
	{
	  src_ = source;
	}
	void add_line(data::line Line)
	{
	  lines.append(Line);
	}
	void add_paragraph(data::paragraph Paragraph)
	{
	  paragraphs.append(Paragraph);
	}
	void add_image(data::image Image)
	{
	  images.append(Image);
	}
	void compute_alignements()
	{
	  for(int N = 0; N < paragraphs.size(); N++)
	  {
	    paragraphs[N].compute_alignements();
	  }
	}
	inline void export_PRima(std::string file, const char qname)
	{
	  
	}
	inline void export_HTML(std::string file)
	{
	  fstream filestream(file.c_str(), std::fstream::out);
	  
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
	}
	
	inline unsigned int paragraph_count()
	{ return paragraphs.size();}
	inline unsigned int line_count()
	{ return lines.size();}
	inline unsigned int image_count()
	{ return images.size();}
	
	inline paragraph get_paragraph(unsigned int ID)
	{ return paragraphs[ID];}
	inline line get_line(unsigned int ID)
	{ return lines[ID];}
	inline line get_image(unsigned int ID)
	{ return images[ID];}
      private:
	image2d<value::rgb8> src_;
	document::document<L,F,D> doc_;
	mln::util::array<paragraph> paragraphs;
	mln::util::array<line> lines;
	mln::util::array<image> images;
    };
  }
}
#endif