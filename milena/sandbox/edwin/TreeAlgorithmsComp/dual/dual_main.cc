#include "dual.hh"

int main(int argc, char** argv)
{
  {
    int_u8 f[] = {2, 1, 2};
    int_u8 m[] = {2, 2, 2};
    int n = sizeof(f);
    test(f, m, n);
  }

  {
    int_u8 f[] = {2, 1, 1, 2, 0};
    int_u8 m[] = {2, 3, 3, 2, 3};
    int n = sizeof(f);
    test(f, m, n);
  }

  // With internmediate level creation
  {
    int_u8 f[] = {10, 1, 1, 10, 5, 10, 5};
    int_u8 m[] = {12, 8, 8, 12, 12, 12, 5};
    int n = sizeof(f);
    test(f, m, n);
  }

  // With singleton creation
  {
    int_u8 f[] = {15, 1, 1, 15, 18, 5};
    int_u8 m[] = {12, 8, 8, 12, 12, 5};
    int n = sizeof(f);
    test(f, m, n);
  }
}
