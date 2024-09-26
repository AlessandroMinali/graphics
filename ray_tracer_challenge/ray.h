// implementation from The Ray Tracer Challenge:
// A Test-Driven Guide to Your First 3D Renderer by James Buck
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 0.0001
#define PPM_HEADER 127

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
Vec4 vclamp(Vec4 a, uint8_t min, uint8_t max) {
  return (Vec4){{
    max < a.x ? max :
    a.x < min ? min :
    a.x,
    max < a.y ? max :
    a.y < min ? min :
    a.y,
    max < a.z ? max :
    a.z < min ? min :
    a.z }};
}

Vec4 colour_mul(Vec4 a, Vec4 b) { // Hadamard product
  return (Vec4){{ a.r * b.r, a.g * b.g, a.b * b.b }};
}

typedef struct {
  size_t w;
  size_t h;
  size_t sz;
  Vec4 *pixels;
} Canvas;
Canvas canvas_init(int w, int h) {
  return (Canvas){ w, h, w*h, calloc(w * h, sizeof(Vec4)) };
}
void write_pixel(Canvas *canvas, int x, int y, Vec4 v) {
  canvas->pixels[y*canvas->w+x] = v;
}
Vec4 pixel_at(Canvas *canvas, int x, int y) {
  return canvas->pixels[y*canvas->w+x];
}
void canvas_to_ppm(Canvas *canvas) {
  FILE *f = fopen("ray.ppm", "w+");
  fprintf(f, "P3\n%zu %zu\n255\n", canvas->w, canvas->h);
  for(uint8_t j = 0; j < canvas->h; ++j) {
    for(uint8_t i = 0; i < canvas->w; ++i) {
      Vec4 v = vclamp(vmul(pixel_at(canvas, i, j), 255), 0, 255);
      fprintf(f, "%s%d %d %d", (i == 0 ? "" : " "), (int)v.x, (int)v.y, (int)v.z);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}