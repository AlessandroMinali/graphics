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

void print_v4(Vec4 v) { printf("(%0.2f, %0.2f, %0.2f, %0.2f)", v.x, v.y, v.z, v.w); }
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
  return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}
Vec4 vnrm(Vec4 a) {
  return vdiv(a, vmag(a));
}
float vdot(Vec4 a, Vec4 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
Vec4 vcross(Vec4 a, Vec4 b) {
  return (Vec4){{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }};
}