#include "ray.h"

void assert_f(float actual, float expected, char *msg) {
  if (fabsf(expected - actual) > EPS) { printf("FAILED %s: %0.2f got %0.2f\n", msg, expected, actual); }
}
void assert_v4(Vec4 actual, Vec4 expected, char *msg) {
  if (fabsf(expected.x - actual.x) > EPS ||
      fabsf(expected.y - actual.y) > EPS ||
      fabsf(expected.z - actual.z) > EPS ||
      fabsf(expected.w - actual.w) > EPS)   {
    printf("FAILED %s: ", msg);
    print_v4(expected);
    printf(" got ");
    print_v4(actual);
    printf("\n");
  }
}

int main(int argc, char const *argv[])
{
  assert_v4(vadd(vec4(3, -2, 5, 1), vec4(-2, 3, 1, 0)), vec4(1, 1, 6, 1), "adding two vec4");

  assert_v4(vsub(vec4(3, 2, 1, 0), vec4(5, 6, 7, 0)), vec4(-2, -4, -6, 0), "subtracting two vec4");
  assert_v4(vsub(vec4(0, 0, 0, 0), vec4(1, -2, 3, 0)), vec4(-1, 2, -3, 0), "subtracting by zero vec4");
  assert_v4(vneg(vec4(1, -2, 3, 4)), vec4(-1, 2, -3, -4), "negating vec4");

  assert_v4(vmul(vec4(1, -2, 3, -4), 3.5), vec4(3.5, -7, 10.5, -14), "scalar multiplication vec4");
  assert_v4(vmul(vec4(1, -2, 3, -4), 0.5), vec4(0.5, -1, 1.5, -2), "scalar multiplication vec4");

  assert_v4(vdiv(vec4(1, -2, 3, -4), 2), vec4(0.5, -1, 1.5, -2), "scalar division vec4");

  assert_f(vmag(vec4(1, 0, 0, 0)), 1, "magnitude vec4");
  assert_f(vmag(vec4(0, 1, 0, 0)), 1, "magnitude vec4");
  assert_f(vmag(vec4(0, 0, 1, 0)), 1, "magnitude vec4");
  assert_f(vmag(vec4(1, 2, 3, 0)), sqrtf(14), "magnitude vec4");
  assert_f(vmag(vec4(-1, -2, -3, 0)), sqrtf(14), "magnitude vec4");

  assert_v4(vnrm(vec4(4, 0, 0, 0)), vec4(1, 0, 0, 0), "normalize vec4");
  assert_v4(vnrm(vec4(1, 2, 3, 0)), vec4(0.26726, 0.53452, 0.80178, 0), "normalize vec4");
  assert_f(vmag(vnrm(vec4(1, 2, 3, 0))), 1, "normalize vec4 magnitude");

  assert_f(vdot(vec4(1, 2, 3, 0), vec4(2, 3, 4, 0)), 20, "dot product vec4");

  assert_v4(vcross(vec4(1, 2, 3, 0), vec4(2, 3, 4, 0)), vec4(-1, 2, -1, 0), "cross product vec4");
  assert_v4(vcross(vec4(2, 3, 4, 0), vec4(1, 2, 3, 0)), vec4(1, -2, 1, 0), "cross product vec4");

  printf("Tests ran.\n");
  return 0;
}