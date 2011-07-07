#include <my/document/document.hh>

// THIS DEMO SHOW YOU HOW TO EXTRACT INFORMATION ON
// A PARAGRAPH WHEN THE DATA HAVE BEEN
// COMPUTED

template<typename L,typename F,typename D>
void demo_extract_paragraphs(const document<L,F,D>& doc)
{
  
  std::cout << "Demo : extract paragraphs" << std::endl;
      for(int Id = 0; Id < doc.paragraph_count(); Id++)
      {
	// get_paragraph_length_direct must be use because here
	// we have directly the Id of the paragraph
	// NOTE :
	// if we are working on a node and if we want the length of the paragraph that
	// contain the node, the method get_paragraph_length must be called
	  if(doc.get_paragraph_length_direct(Id) > 0) // Check if the paragraph exists
	  {
	    
	    // here we can directly extract all the info that the document contain on the paragraph
	    // by using the methods *_direct
	    std::cout 
	    << "Paragraph : " 
	    << doc.get_paragraph_length_direct(Id)
	    << doc.get_paragraph_bbox_direct(Id)
	    std::endl;
	  }
      }
}