
      for (int i = 0; i < S.size(); ++i)
	{
	  P p = S[i],
	    q = parent(p),
	    r = parent(q);
	  if (is_not_2_face(q))
	    parent(p) = r;
	}    

//       if (log)
	{
	  std::cout << std::endl
		    << "after removing 0- and 1-faces:" << std::endl;
	  print_tree_stats(parent, S, K_);
	}

	unsigned count_parent = 0;
	for (int i = 0; i < S.size(); ++i)
	  {
	    P p = S[i], q = parent(p);
	    if (is_not_2_face(p) || q == p)
	      continue;
	    if (is_not_2_face(q))
	      ++count_parent;
	  }    
	std::cout << "### " << count_parent << std::endl;
