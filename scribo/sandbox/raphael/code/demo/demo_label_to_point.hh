#include <my/document/document.hh>

// THIS DEMO SHOW YOU HOW TO OBTAIN THE POINT THAT
// IS THE BOUNDING BOX CENTER OF THE COMPONENT

template<typename L,typename F,typename D>
void demo_extract_paragraphs(const document<L,F,D>& doc)
{
  
  std::cout << "Demo : label to point" << std::endl;
      for(int Id = 0; Id < doc.size(); Id++)
      {
	// Note that if you have got a point and if the want the label
	// you can use exactly the same syntax
	// label = document[point]
	    std::cout 
	    << "The center of : " 
	    << Id
	    << " is : "
	    << doc[Id]
	    << std::endl;
	  }
      }
}