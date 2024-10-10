// implementation from The Ray Tracer Challenge:
// A Test-Driven Guide to Your First 3D Renderer by James Buck
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define EPS 0.0001
#define PI 3.14159
#define I {{1.0,0,0,0},{0,1.0,0,0},{0,0,1.0,0},{0,0,0,1.0}}
#define MAX_HIT 256
#define MAX_OBJ 32
#define PPM_HEADER 127

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc11-extensions"
typedef union {
  struct {
    float x;
    float y;
    float z;
    float w;
  };
  struct {
    float r;
    float g;
    float b;
    float a;
  };
} Vec4;

typedef union {
  struct {
    Vec4 org;
    Vec4 dir;
  };
  struct { // Point Light
    Vec4 pos;
    Vec4 pow;
  };
} Ray;
typedef struct {
    Vec4 clr;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
} Material;
typedef struct {
  Vec4 org;
  float r;
  float transform[4][4];
  Material material;
} Sphere;
typedef struct {
  Ray light;
  float count;
  Sphere obj[MAX_OBJ];
} World;
typedef union {
  struct {
    float t;
    Sphere s;
  };
} Hit;
typedef struct {
  int count;
  Hit raw[MAX_HIT];
} Intersections;
typedef struct {
  Vec4 pos;
  Vec4 eye;
  Vec4 nrm;
  Hit hit;
  bool inside;
} Computations;

typedef struct {
  size_t w;
  size_t h;
  size_t sz;
  Vec4 *pixels;
} Canvas;
#pragma GCC diagnostic pop

int sort(const void *elem1, const void *elem2) {
  float f = *((float*)elem1);
  float s = *((float*)elem2);

  if (s < f) return  1;
  if (f < s) return -1;
  return 0;
}

static inline void print_v4(const Vec4 v) { printf("( %0.2f, %0.2f, %0.2f, %0.2f )", v.x, v.y, v.z, v.w); }
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

