// implementation from The Ray Tracer Challenge:
// A Test-Driven Guide to Your First 3D Renderer by James Buck
#include <stdio.h>
#include <math.h>

#define EPS 0.0001

typedef union {
  struct  {
    float x;
    float y;
    float z;
    float w;
  };
  struct  {
    float r;
    float g;
    float b;
    float a;
  };
} Vec4;
Vec4 vec4(float x, float y, float z, float w) {
  return (Vec4){{ x, y, z, w }};
}
Vec4 vadd(Vec4 a, Vec4 b) {
  return (Vec4){{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }};
}
Vec4 vsub(Vec4 a, Vec4 b) {
  return (Vec4){{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }};
}
Vec4 vneg(Vec4 a) {
  return (Vec4){{ -a.x, -a.y, -a.z, -a.w }};
}
Vec4 vmul(Vec4 a, float s) {
  return (Vec4){{ s * a.x, s * a.y, s * a.z, s * a.w }};
}
Vec4 vdiv(Vec4 a, float s) {
  return (Vec4){{ a.x / s, a.y / s, a.z / s, a.w / s }};
}
float vmag(Vec4 a) {
  return sqrtf(a.x * a.x + a.y * a.y + a.z  + a.w * a.w);
}

void print_v4(Vec4 v) { printf("(%0.2f, %0.2f, %0.2f, %0.2f)", v.x, v.y, v.z, v.w); }
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

void tests() {
  Vec4 a, b;
  float s;

  a = vec4(3, -2, 5, 1);
  b = vec4(-2, 3, 1, 0);
  assert_v4(vadd(a, b), vec4(1, 1, 6, 1), "adding two vec4");

  a = vec4(3, 2, 1, 0);
  b = vec4(5, 6, 7, 0);
  assert_v4(vsub(a, b), vec4(-2, -4, -6, 0), "subtracting two vec4");

  a = vec4(0, 0, 0, 0);
  b = vec4(1, -2, 3, 0);
  assert_v4(vsub(a, b), vec4(-1, 2, -3, 0), "subtracting by zero vec4");
  a = vec4(1, -2, 3, 4);
  assert_v4(vneg(a), vec4(-1, 2, -3, -4), "negating vec4");

  a = vec4(1, -2, 3, -4);
  s = 3.5;
  assert_v4(vmul(a, s), vec4(3.5, -7, 10.5, -14), "scalar multiplication vec4");
  a = vec4(1, -2, 3, -4);
  s = 0.5;
  assert_v4(vmul(a, s), vec4(0.5, -1, 1.5, -2), "scalar multiplication vec4");

  a = vec4(1, -2, 3, -4);
  s = 2;
  assert_v4(vdiv(a, s), vec4(0.5, -1, 1.5, -2), "scalar division vec4");

  a = vec4(1, 0, 0, 0);
  assert_f(vmag(a), 1, "magnitude vec4");
  a = vec4(0, 1, 0, 0);
  assert_f(vmag(a), 1, "magnitude vec4");
  a = vec4(0, 0, 1, 0);
  assert_f(vmag(a), 1, "magnitude vec4");
  a = vec4(1, 2, 3, 0);
  assert_f(vmag(a), sqrtf(14), "magnitude vec4");
  a = vec4(-1, -2, -3, 0);
  assert_f(vmag(a), sqrtf(14), "magnitude vec4");
}

int main(int argc, char const *argv[])
{
  tests();
  printf("Tests ran.\n");

  return 0;
}