#ifndef INC_DOCUMENT_SEPARATOR
#define INC_DOCUMENT_SEPARATOR
#include <my/util/vector_bbox_group.hh>
#include <mln/util/graph.hh>
#include <my/document/document.hh>
using namespace mln;
namespace mymln
{
  namespace document
  {
      namespace separators
      {
	//TODO: CHANGER contain_Hseparator en VSeparator Le H c etait pour le test
	template<typename L, typename F, typename D>
	void separators_rebuild(mymln::document::document<L,F,D>& doc)
	{
	   typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = doc.fun_mask_all();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	    mymln::util::union_find<L> sep_union(doc.size());
	    for_all(v)
	    {
	      if(!doc[v]){continue;}
	      if(doc.contain_Vseparator(doc[v]) && !sep_union[doc[v]])
	      {
		sep_union[doc[v]] = sep_union.new_set();
		sep_union.add_self_link(doc[v]);
		/*doc.debug_union(sep_union);
		doc.debug_breakpoint();*/
	      }
		for_all(q)
		{
		  if(doc.contain_Vseparator(doc[q])){continue;}
		  if(doc.contain_Vseparator(doc[v]) &&
		    doc.separator_has(v, q))
		  {
		   
		    doc.add_Vseparator(doc[q]);
		    doc.debug_draw_box_red_buffer(doc[q]);
		    sep_union[doc[q]] =  sep_union[doc[v]];
		    sep_union.add_link(doc[v], doc[q]);

		  }
		  else if(
			
			doc.letter_ratio_XY (q) <= 1 &&
			doc.allign_H_large_one(q, v) &&
			doc.allign_proximity_top_strict(q,v)
			)
		      {
			sep_union.add_link(doc[v], doc[q]);
		      }
		}
	    }
	    doc.debug_union(sep_union);
	    sep_union.propage_links();
	    for(int N = 0; N < doc.size(); N++)
	    {
	      if(sep_union[N])
	      {
		if(!doc.contain_Vseparator(N))
		{
		  doc.add_Vseparator(N);	
		}
	      }
	    }
	    
	    
	}
		template<typename L, typename F, typename D>
	void separators_merge(mymln::document::document<L,F,D>& doc)
	{
	  
	   typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = doc.fun_mask_separators();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	    mymln::util::union_find<L> sep_union(doc.size());
	    for_all(v)
	    {
	      if(doc.contain_separator(v))
	      {
		for_all(q)
		{
		  if(doc.separator_has(v, q))
		  {doc.merge_separators(v,q);}
		}
	      }
	      else
	      {
		for_all(q)
		{
		  if(doc.separator_has(v, q))
		  {
		    doc.add_Vseparator(doc[q]);
		    doc.merge_separators(v,q);
		    break;
		  }
		}
	      }
	    }

	}
	template<typename L, typename F, typename D>
	void separators_find_allign(mymln::document::document<L,F,D>& doc)
	{
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = doc.fun_mask_all_letters();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	    for_all(v)
	    {

	      if(doc.contain_letter(v))
	      {
		doc.jump_to_separator(v);
		if((!doc.contain_implicit_separator(v)))
		{
		  doc.add_to_separator_left(v);
		  doc.add_to_separator_self_link(v);
		}
		bool All_Alone = true;
		for_all(q)
		{
		  
		    if((!doc.contain_implicit_separator(q)))
		    {
		      // draw::line(out, q,v, mln::literal::blue);
		      if(doc.allign_H_min(q,v) && doc.allign_size(q, v))
		      {
			doc.add_to_separator_link(v, q);
			  All_Alone = false;
		      }
		    }
		    else
		    {
		      if(doc.allign_H_min(q,v) && doc.allign_size(q, v))
		      {
			doc.add_to_separator_link(q, v);
			 All_Alone = false;
		      }
		    }
		}
		if(All_Alone){doc.invalidate_implicit_separator(v);}

	      }
	      
	    }
	    doc.propage_separator_link();
	}
	
	
	template<typename L, typename F, typename D>
	void separators_find_allign_right(mymln::document::document<L,F,D>& doc)
	{
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = doc.fun_mask_all_letters();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	    for_all(v)
	    {

	      if(doc.contain_letter(v) || doc.contain_alone_letter(v))
	      {
		doc.jump_to_separator(v);
		if((!doc.contain_implicit_separator(v)))
		{
		  doc.add_to_separator_right(v);
		  doc.add_to_separator_self_link(v);
		}
		bool All_Alone = true;
		for_all(q)
		{
		  
		    if((!doc.contain_implicit_separator(q)))
		    {
		      // draw::line(out, q,v, mln::literal::blue);
		      if(doc.allign_H_max(q,v) && doc.allign_size(q, v))
		      {
			doc.debug_draw_line_green_buffer(v, q);
			doc.add_to_separator_link(v, q);
			  All_Alone = false;
		      }
		      else
		      {doc.debug_draw_line_red_buffer(v, q);}
		    }
		    else
		    {
		      if(doc.allign_H_max(q,v) && doc.allign_size(q, v))
		      {
			doc.debug_draw_line_green_buffer(v, q);
			doc.add_to_separator_link(q, v);
			 All_Alone = false;
		      }
		      else
		      {doc.debug_draw_line_red_buffer(v, q);}
		    }
		}
		if(All_Alone){doc.invalidate_implicit_separator(v);}

	      }
	      
	    }
	    doc.propage_separator_link();
	}
	
	
	template<typename L, typename F, typename D>
	void separators_make_clean(mymln::document::document<L,F,D>& doc)
	{
	  
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = doc.fun_mask_letters();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	     mln::util::array<unsigned> count = mln::util::array<unsigned>(doc.size());
	     count.fill(0);
	    for_all(v)
	    {

	    	if(doc.contain_implicit_separator(v))
		{
		  bool All_Alone = true;
		

		  for_all(q)
		  {
		    
		      if(doc.contain_implicit_separator(q) && doc.same_implicit_separator(q,v) )
		      {
			// draw::line(out, q,v, mln::literal::blue);
			if(doc.allign_V(q,v) && doc.allign_size(q, v) && doc.allign_right(v,q))
			{
			  count[doc[q]]++;
			}
		
		      }
		      else if (doc.contain_implicit_separator(q))
		      {
			if(doc.allign_V(q,v) && doc.allign_size(q, v) && doc.allign_right(v,q) && doc.allign_proximity_strict(v, q))
			{
			  count[doc[q]]++;
			}
		      }
		      
		  }
		}
	    }   
	    for(unsigned int N = 0; N  < doc.size();N++)
	    {
		if(count[N] > 0)
		  doc.invalidate_implicit_separator(N);
	    }
	}
	
