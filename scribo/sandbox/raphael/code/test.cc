#define NREMOTE
#include <vector>
#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/magick/all.hh>

#include <mln/core/site_set/p_vertices.hh>
#include <mln/core/image/graph_elt_window.hh>
#include <mln/core/image/vertex_image.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>
#include <mln/core/concept/function.hh>
#include <mln/core/neighb.hh>
#include <mln/core/image/graph_elt_neighborhood_if.hh>
#include <mln/core/var.hh>
#include <mln/accu/shape/bbox.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/util/graph.hh>
#include <mln/util/timer.hh>
#include <mln/debug/draw_graph.hh>
#include <mln/debug/println.hh>
#include <mln/transform/influence_zone_geodesic.hh>
#include <mln/make/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/influence_zone_adjacency_graph.hh>
#include <mln/make/w_window2d.hh>
#include <mln/labeling/value_and_compute.hh>
#include <mln/make/image.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u.hh>
#include <mln/labeling/colorize.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/algebra/vec.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>
#include <mln/graph/compute.hh>
#include <my/draw/string.hh>

#include <mln/draw/plot.hh>
#include <my/debug/pict.hh>
#include <my/debug/bbox.hh>
#include <my/util/vector_bbox.hh>
#include <my/util/vector_bbox_group.hh>
#include <my/document/document.hh>
#include <my/document/separator.hh>
#include <my/document/clean.hh>
#include <my/document/recognition.hh>
#include <my/runtime/runtime.hh>
#include <my/runtime/lib.hh>

#include <my/data/page.hh>
#include <my/preprocessing/preprocessing.hh>

#ifndef NREMOTE
#include <my/debug/remote/document_remote.hh>
#include <my/debug/remote/lib.hh>
#endif




#ifndef NDEMO
#include <demo/demo_extract_paragraphs.hh>
#include <demo/demo_label_to_point.hh>
#include <demo/demo_draw_full_graph.hh>
#include <demo/demo_draw_line_graph.hh>
#include <demo/demo_draw_line_length.hh>
#include <demo/demo_draw_tab.hh>
#include <demo/demo_compute_align.hh>
#include <demo/demo_compute_align_page.hh>
#include <demo/demo_draw_implicit_separators.hh>
#endif



using namespace mln;
using namespace std;

void Process(std::string File, std::string Dir, mymln::runtime::runtime< value::int_u<16> ,float,short>& runtime)
{
  // RUNTIME

  runtime.add_variable("FILE", Dir + "/" + File);
  runtime.add_variable("DIR", Dir);
  runtime.add_variable("DEBUG_FILE", Dir + "/debug_" + File); 
  
  
  
   std::cout << "Processing : " << File << endl;
    /* CREATE GRAPH */
      typedef value::int_u<16> uint16;
    image2d<bool> ima;
   mln::util::timer timer;
   timer.start();
   io::pbm::load(ima, Dir + "/" + File);
   mymln::preprocessing::denoise(ima, c8());
   std::cout << "LOADING FILE : " << timer.stop() << endl;
   timer.restart();

   
   
   
   
   
    uint16 areas_detected;
      mln_VAR( couple , mln::labeling::value_and_compute(ima, true, c8(), areas_detected, accu::shape::bbox<point2d>()));
      image2d<uint16> ima_blob = couple.first();
      util::array<box2d> boxes = couple.second().first();

    image2d<uint16> ima_influ = transform::influence_zone_geodesic(ima_blob, c8());
    util::graph grph = make::influence_zone_adjacency_graph(ima_influ, c8(), areas_detected);

    /* COMPUTE GRAPH POINT POSITION */
    typedef p_vertices<util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
    typedef graph_elt_neighborhood<util::graph, g_vertices_p> g_nbh;    
    fun::i2v::array<point2d> graph_points(areas_detected + 1);
    graph_points(0) = point2d(0,0);
    for(unsigned int N = 0; N <  areas_detected; N++)
    {graph_points(N + 1) = boxes[N + 1].pcenter();}
    g_vertices_p area_grph(grph, graph_points);
    
    
    
    
     std::cout << "INITIALIZING : " << timer.stop() << endl;
     timer.restart();
     /* WORK ON GRAPH */
     
    mymln::document::document<uint16,float,short> doc(ima_blob, ima_influ, boxes, area_grph, areas_detected);
    runtime.set_current_document(&doc);
    doc.debug_set_image(ima);
    doc.horizontal_separator_ratio_range(0.0f, 0.2f);
    doc.vertical_separator_ratio_range(6.0f, 1000.0f);
    doc.container_volume_range(40, 100);
    
    for (uint16 N = 1; N <=  areas_detected; N++)
    {

      g_nbh grph_nbh;
      mln_psite_(g_vertices_p) plist(area_grph, N);
      mln_fwd_niter_(g_nbh) fq(grph_nbh, plist);
      unsigned link = 0;
      for_all((fq))
      {
	link++;
      }
      doc.add(N, (uint16)link);
	
    }
    
      runtime.run_debug();
      std::cout << "WORK ON GRAPH : " << timer.stop() << endl;
      //mymln::data::page<uint16,float,short> page(doc);
     // page.export_HTML(Dir + "/debug_" + File + ".html");
 
      
      
      
      
      // THE DEMOS START HERE
#ifndef NDEMO
      demo_extract_paragraphs(doc);
      demo_label_to_point(doc);
      demo_draw_full_graph(doc);
      demo_draw_line_graph(doc);
      demo_draw_line_length(doc);
      demo_draw_tab(doc);
      demo_compute_align(doc);
      demo_compute_align_page(doc);
      demo_draw_implicit_separators(doc);
#endif
}


#ifndef NREMOTE
// THIS IS USED TO CREATE A FILTER WITHOUT RECOMPILING
mymln::document::debug::remote< value::int_u<16> ,float,short> rem;
template<typename L, typename F, typename D>
    void clean_remote(mymln::document::document<L,F,D>& doc)
    {
      rem.filter(doc);
    }
#endif
    
    
int main( int argc, char** argv)
{
   mymln::runtime::runtime< value::int_u<16> ,float,short> run;
   #ifndef NREMOTE
   rem = mymln::document::debug::remote< value::int_u<16> ,float,short>();
   #endif
   mymln::runtime::load_clean(run);
   mymln::runtime::load_debug(run);
   mymln::runtime::load_cooking(run);
   mymln::runtime::load_string(run);
   mymln::runtime::load_system(run);
   mymln::runtime::load_separators(run);
   mymln::runtime::load_compute(run);
   if(argc <= 1){Process("ima.pbm", "", run);}
   else
   {
     bool dir = false;
     bool prog = false;
     bool remote = false;
     std::string Dir = "";
     std::string Prog = "";
     for(int N = 1 ; N < argc; N++)
     {
       if(dir)
       {
	 Dir = argv[N];
	 dir = false;
       }
       else if(prog)
       { 
	 Prog = argv[N];

	  
	 run.load(Prog.c_str());
	
	 prog = false;
       }
       #ifndef NREMOTE
       else if(remote)
       {
	 Prog = argv[N];
	 rem.load(Prog.c_str());
	 remote = false;
	 run.add_function("@remote", clean_remote);
	 mymln::document::debug::load(rem);
       }
       #endif
       else
       {
	if(!strcmp(argv[N], "-D"))
	{ dir = true;}
	else if(!strcmp(argv[N], "-P"))
	{ prog = true; }
	#ifndef NREMOTE
	else if(!strcmp(argv[N], "--Remote"))
	{ remote = true; }
	#endif
	else
	{ Process(argv[N], Dir, run);  }
       }
     }
   }
   
    
}
