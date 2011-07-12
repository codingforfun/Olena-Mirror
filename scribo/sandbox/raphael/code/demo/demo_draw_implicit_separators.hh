  #include <my/document/document.hh>

  // THIS DEMO SHOW YOU HOW TO OBTAIN THE IMPLICIT
  // SEPARATOR

  template<typename L,typename F,typename D>
  void demo_draw_implicit_separators(mymln::document::document<L,F,D>& doc)
  {
    
    std::cout << "   ==   Demo : draw implicit separators   ==   " << std::endl;

	doc.debug_create_buffer();
	for(int Id = 1; Id < doc.size(); Id++) 
	{
	  // NOTE : We must use the method contain_implicit_separator_fixed
	  // and not contain_implicit_separator, because contain_implicit_separator performs
	  // opperation on potential implicit separators and contain_implicit_separator_fixed
	  // performs operations on the detected implicit separators.
	  // However contain_implicit_separator_right and contain_implicit_separator_left
	  // can always be used.
	  if(
	    doc.contain_letter(Id) &&
	    doc.contain_implicit_separator_fixed(Id) &&
	    (doc.contain_start_line(Id) || doc.contain_end_line(Id))
	    )
	  {
	    point2d start = doc.get_line_bbox(Id).to_larger(3).pmin();
	    point2d end = doc.get_line_bbox(Id).to_larger(3).pmax();
	    
	    if(doc.contain_implicit_separator_right(Id))
	      start[1] = end[1];
	    else
	      end[1] = start[1];
	    
	    doc.debug_draw_line_green_buffer(start, end);
	  }
	}
	doc.debug_breakpoint();

    std::cout << "PRESS ENTER TO CONTINUE"; std::cin.get();
  } 
