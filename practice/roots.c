// exploration in different derivation for solving roots which avoids
// floating point issues when a, c are small leading to two nubers
// close to reach being subtracted
// pg.17 of Fundamentals of Computer Graphics 4ed
#include <math.h>
#include <stdio.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct {
  double roots[2];
  char solutions;
} Roots;

Roots quadratic(double a, double b, double c) {
  Roots r = {0};

  double solutions = b * b - 4 * a * c;
  if (solutions >= 0) {
    r.solutions = MIN(solutions, 2);
    double r0 = (-b + sqrtf(b * b - 4 * a * c)) / (2 * a);
    double r1 = (-b - sqrtf(b * b - 4 * a * c)) / (2 * a);
    // double r0 = (2 * c) / (-b + sqrtf(b * b - 4 * a * c));
    // double r1 = (2 * c) / (-b - sqrtf(b * b - 4 * a * c));
    r.roots[0] = r0; // MIN(r0, r1);
    r.roots[1] = r1; // MAX(r0, r1);
  } else {
    // not real solution
    return r;
  }
  return r;
}

int main() {
  Roots r = quadratic(1, pow(10, 8), 1);
  // Roots r = quadratic(1, 1, 1);
  for(unsigned char i = 0; i < r.solutions; ++i) {
    printf("%.17g\n", r.roots[i]);
  }
  return 0;
}