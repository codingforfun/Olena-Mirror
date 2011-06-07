#ifndef INC_DOCUMENT_UNION
#define INC_DOCUMENT_UNION
namespace mymln
{
  namespace util
  {
    template<typename Label>
    class union_find
    {
      public :
	union_find()
	{size_ = 0;}
	union_find(const unsigned int max_size)
	{
	 mark = mln::util::array<unsigned int>(max_size);
	 mark_link = mln::util::array<unsigned int>(max_size);
	 mark.fill(0);
	 mark_link.fill(0);
	 size_ = max_size;
	}
	inline void reset()
	{
	 mark.fill(0);
	 mark_link.fill(0);
	}
	inline bool is_self_link(const Label A)
	{return mark_link[A] == A;}
	inline void invalidate_link(const Label A)
	{ mark_link[A] = 0; }
	inline void add_self_link(const Label A)
	{ mark_link[A] = A; }
	inline unsigned int link(const unsigned int index)
	{return mark_link[index]; }
	inline void add_link(const Label A, const Label B)
	{

	   unsigned int Pos = find_parent_(A);
	   if(mark_link[B] == 0)
	   {
	    if(Pos != B)
	    {
	      if(Pos != 0)
	      {
		mark_link[B] = Pos;
		mark_link[A] = Pos;
	      }
	      else
	      {
		mark_link[A] = B;
	      }
	    }
	   }
	   else
	   {
	     unsigned int PosB = find_parent_(B);
	     if(PosB == Pos)
	     {
	       	mark_link[B] = Pos;
		mark_link[A] = Pos;
	     }
	     else
	     {
	        mark_link[B] = Pos;
		mark_link[PosB] = Pos;
	     }
	   }    
	}
	inline void propage_links()
	{
	    for(unsigned int N = 1; N < size_; N++)
	    {
	      unsigned int Pos = N;
	      while(Pos != mark_link[Pos] && Pos != 0){Pos = mark_link[Pos]; }
	      mark[N] = mark[Pos];
	    }
	}
	inline unsigned int size()
	{return size_; }
	inline unsigned int& operator[](unsigned int i)
	{
	  return mark[i];
	}
      private :
	inline unsigned int find_parent_(const Label A)
	{
	  unsigned int Pos = A;
	  while(Pos != mark_link[Pos] && Pos != 0){Pos = mark_link[Pos];}
	  return Pos;
	}
      	mln::util::array<unsigned int> mark;	
	mln::util::array<unsigned int> mark_link;
	unsigned int size_;
    };
  }
}
#endif