	template<typename L, typename F, typename D>
	void separators_make_clean_right(mymln::document::document<L,F,D>& doc)
	{
	  
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = doc.fun_mask_letters();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	     mln::util::array<unsigned> count = mln::util::array<unsigned>(doc.size());
	     count.fill(0);
	    for_all(v)
	    {

	    	if(doc.contain_implicit_separator(v))
		{
		  bool All_Alone = true;
		  for_all(q)
		  {
		    
		      if(doc.contain_implicit_separator(q) && doc.same_implicit_separator(q,v) )
		      {
			// draw::line(out, q,v, mln::literal::blue);
			if(doc.allign_V(q,v) && doc.allign_size(q, v) && doc.allign_right(q,v))
			{
			  count[doc[q]]++;
			  doc.debug_draw_box_red_buffer(q);
			  doc.debug_draw_line_red_buffer(v, q);
			}
		      }
		      else if (doc.contain_implicit_separator(q))
		      {
			if(doc.allign_V(q,v) && doc.allign_size(q, v) && doc.allign_right(q,v) && doc.allign_proximity_strict(v, q))
			{
			  count[doc[q]]++;
			  doc.debug_draw_box_green_buffer(q);
			  doc.debug_draw_line_green_buffer(v, q);
			}
		      }
		      
		  }
		}
	    }   
	    for(unsigned int N = 0; N  < doc.size();N++)
	    {
		if(count[N] > 0)
		  doc.invalidate_implicit_separator(N);
	    }
	}
	
	
	template<typename L, typename F, typename D>
	void separators_final_clean(mymln::document::document<L,F,D>& doc)
	{
	  
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = doc.fun_mask_letters();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	     mln::util::array<unsigned> count = mln::util::array<unsigned>(doc.size());
	     count.fill(0);
	    for_all(v)
	    {

	    	if(doc.contain_implicit_separator(v))
		{
		  bool All_Alone = true;
		  doc.jump_to_line(v);
		  if((!doc.contain_line(v)))
		  {
		    doc.add_to_line(v);
		    doc.add_to_line_self_link(v); 
		  }

		  for_all(q)
		  {
		    
		      if(doc.contain_implicit_separator(q) && doc.same_implicit_separator(q,v))
		      {
			// draw::line(out, q,v, mln::literal::blue);
			if(doc.allign_V(q,v) && doc.allign_size(q, v))
			{
			  count[doc[q]]++;
			}
		      }
		      
		  }
		}
	    }   
	    for(unsigned int N = 0; N  < doc.size();N++)
	    {
		if(count[N] > 0)
		  doc.invalidate_implicit_separator(N);
	    }
	}
	
      }
  }
}
#endif