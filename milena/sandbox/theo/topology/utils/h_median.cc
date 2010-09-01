# include "h_median.hh"
# include <mln/value/int_u8.hh>


int main()
{
  using mln::utils::h_median;

  h_median<mln::value::int_u8> m;

  {
    m.clear();

    m.take(1);
    m.take(1);
    m.take(2);
    m.take(3);
    m.take(3);

    std::cout << m.value() << std::endl;
    std::cout << m.sum_minus() << ' ' << m.sum() << ' ' << m.sum_plus() << std::endl;
  }

  {
    m.clear();

    m.take(0);
    m.take(1);
    m.take(1);
    m.take(3);
    m.take(3);
    m.take(5);

    std::cout << m.value() << std::endl;
    std::cout << m.sum_minus() << ' ' << m.sum() << ' ' << m.sum_plus() << std::endl;
  }

  {
    m.clear();

    m.take(5);
    m.take(3);
    m.take(3);
    m.take(1);
    m.take(1);
    m.take(0);

    std::cout << m.value() << std::endl;
    std::cout << m.sum_minus() << ' ' << m.sum() << ' ' << m.sum_plus() << std::endl;
  }

  {
    m.clear();

    m.take(5);
    m.take(3);
    m.take(1);
    m.take(3);
    m.take(1);
    m.take(0);

    std::cout << m.value() << std::endl;
    std::cout << m.sum_minus() << ' ' << m.sum() << ' ' << m.sum_plus() << std::endl;
  }

  {
    m.clear();

    m.take(0);
    m.take(1);
    m.take(3);
    m.take(1);
    m.take(3);
    m.take(5);

    std::cout << m.value() << std::endl;
    std::cout << m.sum_minus() << ' ' << m.sum() << ' ' << m.sum_plus() << std::endl;
  }


}