static inline void print_m4(const float a[4][4]) {
  for(uint8_t j = 0; j < 4; ++j) { printf("[ %0.2f, %0.2f, %0.2f, %0.2f ]\n", a[j][0],a[j][1],a[j][2],a[j][3]);}
  printf("\n");
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
  float t[4][4];
  for(uint8_t j = 0; j < 4; ++j) {
    for(uint8_t i = 0; i < 4; ++i) {
      t[j][i] = a[j][0] * b[0][i] +
                a[j][1] * b[1][i] +
                a[j][2] * b[2][i] +
                a[j][3] * b[3][i];
    }
  }
  for(uint8_t j = 0; j < 4; ++j) {
    for(uint8_t i = 0; i < 4; ++i) {
      (*c)[j][i] = t[j][i];
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
  float t[4][4];
  for(uint8_t j = 0; j < 4; ++j) {
    for(uint8_t i = 0; i < 4; ++i) {
      t[j][i] = a[i][j];
    }
  }
  for(uint8_t j = 0; j < 4; ++j) {
    for(uint8_t i = 0; i < 4; ++i) {
      (*b)[j][i] = t[j][i];
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
  return (row+col) % 2 == 0 ? z : -z;
}
float m3det(const float a[3][3]) {
  return m3cof(a, 0, 0) * a[0][0] + m3cof(a, 0, 1) * a[0][1] + m3cof(a, 0, 2) * a[0][2];
}
float m4minor(const float a[4][4], const uint8_t row, const uint8_t col) {
  float b[3][3];
  m4subm(a, row, col, &b);
  return m3det(b);
}
float m4cof(const float a[4][4], const uint8_t row, const uint8_t col) {
  float z = m4minor(a, row, col);
  return (row+col) % 2 == 0 ? z : -z;
}
float m4det(const float a[4][4]) {
  return m4cof(a, 0, 0) * a[0][0] + m4cof(a, 0, 1) * a[0][1] + m4cof(a, 0, 2) * a[0][2] + m4cof(a, 0, 3) * a[0][3];
}
void m4inv(const float a[4][4], float (*b)[4][4]) {
  float det = m4det(a);
  if (det == 0) {
    printf("ERROR: mat4 non-invertible\n");
    exit(1);
  }

  float t[4][4];
  for (uint8_t j = 0; j < 4; ++j) {
    for (uint8_t i = 0; i < 4; ++i) {
      t[i][j] = m4cof(a, j, i) / det;
    }
  }
  for(uint8_t j = 0; j < 4; ++j) {
    for(uint8_t i = 0; i < 4; ++i) {
      (*b)[j][i] = t[j][i];
    }
  }
}
void transm4(const float x, const float y, const float z, float (*a)[4][4]) {
  (*a)[0][0] = (*a)[1][1] = (*a)[2][2] = (*a)[3][3] = 1;
  (*a)[0][1] = (*a)[0][2] = (*a)[1][0] = (*a)[1][2] = (*a)[2][0] = (*a)[2][1] = (*a)[3][0] = (*a)[3][1] = (*a)[3][2] = 0;
  (*a)[0][3] = x;
  (*a)[1][3] = y;
  (*a)[2][3] = z;
}
void scalem4(const float x, const float y, const float z, float (*a)[4][4]) {
  (*a)[0][1] = (*a)[0][2] = (*a)[0][3] =
  (*a)[1][0] = (*a)[1][2] = (*a)[1][3] =
  (*a)[2][0] = (*a)[2][1] = (*a)[2][3] =
  (*a)[3][0] = (*a)[3][1] = (*a)[3][2] = 0;
  (*a)[0][0] = x;
  (*a)[1][1] = y;
  (*a)[2][2] = z;
  (*a)[3][3] = 1;
}
void rotxm4(const float rad, float (*a)[4][4]) {
  (*a)[0][1] = (*a)[0][2] = (*a)[0][3] =
  (*a)[1][0] = (*a)[1][3] =
  (*a)[2][0] = (*a)[2][3] =
  (*a)[3][0] = (*a)[3][1] = (*a)[3][2] = 0;
  (*a)[0][0] = (*a)[3][3]= 1;
  (*a)[1][1] = (*a)[2][2] = cos(rad);
  (*a)[2][1] = (*a)[1][2] = sin(rad);
  (*a)[1][2] = -(*a)[1][2];
}
void rotym4(const float rad, float (*a)[4][4]) {
  (*a)[0][1] = (*a)[0][3] =
  (*a)[1][0] = (*a)[1][2] = (*a)[1][3] =
  (*a)[2][1] = (*a)[2][3] =
  (*a)[3][0] = (*a)[3][1] = (*a)[3][2] = 0;
  (*a)[1][1] = (*a)[3][3] = 1;
  (*a)[0][0] = (*a)[2][2] = cos(rad);
  (*a)[2][0] = (*a)[0][2] = sin(rad);
  (*a)[2][0] = -(*a)[2][0];
}
void rotzm4(const float rad, float (*a)[4][4]) {
  (*a)[0][1] = (*a)[0][2] = (*a)[0][3] =
  (*a)[1][0] = (*a)[1][2] = (*a)[1][3] =
  (*a)[2][0] = (*a)[2][1] = (*a)[2][3] =
  (*a)[3][0] = (*a)[3][1] = (*a)[3][2] = 0;
  (*a)[2][2] = (*a)[3][3] = 1;
  (*a)[0][0] = (*a)[1][1] = cos(rad);
  (*a)[1][0] = (*a)[0][1] = sin(rad);
  (*a)[0][1] = -(*a)[0][1];
}
void shearm4(const float xy, const float xz,
             const float yx, const float yz,
             const float zx, const float zy,
             float (*a)[4][4]) {
  (*a)[0][0] = (*a)[1][1] = (*a)[2][2] = (*a)[3][3] = 1;
  (*a)[3][0] = (*a)[3][1] = (*a)[3][2] = (*a)[0][3] = (*a)[1][3] = (*a)[2][3] = 0;
  (*a)[0][1] = xy;
  (*a)[0][2] = xz;
  (*a)[1][0] = yx;
  (*a)[1][2] = yz;
  (*a)[2][0] = zx;
  (*a)[2][1] = zy;
}

Sphere sphere() {
  return (Sphere){ vec4(0,0,0,1), 1, I, { {{1,1,1,1}}, 0.1, 0.9, 0.9, 200 } };
}
Vec4 snrm(const Sphere s, const Vec4 p) {
  float t[4][4];

  m4inv(s.transform, &t);
  Vec4 o_n = vsub(m4vmul(t, p), s.org);
  m4t(t, &t);
  Vec4 w_n = m4vmul(t, o_n);
  w_n.w = 0;
  return vnrm(w_n);
}

World world() {
  Sphere s1 = sphere();
  s1.material = (Material){ {{0.8,1,0.6,1}}, 0.1, 0.7, 0.2, 200 };
  Sphere s2 = sphere();
  scalem4(0.5,0.5,0.5,&s2.transform);

  return (World){ {{vec4(-10,10,-10,1), vec4(1,1,1,1)}}, 2, {s1, s2}};
}

Vec4 reflect(Vec4 in, Vec4 nrm) {
  return vsub(in, vmul(nrm, vdot(in, nrm) * 2));
}
Vec4 lighting(Material m, Ray l, Vec4 p, Vec4 e, Vec4 n) {
  Vec4 clr = colour_mul(m.clr, l.pow);
  Vec4 lightDir = vnrm(vsub(l.pos, p));

  float a = m.ambient;
  float d = 0;
  float s = 0;

  float g_term = vdot(n, lightDir);
  if (g_term > 0) {
    d = m.diffuse * g_term;

    Vec4 r = reflect(vneg(lightDir), n);
    float r_term = vdot(r, e);
    if (r_term > 0) {
      s = m.specular * powf(r_term, m.shininess);
    }
  }

  clr = vmul(clr, a+d+s);
  clr.a = 1;
  return clr;
}

Vec4 rpos(const Ray r, const float t) {
  return vadd(r.org, vmul(r.dir, t));
}
Ray rtrans(const Ray r, const float (*a)[4][4]) {
  return (Ray){{ m4vmul((*a), r.org), m4vmul((*a), r.dir) }};
}
Intersections intersect(const Ray r, const Sphere s) {
  float t[4][4];
  m4inv(s.transform, &t);
  Ray r_t = rtrans(r, &t);

  Vec4 r_s = vsub(r_t.org, s.org);
  float a = vdot(r_t.dir, r_t.dir);
  float b = 2 * vdot(r_t.dir, r_s);
  float c = vdot(r_s, r_s) - 1;
  float d = b*b - 4*a*c;

  if (d < 0) {
    return (Intersections){ .count = 0 };
  } else {
    float t1 = (-b - sqrtf(d)) / (2 * a);
    float t2 = (-b + sqrtf(d)) / (2 * a);

    return (Intersections){ .count = 2, .raw = {{t1, s}, {t2, s}}};
  }
}
Intersections intersect_world(const Ray r, const World w) {
  Intersections tmp_intersections;
  Intersections intersections;
  size_t count = 0;
  for(uint8_t i = 0; i < w.count; ++i) {
    tmp_intersections = intersect(r, w.obj[i]);
    for(uint8_t j = 0; j < tmp_intersections.count; ++j) {
      if (tmp_intersections.raw[j].t < 0) continue; // NOTE: ignore neg hits
      intersections.raw[count++] = tmp_intersections.raw[j];
    }
  }
  intersections.count = count;
  if (count > MAX_HIT) printf("error: max hit limit exceeded"); exit(1);
  qsort(intersections.raw, intersections.count, sizeof(Hit), sort);

  return intersections;
}
Hit hit(Intersections *intersections) { // TODO: not actually used since we sort in <intersect_world>
  Hit tmp;
  tmp.t = FLT_MAX;
  for(size_t i = 0; i < intersections->count; ++i) {
    if (intersections->raw[i].t > 0 && intersections->raw[i].t < tmp.t) {
      tmp.t = intersections->raw[i].t;
      tmp.s = intersections->raw[i].s;
    }
  }

  return (Hit){tmp.t, tmp.s};
}
Computations prepare_computations(const Hit h,  Ray r) {
  Vec4 pos = rpos(r, h.t);
  Vec4 eye = vneg(r.dir);
  Vec4 nrm = snrm(h.s, pos);
  bool inside = false;

  if (vdot(nrm, eye) < 0) {
    inside = true;
    nrm = vneg(nrm);
  }

  return (Computations){pos, eye, nrm, h, inside };
}
Vec4 shade_hit(World w, Computations c) {
  return lighting(c.hit.s.material, w.light, c.pos, c.eye, c.nrm);
}
Vec4 colour_at(World w, Ray r) {
  Intersections i = intersect_world(r, w);
  if (i.count == 0) return vec4(0,0,0,1);

  return shade_hit(w, prepare_computations(i.raw[0], r));
}

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
