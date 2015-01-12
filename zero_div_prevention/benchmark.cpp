#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <algorithm>
#include <cstring>
#include <array>

struct Point
{
  double x;
  double y;
};

inline Point DeltaFromDesiredA (Point const volatile &a, Point const volatile &b, Point const volatile &des)
{
  Point ret;
  
  double const volatile ax = a.x;
  double const volatile bx = b.x;
  double const volatile ay = a.y;
  double const volatile by = b.y;
  
  double const volatile desx = des.x;
  double const volatile desy = des.y;
  
  double const volatile uax = std::min (ax, bx);
  double const volatile ubx = std::max (ax, bx);
  double const volatile uay = std::min (ay, by);
  double const volatile uby = std::max (ay, by);
  
  double const volatile udesx = desx;
  double const volatile udesy = desy;
  
  ret.x = 0.5 * ((ubx - uax + udesx));
  ret.y = 0.5 * ((uby - uay + udesy));
  
  return ret;
}

inline Point DeltaFromDesiredB (Point const volatile &a, Point const volatile &b, Point const volatile &des)
{
  Point ret;

  double const volatile ax = a.x;
  double const volatile bx = b.x;
  double const volatile ay = a.y;
  double const volatile by = b.y;

  double const volatile desx = des.x;
  double const volatile desy = des.y;

  double const volatile uax = ax;
  double const volatile ubx = bx;
  double const volatile uay = ay;
  double const volatile uby = by;

  double const volatile udesx = desx;
  double const volatile udesy = desy;

  ret.x = 0.5 * ((ubx - uax + udesx));
  ret.y = 0.5 * ((uby - uay + udesy));

  return ret;
}

inline Point DeltaFromDesiredC (Point const volatile &a, Point const volatile &b, Point const volatile &des)
{
  Point ret;
  
  double const volatile ax = a.x;
  double const volatile bx = b.x;
  double const volatile ay = a.y;
  double const volatile by = b.y;
  
  double const volatile desx = des.x;
  double const volatile desy = des.y;
  
  double const volatile uax = fmin (ax, bx);
  double const volatile ubx = fmax (ax, bx);
  double const volatile uay = fmin (ay, by);
  double const volatile uby = fmax (ay, by);
  
  double const volatile udesx = desx;
  double const volatile udesy = desy;
  
  ret.x = 0.5 * ((ubx - uax + udesx));
  ret.y = 0.5 * ((uby - uay + udesy));
  
  return ret;
}

size_t time_delta (struct timeval const &tv1, struct timeval const &tv2)
{
    return (tv2.tv_sec * 1000000 + tv2.tv_usec) - (tv1.tv_sec * 1000000 + tv1.tv_usec);
}

inline double prevent_div_zero_nobranch (double *array, size_t size)
{
    static size_t counter = 0;
    counter = (counter + 1) % size;

    double volatile zero_copy = array[counter];
    zero_copy += (zero_copy == 0.0);
}

inline double prevent_div_zero_branch (double *array, size_t size)
{
    static size_t counter = 0;
    counter = (counter + 1) % size;

    double volatile zero_copy = array[counter];
    if (zero_copy == 0.0)
        zero_copy = 1.0;
}

inline int prevent_div_zero_nobranch (int *array, size_t size)
{
    static size_t counter = 0;
    counter = (counter + 1) % size;

    int volatile zero_copy = array[counter];
    zero_copy += (zero_copy == 0);

    return zero_copy;
}

inline double prevent_div_zero_branch (int *array, size_t size)
{
    static size_t counter = 0;
    counter = (counter + 1) % size;

    int volatile zero_copy = array[counter];
    if (zero_copy == 0)
        zero_copy = 1;

    return zero_copy;
}

int main (int argc, char **argv)
{
  Point x = { 50, 50 };
  Point y = { 100, 50 };
  Point delta = { 50, 0 };

  size_t iterations = atoi (argv[1]);
  struct timeval initial;
  struct timeval end;

/*  
  printf ("%i iterations of A\n", iterations);
  struct timeval initial;
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    volatile Point froma = DeltaFromDesiredA (x, y, delta);
  }

  struct timeval end;
  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  printf ("%i iterations of B\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    volatile Point froma = DeltaFromDesiredB (x, y, delta);
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  printf ("%i iterations of C\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    volatile Point fromc = DeltaFromDesiredC (x, y, delta);
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));
*/

  double array[50];
  memset ((void *) array, 1, sizeof (double) * 50);
  array[(int) (rand () * 50) % 49] = 0;

  printf ("%i iterations of nobreanch\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    volatile double one = prevent_div_zero_nobranch (array, 50);
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  printf ("%i iterations of branch\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    volatile double one = prevent_div_zero_branch (array, 50);
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  int iarray[50];
  memset ((void *) iarray, 1, sizeof (int) * 50);
  iarray[(int) (rand () * 50) % 49] = 0;

  printf ("%i iterations of int nobreanch\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    volatile int one = prevent_div_zero_nobranch (iarray, 50);
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  printf ("%i iterations of int branch\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    volatile int one = prevent_div_zero_branch (iarray, 50);
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  printf ("%i iterations of fmin\n", iterations);
  gettimeofday (&initial, NULL);

  int fmin_counter = 0;

  for (size_t i = 0; i < iterations; ++i)
  {
    fmin_counter = (fmin_counter + 1) % 49;
    volatile double min = fmin (array[fmin_counter], array[fmin_counter - 1]);
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  printf ("%i iterations of min\n", iterations);
  gettimeofday (&initial, NULL);

  int min_counter = 0; 

  for (size_t i = 0; i < iterations; ++i)
  {
    fmin_counter = (fmin_counter + 1) % 49;
    volatile double min = std::min (array[min_counter], array[fmin_counter - 1]);
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));


  std::array <double, 50> numbers;
  std::generate (numbers.begin (), numbers.end (), [] {
    return std::rand () * 50;
  });

  printf ("%i iterations of nocheck deltas\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    size_t count_1 = i % 49;
    size_t count_2 = (i + 1) % 49;
    size_t count_3 = (i + 2) % 49;
    volatile double delta = numbers[count_1] - numbers[count_2] - numbers[count_3]; 
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  printf ("%i iterations of abs deltas\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    size_t count_1 = i % 49;
    size_t count_2 = (i + 1) % 49;
    size_t count_3 = (i + 2) % 49;
    volatile double delta = fabs (numbers[count_1] - numbers[count_2]) - numbers[count_3]; 
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  printf ("%i iterations of fmin deltas\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    size_t count_1 = i % 49;
    size_t count_2 = (i + 1) % 49;
    size_t count_3 = (i + 2) % 49;

    double first = fmin (numbers[count_1], numbers[count_2]);
    double second = fmax (numbers[count_2], numbers[count_1]);
    volatile double delta = (second - first) - numbers[count_3]; 
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));

  printf ("%i iterations of branch deltas\n", iterations);
  gettimeofday (&initial, NULL);

  for (size_t i = 0; i < iterations; ++i)
  {
    size_t count_1 = i % 49;
    size_t count_2 = (i + 1) % 49;
    size_t count_3 = (i + 2) % 49;

    double first = std::min (numbers[count_1], numbers[count_2]);
    double second = std::max (numbers[count_2], numbers[count_1]);
    volatile double delta = (second - first) - numbers[count_3]; 
  }

  gettimeofday (&end, NULL);
  printf ("took %lu msec\n", time_delta (initial, end));
}