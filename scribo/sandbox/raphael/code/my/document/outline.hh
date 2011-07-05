using namespace mln;
namespace mymln
{
  namespace document
  {
    

    
    template <typename I, typename N>
    image2d<bool> outline(const Image<I>& iz_, const Neighborhood<N>& nbh_)
    {

      const I& iz = exact(iz_);
      const N& nbh = exact(nbh_);

      
     image2d<bool> out(iz.domain());
     
      mln_pixter(const I)    p(iz);
      mln_nixter(const I, N) n(p, nbh);
   
      for_all(p)
      {
	unsigned int count = 0;
	int variate = 0;
	for_all(n)
	{
	    variate += n.val();
	    count++;
	}
	variate /= count;
	variate -= p.val();
	if(variate > 10)
	{
	  out.element(p.offset()) = true;
	}
      }
      
   
      for_all(p)
      {
	unsigned int count = 0;
	int variate = 0;
	if(out.element(p.offset()))
	{
	  for_all(n)
	  {
	    if(out.element(n.offset()))
	      count++;
	  }
	  if(count == 0)
	  {
	    out.element(p.offset()) = false;
	  }
	}
      }
      
      
      
      return out;
    }
    template <typename I, typename N>
    image2d<bool> threshold(const Image<I>& iz_, const Neighborhood<N>& nbh_)
    {

      const I& iz = exact(iz_);
      const N& nbh = exact(nbh_);

      
     image2d<bool> out(iz.domain());
     
      mln_pixter(const I)    p(iz);
      mln_nixter(const I, N) n(p, nbh);
   
      for_all(p)
      {
	unsigned int count = 0;
	int variate = 0;
	for_all(n)
	{
	    variate += n.val();
	    count++;
	}
	variate /= count;
	variate -= p.val();
	if(variate > 10)
	{
	  out.element(p.offset()) = true;
	}
      }
      
   
      for_all(p)
      {
	unsigned int count = 0;
	int variate = 0;
	if(out.element(p.offset()))
	{
	  for_all(n)
	  {
	    if(out.element(n.offset()))
	      count++;
	  }
	  if(count == 0)
	  {
	    out.element(p.offset()) = false;
	  }
	}
      }
      
      
      
      return out;
    }
    template <typename I, typename N>
    image2d<value::int_u8> luminance(const Image<I>& iz_)
    {

      const I& iz = exact(iz_);

      
     image2d<value::int_u8> out(iz.domain());
     
      mln_pixter(const I)    p(iz);  
      for_all(p)
      {
	  value::rgb8 V = p.val();
	  out.element(p.offset()) = ( (V.red() + V.blue() + V.green()) / 9 );
	  out.element(p.offset()) *= 3;
      }
      return out;
    }
    
        template <typename I>
    image2d<bool> binarize_outline(const Image<I>& iz_)
    {return mymln::document::outline(mymln::document::luminance(iz_), c8());}
    
    
    
  }
}