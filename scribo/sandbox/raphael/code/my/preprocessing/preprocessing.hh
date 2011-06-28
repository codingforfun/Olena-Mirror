 using namespace mln; 
  namespace mymln
   {
     namespace preprocessing
     {
   template <typename I, typename N>
    void denoise(Image<I>& iz_, const Neighborhood<N>& nbh_)
    {

      I& iz = exact(iz_);
      const N& nbh = exact(nbh_);

      
     
     
      mln_pixter(I)    p(iz);
      mln_nixter(I, N) n(p, nbh);
     
      for_all(p)
      {
	unsigned int count = 0;
	int variate = 0;
	  for_all(n)
	  {
	    if(iz.element(n.offset()))
	      count++;
	  }
	if(iz.element(p.offset()))
	{
	  if(count < 2)
	  {
	    iz.element(p.offset()) = false;
	  }
	}
	else
	{
	  if(count > 5)
	  {
	    iz.element(p.offset()) = true;
	  }
	}
	  
      
      }
    }
     } // END NAMESPACE PREPROCESSING
   }// END NAMESPACE MYMLN
      