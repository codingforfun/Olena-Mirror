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
    if (mask[p] == true)
      out[p] = 255;
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
    if (mask[p] == true)
      out[p] = 255;
}


template <typename T>
void my_copy(const oln::image2d<T>& input,
	     oln::image2d<T>& out)
{
  oln_iter_type(oln::image2d<T>) p(input);
  for_all(p)
    out[p] = input[p];
}

template <typename T>
bool is_element_vec(const std::vector<T>& v, const T& elt)
{
  typename std::vector<T>::const_iterator it = v.begin();
  for (; it != v.end(); it++)
    if (*it == elt)
      return true;
  return false;
}

bool nearly_equal(int x1, int x2, int y1, int y2)
{
  return (my_abs(x1 - x2) < 5 and my_abs(y1 - y2) < 5);
}

template <typename T>
int chemin(const std::vector<oln::point2d>& v,
	   const oln::image2d<T>& ima,
	   std::vector<oln::point2d>& seen,
	   //	   const oln::point2d& pred,
	   const oln::point2d& p,
	   oln::point2d& f,
	   oln::point2d& tmp,
	   int& max,
	   bool& stop)
{
  oln_neighb_type(oln::window2d) q(oln::win_c8p(), p);
  bool end = true;
  seen.push_back(p);
  for_all (q)
    {
      if (is_element_vec(seen, (oln::point2d)(q)) == 0 and
	  is_element_vec(v, (oln::point2d)(q)))
	    {
	      int res = 1 + chemin(v, ima, seen, q, f, tmp, max, stop);
	      if (res > max)
		{
		  max = res;
		  f = tmp;
		}
	      end = false;
	    }
    }
  if (end)
    {
      tmp = p;
      return 1;
    }
  return 0;
}


std::vector<oln::point2d> verify_snaxels(const std::vector<oln::point2d>& v_init,
					 const std::vector<oln::point2d>& v)
{
  std::vector<oln::point2d> res;

  if (v_init.size() - v.size() > 5)
    {
      std::vector<oln::point2d>::const_iterator it;
      for (it = v_init.begin(); it != v_init.end(); it++)
	if (is_element_vec(v, *it) == 0)
	  res.push_back(*it);
    }
  return res;
}



template <typename T>
std::vector<oln::point2d> tri_vec(const oln::image2d<T>& ima,
				  const std::vector<oln::point2d>& v,
				  snake_type type) // OUVERT || FERME
{
  std::vector<oln::point2d> res;
  std::vector<oln::point2d>::const_iterator it;
  std::vector<oln::point2d>::iterator it2;
  oln::point2d dep;
  oln::point2d tmp2;
  oln::point2d arr;

  bool stop = false;
  oln::point2d beg(-1,-1);

  for (it = v.begin(); it != v.end(); it++)
    {
      unsigned nb_nbh = 0;
      oln_neighb_type(oln::window2d) q(oln::win_c8p(), *it);
      for_all (q)
	if ((oln::point2d)(q) != *it and ima.hold(q) and is_element_vec(v, (oln::point2d)(q)))
	  ++nb_nbh;
      if (nb_nbh == 2)
	{
	  beg = *it;
	  break;
	}
    }
  if (beg.row() == -1)
    beg = *(v.begin() + v.size() / 2);

  dep = beg;
  arr = beg;
  std::vector<oln::point2d> seen;

  int i = 0;
  oln_neighb_type(oln::window2d) q(oln::win_c8p(), beg);
  for_all (q)
    {
      if ((oln::point2d)(q) != beg and ima.hold(q) and is_element_vec(v, (oln::point2d)(q)))
	{
	  bool s = false;
	  int max = 0;

	  seen.push_back(beg);
	  if (not i)
	    chemin(v, ima, seen, (oln::point2d)(q), dep, tmp2, max, s);
	  else
	    chemin(v, ima, seen, (oln::point2d)(q), arr, tmp2, max, s);

	  seen.clear();
	  i++;
	}
    }
  res.push_back(dep);

  std::vector<oln::point2d> tmp;
  for (it = v.begin(); it != v.end(); it++)
    if (*it != dep)
      if (type == OUVERT)
	{
	  if (*it != arr)
	    tmp.push_back(*it);
	}
      else
	tmp.push_back(*it);


  stop = false;
  int tmp_x = dep.row();
  int tmp_y = dep.col();

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
 	  if (min <= 10)
	    res.push_back(p);
	  tmp.erase(p_ref);
 	  if (min <= 10)
	    {
	      tmp_x = p.row();
	      tmp_y = p.col();
	    }
	  if (tmp.size() == 1)
	    stop = true;
	}
    }

  if (type == OUVERT or is_element_vec(res, arr) == 0)
    res.push_back(arr);
  return res;
}


