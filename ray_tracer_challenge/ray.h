// implementation from The Ray Tracer Challenge:
// A Test-Driven Guide to Your First 3D Renderer by James Buck
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

void print_v4(const Vec4 v) { printf("(%0.2f, %0.2f, %0.2f, %0.2f)", v.x, v.y, v.z, v.w); }
Vec4 vec4(const float x, const float y, const float z, const float w) {
  return (Vec4){{ x, y, z, w }};
}
Vec4 vadd(const Vec4 a, const Vec4 b) {
  return (Vec4){{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }};
}
Vec4 vsub(const Vec4 a, const Vec4 b) {
  return (Vec4){{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }};
}
Vec4 vneg(const Vec4 a) {
  return (Vec4){{ -a.x, -a.y, -a.z, -a.w }};
}
Vec4 vmul(const Vec4 a, const float s) {
  return (Vec4){{ s * a.x, s * a.y, s * a.z, s * a.w }};
}
Vec4 vdiv(const Vec4 a, const float s) {
  return (Vec4){{ a.x / s, a.y / s, a.z / s, a.w / s }};
}
float vmag(const Vec4 a) {
  return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}
Vec4 vnrm(const Vec4 a) {
  return vdiv(a, vmag(a));
}
float vdot(const Vec4 a, const Vec4 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
Vec4 vcross(const Vec4 a, const Vec4 b) {
  return (Vec4){{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }};
}
Vec4 vclamp(const Vec4 a, const uint8_t min, const uint8_t max) {
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

Vec4 colour_mul(const Vec4 a, const Vec4 b) { // Hadamard product
  return (Vec4){{ a.r * b.r, a.g * b.g, a.b * b.b }};
}

bool meq(const float a[4][4], const float b[4][4]) {
  for(uint8_t j = 0; j < 4; ++j) {
    for(uint8_t i = 0; i < 4; ++i) {
      if (EPS < fabsf(a[j][i] - b[j][i])) {
        return false;
      }
    }
  }
  return true;
}
void m4mul(const float a[4][4], const float b[4][4], float (*c)[4][4]) {
  for(uint8_t j = 0; j < 4; ++j) {
    for(uint8_t i = 0; i < 4; ++i) {
      (*c)[j][i] = a[j][0] * b[0][i] +
                a[j][1] * b[1][i] +
                a[j][2] * b[2][i] +
                a[j][3] * b[3][i];
    }
  }
}
Vec4 m4vmul(const float a[4][4], const Vec4 v) {
  return (Vec4){{
    a[0][0]*v.x + a[0][1]*v.y + a[0][2]*v.z + a[0][3]*v.w,
    a[1][0]*v.x + a[1][1]*v.y + a[1][2]*v.z + a[1][3]*v.w,
    a[2][0]*v.x + a[2][1]*v.y + a[2][2]*v.z + a[2][3]*v.w,
    a[3][0]*v.x + a[3][1]*v.y + a[3][2]*v.z + a[3][3]*v.w,
  }};
}
void m4t(const float a[4][4], float (*b)[4][4]) {
  for(uint8_t j = 0; j < 4; ++j) {
    for(uint8_t i = 0; i < 4; ++i) {
      (*b)[j][i] = a[i][j];
    }
  }
}
float m2det(const float a[2][2]) {
  return a[0][0]*a[1][1] - a[0][1]*a[1][0];
}
void m3subm(const float a[3][3], const uint8_t row, const uint8_t col, float (*b)[2][2]) {
  uint8_t x = 0;
  uint8_t y = 0;
  for(uint8_t j = 0; j < 3; ++j) {
    if (j == row) continue;
    x = 0;
    for(uint8_t i = 0; i < 3; ++i) {
      if (i == col) continue;
      (*b)[y][x] = a[j][i];
      ++x;
    }
    ++y;
  }
}
void m4subm(const float a[4][4], const uint8_t row, const uint8_t col, float (*b)[3][3]) {
  uint8_t x = 0;
  uint8_t y = 0;
  for(uint8_t j = 0; j < 4; ++j) {
    if (j == row) continue;
    x = 0;
    for(uint8_t i = 0; i < 4; ++i) {
      if (i == col) continue;
      (*b)[y][x] = a[j][i];
      ++x;
    }
    ++y;
  }
}
float m3minor(const float a[3][3], const uint8_t row, const uint8_t col) {
  float b[2][2];
  m3subm(a, row, col, &b);
  return m2det(b);
}
float m3cof(const float a[3][3], const uint8_t row, const uint8_t col) {
  float z = m3minor(a, row, col);
  return row+col % 2 == 0 ? z : -z;
}

const float I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

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
  for(size_t j = 0; j < canvas->h; ++j) {
    for(size_t i = 0; i < canvas->w; ++i) {
      Vec4 v = vclamp(vmul(pixel_at(canvas, i, j), 255), 0, 255);
      fprintf(f, "%s%d %d %d", (i == 0 ? "" : " "), (int)v.x, (int)v.y, (int)v.z);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}