#ifndef INC_DOCUMENT_FILTER_GENERIC
#define INC_DOCUMENT_FILTER_GENERIC
namespace mymln
{
  namespace document
  {
    namespace filter
    {
      template<typename L, typename F, typename D, typename Left, typename Right>
      class filter
      {
	public:
	  filter(){}
	  filter(document<L,F,D>& doc){ doc_ = doc; }
	  filter(document<L,F,D>& doc, vertex_image<point2d,bool> mask){ doc_ = doc; mask_ = mask; }
	  inline bool link_test(point2d& A, point2d& B){ return true; }
	  inline bool vertex_test(point2d& A){ return true; }
	  inline bool gen_link_test(point2d& A, point2d& B)
	  {
	    return link_test(A, B);
	  }
	  inline bool gen_vertex_test(point2d& A)
	  {
	    return vertex_test(A);
	  }
	  inline void iter_dgb(std::string dgb_out, image2d<bool> s)
	  {
	    image2d<value::rgb8> out;
	    mln::initialize(out, s);
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    mln_piter_(v_ima_g) v(mask_.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask_);
	    mln_niter_(nbh_t) q(nbh, v);
	    for_all(v)
	    {
	      if(gen_vertex_test(v))
	      {
		for_all(q)
		{
		  if(gen_link_test(v, q))
		  {
		   draw::line(out, q,v, mln::literal::green);
		  }
		  else
		  {
		    draw::line(out, q,v, mln::literal::magenta);
		  }
		}
	      }
	      else
	      {
		draw::line(out, q,v, mln::literal::magenta);
	      }
	    }
	  }
	  inline void iter()
	  {
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    mln_piter_(v_ima_g) v(mask_.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask_);
	    mln_niter_(nbh_t) q(nbh, v);
	    for_all(v)
	    {
	      if(gen_vertex_test(v))
	      {
		for_all(q)
		{
		  if(gen_link_test(v, q))
		  {
		   
		  }
		}
	      }
	    }
	  }
	  
	  inline filter& operator|(filter& B)
	  {
	      filter<L,F,D> PFilter = filter_or(doc_, mask_);
	      PFilter.sub_filter_A_ = this;
	      PFilter.sub_filter_B_ = B;
	      B.doc_ = doc_;
	      B.mask_ = mask_;
	      return PFilter;
	  }
	  
	  inline filter& operator&(filter& B)
	  {
	      filter<L,F,D> PFilter = filter_and(doc_, mask_);
	      PFilter.sub_filter_A_ = this;
	      PFilter.sub_filter_B_ = B;
	      B.doc_ = doc_;
	      B.mask_ = mask_;
	      return PFilter;
	  }
	  
	protected:
	  Left sub_filter_A_;
	  Right sub_filter_B_;
	  
	  document<L,F,D> doc_;
	  vertex_image<point2d,bool> mask_;
	  

	  
      };
      
      
      
      
      
      
      
      
      
      
      template<typename L, typename F, typename D>
      class filter_or : filter<L,F,D>
      {
	public:
	  inline bool gen_link_test(point2d& A, point2d& B)
	  {
	    return sub_filter_A_.gen_link_test(A, B) || sub_filter_B_.gen_link_test(A, B);
	  }
	  inline bool gen_vertex_test(point2d& A)
	  {
	    return sub_filter_A_.gen_vertex_test(A) || sub_filter_B_.gen_vertex_test(A);
	  }
	  
	  protected:
	  filter<L,F,D> sub_filter_A_;
	  filter<L,F,D> sub_filter_B_;
	  
	  document<L,F,D> doc_;
	  vertex_image<point2d,bool> mask_;
      };
      
     template<typename L, typename F, typename D>
      class filter_and : filter<L,F,D>
      {
	public:
	  inline bool gen_link_test(point2d& A, point2d& B)
	  {
	    return sub_filter_A_.gen_link_test(A, B) || sub_filter_B_.gen_link_test(A, B);
	  }
	  inline bool gen_vertex_test(point2d& A)
	  {
	    return sub_filter_A_.gen_vertex_test(A) || sub_filter_B_.gen_vertex_test(A);
	  }
	  
	  protected:
	  filter<L,F,D> sub_filter_A_;
	  filter<L,F,D> sub_filter_B_;
	  
	  document<L,F,D> doc_;
	  vertex_image<point2d,bool> mask_;
      };
      
      template<typename L, typename F, typename D>
      class filter_letter : filter<L,F,D>
      {
	public:
	 inline bool vertex_test(point2d& A){ return doc_.contain_letter(A); }
	 
	 protected:
	  filter<L,F,D> sub_filter_A_;
	  filter<L,F,D> sub_filter_B_;
	  
	  document<L,F,D> doc_;
	  vertex_image<point2d,bool> mask_;
      };
      
      
      
    }
  }
}
#endif