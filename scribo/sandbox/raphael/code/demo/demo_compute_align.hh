  #include <my/document/document.hh>

  // THIS DEMO SHOW YOU HOW COMPUTE ALIGNEMENT

  template<typename L,typename F,typename D>
  void demo_compute_align(mymln::document::document<L,F,D>& doc)
  {
    

    	  // NOTE : We must use the method contain_implicit_separator_fixed
	  // and not contain_implicit_separator, because contain_implicit_separator performs
	  // opperation on potential implicit separators and contain_implicit_separator_fixed
	  // performs operations on the detected implicit separators.
	  // However contain_implicit_separator_right and contain_implicit_separator_left
	  // can always be used.

	std::cout << "   ==   Demo : compute allignement   ==   " << std::endl;

	mln::util::array<int> decal_left(doc.paragraph_count());
	mln::util::array<int> decal_right(doc.paragraph_count());
	for(int Id = 1; Id < doc.line_count(); Id++) 
	{
	  if(doc.get_line_length_direct(Id) && doc.get_line_parent(Id))
	  {
	    if(
	      doc.get_paragraph_length_direct(doc.get_line_parent(Id)) &&
	      !doc.contain_start_paragraph_direct(Id) &&
	      !doc.contain_end_paragraph_direct(Id)
	      ) // We eliminate the irst and the last line because they can contain tab.
	    {
	      decal_left[doc.get_line_parent(Id)] +=
		(
		  doc.get_line_bbox_direct(Id).pmin()[1] - 
		  doc.get_paragraph_bbox_direct(doc.get_line_parent(Id)).pmin()[1]
		);
	      decal_right[doc.get_line_parent(Id)] +=
		(
		  
		  doc.get_paragraph_bbox_direct(doc.get_line_parent(Id)).pmax()[1] -
		  doc.get_line_bbox_direct(Id).pmax()[1]
		);
	    }
	  }
	}
	
	

	for(int Id = 1; Id < doc.paragraph_count(); Id++)
	{
	  
	    if(doc.get_paragraph_length_direct(Id))
	    {
	      std::cout << "Paragraph "<< Id << " is ";
	      decal_right[Id] /= doc.get_paragraph_length_direct(Id);
	      decal_left[Id] /= doc.get_paragraph_length_direct(Id);
	      bool dock_left = (decal_left[Id] <= doc.get_paragraph_bbox_direct(Id).len(1) / 40);
	      bool dock_right = (decal_right[Id] <= doc.get_paragraph_bbox_direct(Id).len(1) / 40);
	      if(dock_left && dock_right)
		std::cout << "Justified\n";
	      else if(dock_left)
		std::cout << "Left\n";
	      else if(dock_right)
		std::cout << "Right\n";
	      else
		std::cout << "Center\n";
	    }
	}

	std::cout << "PRESS ENTER TO CONTINUE"; std::cin.get();
  }