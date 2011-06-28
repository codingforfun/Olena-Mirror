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
	{bbox_ = mln::box2d(); ID_ = 0;}
	line(box2d bounding_box, int ID)
	{bbox_ = bounding_box; ID_ = ID;}
	mln::box2d bbox()
	{return bbox_;}
      private:
	int ID_;
	mln::box2d bbox_;
    };
  }
}
#endif