std::vector<std::vector<oln::point2d> > filter_size(std::vector<std::vector<oln::point2d> >& v)
{
  std::vector<std::vector<oln::point2d> > res;

  for (int i = 0; i < v.size(); i++)
    if (v[i].size() > 4)
      res.push_back(v[i]);

  return res;
}


void decoup_set(std::vector<std::vector<oln::point2d> >& v)
{
  std::vector<oln::point2d>::iterator it;
  std::vector<oln::point2d>::iterator it2;
  bool stop = false;

  for (int i = 0; stop == false and i < v.size(); i++)
    for (int j = i + 1; stop == false and j < v.size(); j++)
      for (it = v[i].begin(); stop == false and it != v[i].end(); it++)
	for (it2 = v[j].begin(); stop == false and it2 != v[j].end(); it2++)
	  if (nearly_equal((*it).row(), (*it2).row(), (*it).col(), (*it2).col()))
	    {
	      std::vector<oln::point2d> n1;
	      std::vector<oln::point2d> n2;

	      std::vector<oln::point2d>::iterator it3;
	      std::vector<oln::point2d>::iterator it4;

	      if (it + 1 != v[i].end() and it + 2 != v[i].end() and it + 3 != v[i].end() and
		  it != v[i].begin() and it + 1 != v[i].begin() and it + 2 != v[i].begin())
		{
		  for (it3 = it + 1; it3 != v[i].end(); it3++)
		    n1.push_back(*it3);
		  v[i].erase(it + 1, v[i].end() - 1);
		}

	      if (it2 + 1 != v[j].end() and it2 + 2 != v[j].end() and it2 + 3 != v[j].end() and
		  it2 != v[j].begin() and it2 + 1 != v[j].begin() and it2 + 2 != v[j].begin())
		{
		  for (it4 = it2 + 1; it4 != v[j].end(); it4++)
		    n2.push_back(*it4);
		  v[j].erase(it2 + 1, v[j].end() - 1);
		}
	      if (n1.size())
		v.push_back(n1);
	      if (n2.size())
		v.push_back(n2);
	      if (n2.size() or n1.size())
		decoup_set(v);
	      stop = true;
	    }

}



int main(int argc, char **argv)
{
  using namespace oln;

  if (argc < 10)
    {
      std::cout << "Usage : ./cmd force_type(0: grad, 1: variance) fen nb_gen lamda dmax snake_type(0:ferme, 1:ouvert) max_pts image_bool images(1+) " << std::endl;
      exit(1);
    }

  std::vector<std::vector<oln::point2d> > v;
  unsigned max_pts = (unsigned)atoi(argv[7]);

  oln::image2d<ntg::bin> mask = load(argv[8]);
  oln::image2d<int_u8> input1 = load(argv[9]);

  v = morpho::watersnakes(input1, mask, win_c8p(), 5);
  v = filter_size(v);
  //  decoup_set(v);

  for (int cpt = 0; cpt < v.size(); cpt++)
    {
      std::vector<oln::point2d> v_new = tri_vec(input1, v[cpt], (snake_type)atoi(argv[6]));
      std::vector<oln::point2d> v_verif = verify_snaxels(v[cpt], v_new);
      v[cpt] = v_new;
      if (v_verif.size() > 0)
	v.push_back(v_verif);
      if (v[cpt].size() > max_pts)
	v[cpt] = oln::snakes::impl::redispatch(v[cpt], max_pts);
    }

  std::vector<unsigned> v_max_pts;
  for (int cpt = 0; cpt < v.size(); cpt++)
    if ((unsigned)((float)(v[cpt].size()) * 0.6) > max_pts)
      v_max_pts.push_back(max_pts);
    else
      v_max_pts.push_back((unsigned)((float)(v[cpt].size()) * 0.6));

  for (int i = 9; i < argc; i++)
    {
      std::cout << i - 8 << " / " << argc - 9 << std::endl;

      oln::image2d<int_u8> input;
      input = load(argv[i]);
      oln::image2d<int_u8> out(input.size());
      unsigned nb_gen = 0;

      my_copy(input, out);

      nb_gen = (unsigned)atoi(argv[3]);

      for (int cpt = 0; cpt < v.size(); cpt++)
	{
	  if (v[cpt].size() > v_max_pts[cpt])
	    v[cpt] = oln::snakes::impl::redispatch(v[cpt], v_max_pts[cpt]);
	  else
	    v[cpt] = oln::snakes::impl::redispatch_normal(v[cpt], v[cpt].size());
	  v[cpt] = oln::snakes::snakes(input, v[cpt], (force_type)(atoi(argv[1])), (unsigned)atoi(argv[2]), nb_gen, v_max_pts[cpt], atof(argv[4]), (unsigned)atoi(argv[5]), (snake_type)atoi(argv[6]));
	  aff_droites(input, v[cpt], out);
	}
      char t[30];
      sprintf(t, "snakes_%d.pgm", i - 8);

      save(out, t);
    }
}
