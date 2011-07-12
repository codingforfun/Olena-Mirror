#include <my/document/document.hh>

// THIS DEMO SHOW YOU HOW TO OBTAIN THE POINT THAT
// IS THE BOUNDING BOX CENTER OF THE COMPONENT

template<typename L,typename F,typename D>
void demo_label_to_point(mymln::document::document<L,F,D>& doc)
{
  
  std::cout << "   ==   Demo : label to point   ==   " << std::endl;

      for(int Id = 0; Id < doc.size() && Id < 20; Id++) //only the 20 first labels are displayed
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
  std::cout << "PRESS ENTER TO CONTINUE"; std::cin.get();
}