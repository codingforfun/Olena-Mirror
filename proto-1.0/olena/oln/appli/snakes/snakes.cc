#include <oln/basics2d.hh>
#include <ntg/all.hh>
#include <oln/morpho/watersnakes.hh>
#include <oln/level/fill.hh>
#include <oln/appli/snakes/snakes.hh>

#define my_abs(u) ((u) > 0 ? (u) : (-(u)))

using ntg::int_u8;

template <typename T>
void aff_points(const oln::image2d<T>& input,
		const std::vector<oln::point2d>& v,
		oln::image2d<T>& out)
{
  oln_iter_type(oln::image2d<T>) p(input);
  oln::image2d<bool> mask(input.size());

  for_all(p)
    mask[p] = false;

  std::vector<oln::point2d>::const_iterator it = v.begin();
  for (; it != v.end(); it++)
    mask[*it] = true;

  for_all(p)
    {
      if (mask[p] == true)
	out[p] = 255;
      else
	out[p] = input[p];
    }
}

template <typename T>
void aff_droites(const oln::image2d<T>& input,
		 const std::vector<oln::point2d>& v,
		 oln::image2d<T>& out)
{
  oln_iter_type(oln::image2d<T>) p(input);
  oln::image2d<bool> mask(input.size());

  for_all(p)
    mask[p] = false;

  std::vector<oln::point2d>::const_iterator it = v.begin();
  for (; it + 1!= v.end(); it++)
    {
      std::vector<oln::point2d> vec = oln::snakes::impl::draw_line_(*it, *(it+1));
      std::vector<oln::point2d>::const_iterator it2 = vec.begin();

      for (; it2 != vec.end(); it2++)
	mask[*it2] = true;
    }

  for_all(p)
    {
      if (mask[p] == true)
	out[p] = 255;
      else
	out[p] = input[p];
    }
}



bool nearly_equal(int x1, int x2, int y1, int y2)
{
  return (my_abs(x1 - x2) < 5 and my_abs(y1 - y2) < 5);
}

template <typename T>
std::vector<oln::point2d> tri_vec(const oln::image2d<T>& ima,
				  const std::vector<oln::point2d>& v)
{
  std::vector<oln::point2d> res;
  std::vector<oln::point2d>::const_iterator it;
  std::vector<oln::point2d>::iterator it2;

  int ex_1_x = -100;
  int ex_1_y = -100;
  int ex_2_x = 0;
  int ex_2_y = 0;
  int save_x = -1;
  int save_y = -1;


  for (int i = 0; i < 2; i++)
    {
      double min = 100000000;

      for (it = v.begin(); it != v.end(); it++)
	{
	  if (i and nearly_equal((*it).row(), ex_1_x, (*it).col(), ex_1_y))
	    continue;

	  double tmp1 = oln::snakes::impl::euclidian_distance((*it).row(), 0, 0, 0);
	  double tmp2 = oln::snakes::impl::euclidian_distance((*it).row(), ima.nrows(), 0, 0);
	  double tmp3 = oln::snakes::impl::euclidian_distance(0, 0, (*it).col(), 0);
	  double tmp4 = oln::snakes::impl::euclidian_distance(0, 0, (*it).col(), ima.ncols());

	  double tmp_min;

	  if (tmp1 <= tmp2 and tmp1 <= tmp3 and tmp1 <= tmp4)
	    tmp_min = tmp1;
	  if (tmp2 <= tmp1 and tmp2 <= tmp3 and tmp2 <= tmp4)
	    tmp_min = tmp2;
	  if (tmp3 <= tmp1 and tmp3 <= tmp2 and tmp3 <= tmp4)
	    tmp_min = tmp3;
	  if (tmp4 <= tmp1 and tmp4 <= tmp2 and tmp4 <= tmp3)
	    tmp_min = tmp4;

	  if (tmp_min < min)
	    {
	      min = tmp_min;
	      save_x = (*it).row();
	      save_y = (*it).col();
	    }
	}
      if (not i)
	{
	  ex_1_x = save_x;
	  ex_1_y = save_y;
	}
      else
	{
	  ex_2_x = save_x;
	  ex_2_y = save_y;
	}
    }

  res.push_back(oln::point2d(ex_1_x, ex_1_y));

  std::vector<oln::point2d> tmp;
  for (it = v.begin(); it != v.end(); it++)
    if (((*it).row() != ex_1_x or (*it).col() != ex_1_y) and
	((*it).row() != ex_2_x or (*it).col() != ex_2_y))
      tmp.push_back(*it);


  bool stop = false;

  int tmp_x = ex_1_x;
  int tmp_y = ex_1_y;

  while (stop == false)
    {
      if (tmp.size() > 1)
	{
	  double min = 10000000;
	  oln::point2d p;
	  std::vector<oln::point2d>::iterator p_ref;
	  for (it2 = tmp.begin(); it2 != tmp.end(); it2++)
	    {
 	      double tmp_d = oln::snakes::impl::euclidian_distance((*it2).row(), tmp_x, (*it2).col(), tmp_y);
	      if (tmp_d < min)
		{
		  min = tmp_d;
		  p = *it2;
		  p_ref = it2;
		}
	    }
	  res.push_back(p);
	  tmp.erase(p_ref);
	  tmp_x = p.row();
	  tmp_y = p.col();
	}
      else
	{
	  res.push_back(oln::point2d(tmp.front().row(), tmp.front().col()));
	  stop = true;
	}
    }

  res.push_back(oln::point2d(ex_2_x, ex_2_y));
  return res;
}


int main(int argc, char **argv)
{
  using namespace oln;

  if (argc < 10)
    {
      std::cout << "Usage : ./cmd force_type(0: grad, 1: variance) fen nb_gen lamda dmax snake_type(0:ferme, 1:ouvert) max_pts image_bool images(1+) " << std::endl;
      exit(1);
    }

  std::vector<oln::point2d> v;
  unsigned max_pts = (unsigned)atoi(argv[7]);

  oln::image2d<ntg::bin> mask = load(argv[8]);
  oln::image2d<int_u8> input1 = load(argv[9]);

  v = morpho::watersnakes(input1, mask, win_c8p(), 5);

  v = tri_vec(input1, v);

  if (max_pts)
    v = oln::snakes::impl::redispatch(v, max_pts);

  for (int i = 9; i < argc; i++)
    {
      std::cout << i - 8 << " / " << argc - 9 << std::endl;

      oln::image2d<int_u8> input;
      input = load(argv[i]);
      oln::image2d<int_u8> out(input.size());
      unsigned nb_gen = 0;

      if (i == 9)
	nb_gen = (unsigned)atoi(argv[3]);
      else
	nb_gen = 1;

      v = oln::snakes::snakes(input, v, (force_type)(atoi(argv[1])), (unsigned)atoi(argv[2]), nb_gen, max_pts, atof(argv[4]), (unsigned)atoi(argv[5]), (snake_type)atoi(argv[6]));

      aff_droites(input, v, out);
      //      aff_points(input, v, out);

      char t[30];
      sprintf(t, "snakes_%d.pgm", i - 8);

      save(out, t);
    }
}
