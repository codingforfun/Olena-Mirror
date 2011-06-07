#ifndef INC_DEBUG_PICT
#define INC_DEBUG_PICT
#include <mln/util/graph.hh>

#include <mln/debug/superpose.hh>
#include <my/util/union.hh>
using namespace mln;
using namespace std;
namespace mymln
{
  namespace debug
  {
      template<typename I> inline void save_label_image(I ima, std::string file)
      {
	image2d<value::rgb8> ima_color = labeling::colorize(value::rgb8(), ima);
	io::ppm::save(ima_color, file);
      }
           template<typename I1, typename I2> inline void save_label_image(image2d<I1> ima, mln::util::array<I2> trans, std::string file)
      {	
	mln_piter(image2d<I1>) p(ima.domain());
	p.start();
	while(p.is_valid())
	{
	     ima(p) = trans(ima(p));
	     p.next();
	}
	image2d<value::rgb8> ima_color = labeling::colorize(value::rgb8(), ima);
	io::ppm::save(ima_color, file);
      }
      
      template<typename I1, typename I2> inline void save_label_image(image2d<I1> ima, mymln::util::union_find<I2> trans, std::string file)
      {	
	mln_piter(image2d<I1>) p(ima.domain());
	p.start();
	while(p.is_valid())
	{
	     ima(p) = trans[ima(p)];
	     p.next();
	}
	image2d<value::rgb8> ima_color = labeling::colorize(value::rgb8(), ima);
	io::ppm::save(ima_color, file);
      }
      
      
      template<typename p_v> inline void save_graph_image(p_v& pv, unsigned int SizeX, unsigned int SizeY, std::string file)
      {
	image2d<value::rgb8> ima_graph(SizeY, SizeX);
	data::fill(ima_graph, literal::black);
	mln::debug::draw_graph(ima_graph, pv, mln::literal::magenta, mln::literal::magenta);
	io::ppm::save(ima_graph, file);
      }
      template<typename p_v,typename I> inline void save_graph_image(p_v& pv, I source, std::string file)
      {
	image2d<value::rgb8> ima_graph;
	mln::initialize(ima_graph, source);
	data::fill(ima_graph, literal::black);
	mln::debug::draw_graph(ima_graph, pv, mln::literal::magenta, mln::literal::magenta);
	 
	io::ppm::save(mln::debug::superpose(ima_graph,source, literal::white) , file);
      }
      typedef  fun::i2v::array<point2d> graph_fun_points2d;
      template<typename point> inline void save_graph_image(mln::util::graph& graph, fun::i2v::array<point>&  points, unsigned int SizeX, unsigned int SizeY, std::string file)
      {
	 typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	 g_vertices_p pv(graph, points);
	image2d<value::rgb8> ima_graph(SizeY, SizeX);
	data::fill(ima_graph, literal::black);
	mln::debug::draw_graph(ima_graph, pv, mln::literal::magenta, mln::literal::magenta);
	io::ppm::save(ima_graph, file);
      }
      template<typename I> inline void save_boxes_image(mln::util::array<box2d>  boxes, I source, std::string file)
      {
	image2d<bool> out;
	mln::initialize(out, source);
	  for(unsigned int N = 0 ; N < boxes.size(); N++)
	  {
	    if(!boxes[N].is_valid()){continue;}
	    if((boxes[N]).pmin()[0] < 0 || (boxes[N]).pmin()[1] < 0 || (boxes[N]).pmax()[0] < 0 || (boxes[N]).pmax()[1] < 0 ){continue;}
	     if((boxes[N]).pmax()[0] > source.domain().pmax()[0] || (boxes[N]).pmax()[1] > source.domain().pmax()[1] ){continue;}
	     if((boxes[N]).pmin()[0] > source.domain().pmax()[0] || (boxes[N]).pmin()[1] > source.domain().pmax()[1] ){continue;}
	    data::fill((out | (boxes[N])).rw(), true);
	  }
	io::pbm::save(out , file);
      }
      template<typename I> inline void save_graph_image(vertex_image<point2d,bool>  mask, I source, std::string file)
      {
	typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	typedef vertex_image<point2d,bool> v_ima_g;
	typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	
	image2d<value::rgb8> out;
	mln::initialize(out, source);
	
	mln_piter_(v_ima_g) v(mask.domain());
	
	nbh_t nbh(mask);
	mln_niter_(nbh_t) q(nbh, v);
	
	unsigned int fnds = 0;
	for_all(v)
	{
	  unsigned int nds = 0;
	  for_all(q)      {	

	    draw::line(out, q,v, mln::literal::magenta);
	  }
	} 
	io::ppm::save(mln::debug::superpose(out,source, literal::white) , file);
      }
      
      template<typename I> inline void save_graph_image(vertex_image<point2d,bool>  graphsource, vertex_image<point2d,bool>  mask, I source, std::string file)
      {
	typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	typedef vertex_image<point2d,bool> v_ima_g;
	typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	
	image2d<value::rgb8> out;
	mln::initialize(out, source);
	
	mln_piter_(v_ima_g) v(graphsource.domain());
	
	nbh_t nbh(mask);
	mln_niter_(nbh_t) q(nbh, v);
	
	unsigned int fnds = 0;
	for_all(v)
	{
	  unsigned int nds = 0;
	  for_all(q)      {	

	    draw::line(out, q,v, mln::literal::magenta);
	  }
	} 
	io::ppm::save(mln::debug::superpose(out,source, literal::white) , file);
      }
      
  }
}
#endif