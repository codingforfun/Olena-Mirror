#ifndef INC_VECTOR_2_TOOLS
#define INC_VECTOR_2_TOOLS
namespace mymln
{
  namespace util
  {
    template<typename dataType>
    class vec2
    {
      public:
	vec2(dataType X,dataType Y)
	{_x = X; _y = Y;}
	public dataType x(){return x;}
	public dataType y(){return y;}
	bool allign_x(vec2<dataType>& V,dataType T)
	{
	  if(_x < V._x + T && _x < V._y + T)
	  {return }
	}
      private:
	
    }
#endif