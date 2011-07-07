#ifndef MLN_DATA_LINE
#define MLN_DATA_LINE
namespace mymln
{
  namespace data
  {
    class line
    {
      public:
	line()
	{bbox_ = mln::box2d(); ID_ = 0; value_ = "-*-";}
	line(box2d bounding_box, int ID)
	{bbox_ = bounding_box; ID_ = ID;value_ = "-*-";}
	mln::box2d bbox()
	{return bbox_;}
	std::string get_value()
	{return value_;}
	void set_value(std::string value)
	{value_ = value;}
      private:
	int ID_;
	mln::box2d bbox_;
	std::string value_;
    };
  }
}
#endif