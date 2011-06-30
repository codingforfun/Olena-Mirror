#ifndef MLN_DEBUG_DOCUMENT_REMOTE
#define MLN_DEBUG_DOCUMENT_REMOTE
#include <my/document/document.hh>
using namespace mln;
namespace mymln
{
  namespace document
  {
    namespace debug
    {
         template<typename Label, typename Float, typename Data>
      class remote
      {
	  typedef document< Label, Float, Data > doc;
	public:
	  remote()
	  {
	    program_node = mln::util::array<string>(0);
	    program_argument = mln::util::array<string>(0);
	    program_argument2 = mln::util::array<string>(0);
	    program_instruction = mln::util::array<string>(0);
	    doc_b_pp_fun = map<string, bool(*)(const doc&, const point2d&, const point2d&) >();
	    doc_b_p_fun = map<string, bool(*)(const doc&, const point2d&) >();
	  }
	  void load(const char* file)
	  {
	  
	    fstream filestream(file, fstream::in | fstream::out);
	    std::string Buffer = "";
	      bool flag = !getline(filestream, Buffer).eof();
	      int L = 0;
	      bool remain = flag;
	      bool fun_mask_set = false;
	      
		  while(remain)
		  {
		      if(!flag){remain = false;}
		    int N = 0;
		     std::string Node = "";
		    std::string Instr = "";
		      std::string Arg = "";
		      std::string Arg2 = "";
		    while( N < Buffer.length() && (Buffer[N] == ' ' || Buffer[N] == '\t') && Buffer[N] != ';'){N++;} // trim
		      while(N < Buffer.length() && Buffer[N] != ' ' && Buffer[N] != '\t' && Buffer[N] != ';' && Buffer[N] != ':'){Instr += Buffer[N]; N++;}
		    if(Buffer[N] != ':')
		    {std::cout << "#REMOTE DEBUG SCRIPT ERROR : THE NODE HAS NOT BEEN SPECIFIED" << std::endl; return;}
		    while( N < Buffer.length() && (Buffer[N] == ' ' || Buffer[N] == '\t') && Buffer[N] != ';'){N++;} // trim
		    while(N < Buffer.length() && Buffer[N] != ' ' && Buffer[N] != '\t' && Buffer[N] != ';'){Instr += Buffer[N]; N++;}
		    while(N < Buffer.length() && (Buffer[N] == ' ' || Buffer[N] == '\t') && Buffer[N] != ';'){N++;} // trim
		    while(N < Buffer.length() && Buffer[N] != ' ' && Buffer[N] != '\t' && Buffer[N] != ';'){Arg += Buffer[N]; N++;}
		    while(N < Buffer.length() && (Buffer[N] == ' ' || Buffer[N] == '\t') && Buffer[N] != ';'){N++;} // trim
		    while(N < Buffer.length() && Buffer[N] != ' ' && Buffer[N] != '\t' && Buffer[N] != ';'){Arg2 += Buffer[N]; N++;}
		      if(Instr.length() != 1 || (Instr[0] != 'v' &&  Instr[0] != 'q'))
		      {
			if(Instr.length() != 2 || (Instr[0] != 'v' &&  Instr[0] != 'q') || (Instr[1] != 'v' &&  Instr[1] != 'q'))
			{
			  if(fun_mask_set)
			  {
			    std::cout << "#REMOTE DEBUG SCRIPT ERROR : INVALID NODE" << std::endl; return;
			  }
			  else
			  {
			    fun_mask_set = true;
			    fun_mask = Node;
			  }
			}
		      }
		      program_node.append(Node);
		      program_instruction.append(Instr);
		      program_argument.append(Arg);
		      program_argument2.append(Arg2);
		      
		  } 
	    
	    filestream.close();
	  
	  }
	  inline void filter(doc& d)
	  {
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = 
	    (!fun_mask.compare("all_letters"))?d.fun_mask_all_letters()
	    :(!fun_mask.compare("letters"))?d.fun_mask_letters()
	    :(!fun_mask.compare("alone_letters"))?d.fun_mask_letters()
	    :(!fun_mask.compare("start_end_lines"))?d.fun_mask_start_end_lines()
	    :(!fun_mask.compare("start_lines"))?d.fun_mask_start_lines()
	    :(!fun_mask.compare("end_lines"))?d.fun_mask_start_lines()
	    :(!fun_mask.compare("all"))?d.fun_mask_all()
	    :d.fun_mask_all(); // DEFAULT VALUE
	    
	    
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	    for_all(v)
	    {
	      bool _VTRUE = true;
		for(int N = 0; N < program_instruction.size(); N++)
		{
		  if(!program_node[N].compare("v"))
		  { _VTRUE = _VTRUE && doc_b_p_fun[program_instruction[N]](d, q); }
		}
		if(!_VTRUE){continue;}
	      for_all(q)
	      {
		bool _TRUE = true;
		for(int N = 0; N < program_instruction.size(); N++)
		{
		  if(!program_node[N].compare("vq"))
		  { _TRUE = _TRUE && doc_b_pp_fun[program_instruction[N]](d, v, q); }
		  else if(!program_node[N].compare("qv"))
		  { _TRUE = _TRUE && doc_b_pp_fun[program_instruction[N]](d, (point2d)q, v); }
		  else if(!program_node[N].compare("q"))
		  { _TRUE = _TRUE && doc_b_p_fun[program_instruction[N]](d, (point2d)q); }
		}
		
		if(_TRUE)
		{d.debug_draw_line_green_buffer(q,v);}
		else
		{d.debug_draw_line_red_buffer(q,v);}
	      }
	    }
	  }
	inline void add_function_pp(string name, bool(*ptr)(const doc&, const point2d&, const point2d&) )
	{
	  doc_b_pp_fun[name] = ptr;
	}
	inline void add_function_p(string name, bool(*ptr)(const doc&, const point2d&) )
	{
	  doc_b_p_fun[name] = ptr;
	}
	private:
	map<string, bool(*)(const doc&, const point2d&, const point2d&) > doc_b_pp_fun;
	map<string, bool(*)(const doc&, const point2d&) > doc_b_p_fun;
	mln::util::array<std::string> program_node;
	mln::util::array<std::string> program_instruction;
	mln::util::array<std::string> program_argument;
	mln::util::array<std::string> program_argument2;
	std::string fun_mask;
      };
    }
  }
}
#endif