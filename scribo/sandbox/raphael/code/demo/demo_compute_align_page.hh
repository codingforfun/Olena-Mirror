  #include <my/document/document.hh>

  // THIS DEMO SHOW YOU HOW COMPUTE ALIGNEMENT WITH A PAGE

  template<typename L,typename F,typename D>
  void demo_compute_align_page(mymln::document::document<L,F,D>& doc)
  {
    

    	  // NOTE : We must use the method contain_implicit_separator_fixed
	  // and not contain_implicit_separator, because contain_implicit_separator performs
	  // opperation on potential implicit separators and contain_implicit_separator_fixed
	  // performs operations on the detected implicit separators.
	  // However contain_implicit_separator_right and contain_implicit_separator_left
	  // can always be used.

	std::cout << "   ==   Demo : compute allignement with page   ==   " << std::endl;

	// NOTE : THE FOLLOWING CODE DOEXACTLY THE SAME THING AS THE
	// DEMO demo_compute_align.hh BUT WITH PAGE
	// THE CODE BECOMES SMALLER
	// WARNING : WE GENERATE A PAGE HERE FOR THE DEMO BUT FOR A REAL
	// PROGRAM THE SYSTEM MUST NOT RECREATE A PAGE AGAIN AND AGAIN
	
	mymln::data::page<L,F,D> page = doc;
	page.compute_alignements();
	
	for(int Id = 0; Id < page.paragraph_count(); Id++)
	{
	   std::cout 	<< "Paragraph "
			<< Id
			<< " is "
			<< page.get_paragraph(Id).get_style().get_text_align_string()
			<< std::endl;
	}
	std::cout << "PRESS ENTER TO CONTINUE"; std::cin.get();
  } 
