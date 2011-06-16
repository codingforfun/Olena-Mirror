#ifndef INC_RUNTIME
#define INC_RUNTIME

using namespace mln;
using namespace std;
namespace mymln
{
  namespace runtime
  {
    template<typename Label, typename Float, typename Data>
    
    class runtime
    {
      typedef document::document<Label, Float, Data> doc;
      typedef void (*fun_doc_ptr)(doc);
      public:
	runtime()
	{
	  program_argument = mln::util::array<string>(0);
	  program_argument2 = mln::util::array<string>(0);
	  program_instruction = mln::util::array<string>(0);
	  doc_fun = map<string, void(*)(doc&) >();
	  doc_arg_fun = map<string, void(*)(doc&, string) >();
	  doc_arg2_fun = map<string, void(*)(runtime<Label,Float, Data>&, string, string) >();
	  doc_local_fun = map<string, int >();
	  call_stack = stack<int>();
	  CP = 0;
	}
	void load(const char* file)
	{
	  
	   fstream filestream(file, fstream::in | fstream::out);
	   std::string Buffer = "";
	     bool flag = !getline(filestream, Buffer).eof();
	     int L = 0;
	     bool remain = flag;
		while(remain)
		{
		    if(!flag){remain = false;}
		  int N = 0;
		  std::string Instr = "";
		    std::string Arg = "";
		     std::string Arg2 = "";
		  while( N < Buffer.length() && (Buffer[N] == ' ' || Buffer[N] == '\t') && Buffer[N] != ';'){N++;} // trim
		  while( N < Buffer.length() && (Buffer[N] == ' ' || Buffer[N] == '\t') && Buffer[N] != ';'){N++;} // trim
		  while(N < Buffer.length() && Buffer[N] != ' ' && Buffer[N] != '\t' && Buffer[N] != ';'){Instr += Buffer[N]; N++;}
		  while(N < Buffer.length() && (Buffer[N] == ' ' || Buffer[N] == '\t') && Buffer[N] != ';'){N++;} // trim
		  while(N < Buffer.length() && Buffer[N] != ' ' && Buffer[N] != '\t' && Buffer[N] != ';'){Arg += Buffer[N]; N++;}
		  while(N < Buffer.length() && (Buffer[N] == ' ' || Buffer[N] == '\t') && Buffer[N] != ';'){N++;} // trim
		  while(N < Buffer.length() && Buffer[N] != ' ' && Buffer[N] != '\t' && Buffer[N] != ';'){Arg2 += Buffer[N]; N++;}
		  if(!Instr.compare("fun"))
		  {
		    doc_local_fun[Arg] = L;
		    program_instruction.append("");
		    program_argument.append("");
		    program_argument2.append("");
		  }
		  else if(Instr.length() > 0 && Instr[0] == '$')
		  {
		    program_instruction.append("system.set");
		    program_argument.append(Instr);
		    program_argument2.append(Arg);
		  }
		  else
		  {
		    program_instruction.append(Instr);
		    program_argument.append(Arg);
		    program_argument2.append(Arg2);
		  }
		  if(flag)
		    flag = !getline(filestream, Buffer).eof();
		  L++;
		} 
	   
	  filestream.close();
	  
	}
	runtime(const char* file)
	{
	 CP = 0;
	 program_argument2 = mln::util::array<string>(0);
	  program_argument = mln::util::array<string>(0);
	  program_instruction = mln::util::array<string>(0);
	  doc_local_fun = map<string, void(*)(doc&, string) >();
	 load(file);
	  doc_fun = map<string, void(*)(doc&) >();
	  doc_arg_fun = map<string, void(*)(doc&, string) >();
	  doc_arg2_fun = map<string, void(*)(runtime<Label,Float, Data>&, string, string) >();
	  doc_local_fun = map<string, int >();
	  call_stack = stack<int>();
	}
	
	void run()
	{
	  for(CP = 0; CP < program_instruction.size(); CP++)
	  {
	    if(!program_instruction[CP].compare("end"))
	      ret();
	    else if(program_argument2[CP].compare(""))
	      call_function(program_instruction[CP], get_variable(program_argument[CP]), get_variable(program_argument2[CP]));
	    else if(program_argument[CP].compare(""))
	      call_function(program_instruction[CP], get_variable(program_argument[CP]));
	    else if(program_instruction[CP].compare(""))
	      call_function(program_instruction[CP]);
	  }
	}
	inline void add_function(string name, void(*ptr)(doc&) )
	{
	  doc_fun[name] = ptr;
	}
	inline void add_function_string(string name, void(*ptr)(doc&, string arg) )
	{
	  doc_arg_fun[name] = ptr;
	}
	inline void add_function_string_string(string name, void(*ptr)(runtime<Label,Float, Data>&, string arg, string arg2) )
	{
	  doc_arg2_fun[name] = ptr;
	}
	void add_variable(string name, string value)
	{
	  vars["$" + name] = value;
	}
	string get_variable(string name)
	{
	  
	  if(name.length() > 0 && name[0] == '$')
	  {
	    if(vars.find(name) == vars.end())
	    {
	      std::cout << "#SCRIPT ERROR : The variable " << name << " doesn't exist" << std::endl;
	      return "";
	    }
	    return vars[name];
	  }
	  else
	    return name;
	}
	void ret()
	{
	  if(call_stack.empty()){ CP = program_instruction.size(); }
	  else{ CP = call_stack.top(); call_stack.pop(); } 
	}
	void call_function(string name)
	{
	  if(doc_local_fun.find(name) != doc_local_fun.end())
	  {
	    call_stack.push(CP);
	    CP = doc_local_fun[name];
	    return;
	  }
	  
	  if(doc_fun.find(name) == doc_fun.end())
	  {
	    std::cout << "#SCRIPT ERROR : The function " << name << " doesn't exist" << std::endl;
	    return;
	  }
	  doc_fun[name](*current);
	}
	void call_function(string name, string arg)
	{
	  if(doc_arg_fun.find(name) == doc_arg_fun.end())
	  {
	    std::cout << "#SCRIPT ERROR : The function " << name << " doesn't exist" << std::endl;
	    return;
	  }
	  doc_arg_fun[name](*current, arg);
	}
	
	void call_function(string name, string arg, string arg2)
	{
	   if(doc_arg2_fun.find(name) == doc_arg2_fun.end())
	  {
	    std::cout << "#SCRIPT ERROR : The function " << name << " doesn't exist" << std::endl;
	    return;
	  }
	  doc_arg2_fun[name](*this, arg, arg2);
	}
	void set_current_document(doc* document)
	{current = document;}
      private:
	int CP;
	map<string, string > vars;
	map<string, void(*)(doc&) > doc_fun;
	map<string, void(*)(doc&, string) > doc_arg_fun;
	map<string, void(*)(runtime<Label,Float, Data>&, string, string) > doc_arg2_fun;
	map<string, int > doc_local_fun;
	mln::util::array<std::string> program_instruction;
	mln::util::array<std::string> program_argument;
	mln::util::array<std::string> program_argument2;
	
	stack<int> call_stack;
	doc* current;
    };
  }
}
#endif