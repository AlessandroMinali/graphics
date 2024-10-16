#include "ray.h"
#include <stdint.h>
#include <string.h>

#define FILE_SZ 4096

void assert_b(bool actual, bool expected, char *msg) {
  if (actual != expected) { printf("FAILED %s: %d got %d\n", msg, expected, actual); }
}
void assert_f(float actual, float expected, char *msg) {
  if (fabsf(expected - actual) > EPS) { printf("FAILED %s: %0.2f got %0.2f\n", msg, expected, actual); }
}
void assert_s(char *actual, char *expected, char *msg) {
  if (strcmp(actual, expected) != 0) {
    printf("FAILED %s:\n%s got\n%s", msg, expected, actual);
  }
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
void assert_ppm(char* actual_file, char* expected, char* msg) {
  char buf[FILE_SZ] = {0};
  FILE* f = fopen(actual_file, "r");
  fread(buf, FILE_SZ-1, 1, f);
  assert_s(buf, expected, msg);
  fclose(f);
}
void assert_m4(float actual[4][4], float expected[4][4], char* msg) {
  for(uint8_t j = 0; j < 4; ++j) {
    for(uint8_t i = 0; i < 4; ++i) {
      if (EPS < fabsf(actual[j][i] - expected[j][i])) {
        printf("FAILED %s@[%d][%d]:\n%0.2f got\n%0.2f\n", msg, j, i, expected[j][i], actual[j][i]);
      }
    }
  }
}
void assert_m3(float actual[3][3], float expected[3][3], char* msg) {
  for(uint8_t j = 0; j < 3; ++j) {
    for(uint8_t i = 0; i < 3; ++i) {
      if (EPS < fabsf(actual[j][i] - expected[j][i])) {
        printf("FAILED %s@[%d][%d]:\n%0.2f got\n%0.2f\n", msg, j, i, expected[j][i], actual[j][i]);
      }
    }
  }
}
void assert_m2(float actual[2][2], float expected[2][2], char* msg) {
  for(uint8_t j = 0; j < 2; ++j) {
    for(uint8_t i = 0; i < 2; ++i) {
      if (EPS < fabsf(expected[j][i] - actual[j][i])) {
        printf("FAILED %s@[%d][%d]:\n%0.2f got\n%0.2f\n", msg, j, i, expected[j][i], actual[j][i]);
      }
    }
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

  assert_v4(colour_mul(vec4(1, 0.2, 0.4, 0), vec4(0.9, 1, 0.1, 0)), vec4(0.9, 0.2, 0.04, 0), "colour multiply vec4");

  Canvas canvas = canvas_init(10, 20);
  Vec4 zero_v = vec4(0, 0, 0, 0);
  for(uint8_t j = 0; j < canvas.h; ++j) {
    for(uint8_t i = 0; i < canvas.w; ++i) {
      assert_v4(canvas.pixels[j*canvas.w+i], zero_v, "canvas is zeroed");
    }
  }
  Vec4 red = vec4(1, 0, 0, 0);
  write_pixel(&canvas, 2, 3, red);
  assert_v4(pixel_at(&canvas, 2, 3), red, "writing pixel canvas");

  Canvas c = canvas_init(5, 3);
  write_pixel(&c, 0, 0, vec4(1.5, 0, 0, 0));
  write_pixel(&c, 2, 1, vec4(0, 0.5, 0, 0));
  write_pixel(&c, 4, 2, vec4(-0.5, 0, 1, 0));
  canvas_to_ppm(&c);
  assert_ppm("ray.ppm", "P3\n5 3\n255\n255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 127 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n", "canvas to ppm");

  assert_m4((float [4][4]){{1,2,3,4},
                           {5,6,7,8},
                           {9,10,11,12},
                           {13,14,15,16},},
            (float [4][4]){{1,2,3,4},
                           {5,6,7,8},
                           {9,10,11,12},
                           {13,14,15,16},}, "inspect mat4");
  assert_m3((float [3][3]){{-3,5,0},
                           {1,-2,-7},
                           {0,1,1},},
            (float [3][3]){{-3,5,0},
                           {1,-2,-7},
                           {0,1,1},}, "inspect mat3");
  assert_m2((float [2][2]){{-3,5},
                           {1,-2},},
            (float [2][2]){{-3,5},
                           {1,-2},}, "inspect mat2");

  assert_b(meq((float [4][4]){{1,2,3,4},
                              {5,6,7,8},
                              {9,10,11,12},
                              {13,14,15,16},},
               (float [4][4]){{1,2,3,4},
                              {5,6,7,8},
                              {9,10,11,12},
                              {13,14,15,16},}), true, "check equality mat4");
  assert_b(meq((float [4][4]){{1,2,3,4},
                              {5,6,7,8},
                              {9,10,11,12},
                              {13,14,15,16},},
               (float [4][4]){{1,2,3,4},
                              {9,10,11,12},
                              {5,6,7,8},
                              {13,14,15,16},}), false, "check equality mat4");

  float result[4][4];
  float prev[4][4] = {{1,2,3,4},{5,6,7,8},{9,8,7,6},{5,4,3,2}};
  m4mul(
    prev,
    (float[4][4]){{-2,1,2,3},{3,2,1,-1},{4,3,6,5},{1,2,7,8}},
    &result
  );
  assert_m4(
    result,
    (float[4][4]){{20,22,50,48},{44,54,114,108},{40,58,110,102},{16,26,46,42}},
    "multiply mat4");

  assert_v4(
    m4vmul(
      (float[4][4]){{1,2,3,4},{2,4,4,2},{8,6,4,1},{0,0,0,1}},
      vec4(1,2,3,1)
    ),
    vec4(18,24,33,1),
    "multiply mat4 and vec4");

  m4mul(prev, (float[4][4])I, &result);
  assert_m4(
    result,
    prev,
    "multiply mat4 with identity");

  m4t((float[4][4]){{0,9,3,0},{9,8,0,8},{1,8,5,3},{0,0,5,8}}, &result);
  assert_m4(
    result,
    (float[4][4]){{0,9,1,0},{9,8,8,0},{3,0,5,5},{0,8,3,8}},
    "mat4 transpose");

  m4t((float[4][4])I, &result);
  assert_m4(result, (float[4][4])I, "mat4 transpose identity");

  assert_f(m2det((float[2][2]){{1,5},{-3,2}}), 17, "determinant of m2");

  float result2[2][2];
  m3subm((float[3][3]){{1,5,0},{-3,2,7},{0,6,-3}}, 0, 2, &result2);
  assert_m2(
    result2,
    (float[2][2]){{-3,2},{0,6}},
    "m3 sub matrix");
  float result3[3][3];
  m4subm((float[4][4]){{-6,1,1,6},{-8,5,8,6},{-1,0,8,2},{-7,1,-1,1}}, 2, 1, &result3);
  assert_m3(
    result3,
    (float[3][3]){{-6,1,6},{-8,8,6},{-7,-1,1}},
    "m4 sub matrix");

  float tmp3[3][3] = {{3,5,0},{2,-1,-7},{6,-1,5}};
  assert_f(m3minor(tmp3, 0, 0),-12, "m3 minor");
  assert_f(m3cof(tmp3, 0, 0),-12, "m3 cofactor");
  assert_f(m3minor(tmp3, 1, 0),25, "m3 minor");
  assert_f(m3cof(tmp3, 1, 0),-25, "m3 cofactor");

  assert_f(m3cof((float[3][3]){{1,2,6},{-5,8,-4},{2,6,4}}, 0, 0), 56, "m3 cofactor");
  assert_f(m3cof((float[3][3]){{1,2,6},{-5,8,-4},{2,6,4}}, 0, 1), 12, "m3 cofactor");
  assert_f(m3cof((float[3][3]){{1,2,6},{-5,8,-4},{2,6,4}}, 0, 2), -46, "m3 cofactor");
  assert_f(m3det((float[3][3]){{1,2,6},{-5,8,-4},{2,6,4}}), -196, "m3 determinant");

  assert_f(m4cof((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{-6,7,7,-9}}, 0, 0), 690, "m4 cofactor");
  assert_f(m4cof((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{-6,7,7,-9}}, 0, 1), 447, "m4 cofactor");
  assert_f(m4cof((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{-6,7,7,-9}}, 0, 2), 210, "m4 cofactor");
  assert_f(m4cof((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{-6,7,7,-9}}, 0, 3), 51, "m4 cofactor");
  assert_f(m4det((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{-6,7,7,-9}}), -4071, "m4 determinant");

  float tmp4[4][4] = {{-5,2,6,-8},{1,-5,1,8},{7,7,-6,-7},{1,-3,7,4}};
  assert_f(m4det(tmp4), 532, "m4 determinant");
  assert_f(m4cof(tmp4, 2, 3), -160, "m4 cofactor");
  assert_f(m4cof(tmp4, 3, 2), 105, "m4 cofactor");
  float result4[4][4];
  m4inv(tmp4, &result4);
  assert_m4(
    result4,
    (float[4][4]){{0.21805,0.45113,0.24060,-0.04511},{-0.80827,-1.45677,-0.44361,0.52068},{-0.07895,-0.22368,-0.05263,0.19737},{-0.52256,-0.81391,-0.30075,0.30639}},
    "m4 inv");

  float result5[4][4];
  m4inv((float[4][4]){{8,-5,9,2},{7,5,6,1},{-6,0,9,6},{-3,0,-9,-4}}, &result5);
  assert_m4(
    result5,
    (float[4][4]){{-0.15385,-0.15385,-0.28205,-0.53846},{-0.07692,0.12308,0.02564,0.03077},{0.35897,0.35897,0.43590,0.92308},{-0.69231,-0.69231,-0.76923,-1.92308}},
    "m4 inv");

  float result6[4][4];
  m4inv((float[4][4]){{9,3,0,9},{-5,-2,-6,-3},{-4,9,6,4},{-7,6,6,2}}, &result6);
  assert_m4(result6,
    (float[4][4]){{-0.04074,-0.07778,0.14444,-0.22222},{-0.07778,0.03333,0.36667,-0.33333},{-0.02901,-0.14630,-0.10926,0.12963},{0.17778,0.06667,-0.26667,0.33333}},
    "m4 inv");

  float result7[4][4];
  float result8[4][4];
  float result9[4][4];
  m4mul(
    (float[4][4]){{3,-9,7,3},{3.-8,2,-9},{-4,4,4,1},{-6,5,-1,1}},
    (float[4][4]){{8,2,2,2},{3,-1,7,0},{7,0,5,4},{6,-2,0,5}},
    &result8 // C <- A * B
  );
  m4inv((float[4][4]){{8,2,2,2},{3,-1,7,0},{7,0,5,4},{6,-2,0,5}}, &result7); // inv(B)
  m4mul(result8, result7, &result9); // C * inv(B)
  assert_m4(result9, (float[4][4]){{3,-9,7,3},{3.-8,2,-9},{-4,4,4,1},{-6,5,-1,1}}, "m4 product by inv");

  float trans1[4][4];
  transm4(5, -3, 2, &trans1);
  assert_v4(m4vmul(trans1, vec4(-3,4,5,1)), vec4(2,1,7,1), "transformation matrix");
  float invtrans1[4][4];
  m4inv(trans1, &invtrans1);
  assert_v4(m4vmul(invtrans1, vec4(-3,4,5,1)), vec4(-8,7,3,1), "inverse transformation matrix");
  assert_v4(m4vmul(trans1, vec4(-3,4,5,0)), vec4(-3,4,5,0), "transformation matrix with vector");

  float scale1[4][4];
  scalem4(2,3,4, &scale1);
  assert_v4(m4vmul(scale1, vec4(-4,6,8,1)), vec4(-8,18,32,1), "scale matrix");
  assert_v4(m4vmul(scale1, vec4(-4,6,8,0)), vec4(-8,18,32,0), "scale matrix with vector");
  float invscale1[4][4];
  m4inv(scale1, &invscale1);
  assert_v4(m4vmul(invscale1, vec4(-4,6,8,0)), vec4(-2,2,2,0), "inverse transformation matrix");

  scalem4(-1,1,1, &scale1);
  assert_v4(m4vmul(scale1, vec4(2,3,4,1)), vec4(-2,3,4,1), "reflection via scale matrix");

  float eighthrot[4][4];
  rotxm4(PI/4, &eighthrot);
  float quartrot[4][4];
  rotxm4(PI/2, &quartrot);
  assert_v4(m4vmul(eighthrot, vec4(0,1,0,1)), vec4(0,sqrtf(2)/2,sqrtf(2)/2,1), "rotate around x-axis");
  assert_v4(m4vmul(quartrot, vec4(0,1,0,1)), vec4(0,0,1,1), "rotate around x-axis");
  float inveighthrot[4][4];
  m4inv(eighthrot, &inveighthrot);
  assert_v4(m4vmul(inveighthrot, vec4(0,1,0,1)), vec4(0,sqrtf(2)/2,-sqrtf(2)/2,1), "rotate around x-axis");

  rotym4(PI/4, &eighthrot);
  rotym4(PI/2, &quartrot);
  assert_v4(m4vmul(eighthrot, vec4(0,0,1,1)), vec4(sqrtf(2)/2,0,sqrtf(2)/2,1), "rotate around y-axis");
  assert_v4(m4vmul(quartrot, vec4(0,0,1,1)), vec4(1,0,0,1), "rotate around y-axis");

  rotzm4(PI/4, &eighthrot);
  rotzm4(PI/2, &quartrot);
  assert_v4(m4vmul(eighthrot, vec4(0,1,0,1)), vec4(-sqrtf(2)/2,sqrtf(2)/2,0,1), "rotate around z-axis");
  assert_v4(m4vmul(quartrot, vec4(0,1,0,1)), vec4(-1,0,0,1), "rotate around z-axis");

  float shear[4][4];
  shearm4(1,0,0,0,0,0, &shear);
  assert_v4(m4vmul(shear, vec4(2,3,4,1)), vec4(5,3,4,1), "shear matrix");
  shearm4(0,1,0,0,0,0, &shear);
  assert_v4(m4vmul(shear, vec4(2,3,4,1)), vec4(6,3,4,1), "shear matrix");
  shearm4(0,0,1,0,0,0, &shear);
  assert_v4(m4vmul(shear, vec4(2,3,4,1)), vec4(2,5,4,1), "shear matrix");
  shearm4(0,0,0,1,0,0, &shear);
  assert_v4(m4vmul(shear, vec4(2,3,4,1)), vec4(2,7,4,1), "shear matrix");
  shearm4(0,0,0,0,1,0, &shear);
  assert_v4(m4vmul(shear, vec4(2,3,4,1)), vec4(2,3,6,1), "shear matrix");
  shearm4(0,0,0,0,0,1, &shear);
  assert_v4(m4vmul(shear, vec4(2,3,4,1)), vec4(2,3,7,1), "shear matrix");

  rotxm4(PI/2, &quartrot);
  scalem4(5,5,5, &scale1);
  transm4(10,5,7, &trans1);
  assert_v4(m4vmul(quartrot, vec4(1,0,1,1)), vec4(1,-1,0,1), "sequence of matrices");
  assert_v4(m4vmul(scale1, vec4(1,-1,0,1)), vec4(5,-5,0,1), "sequence of matrices");
  assert_v4(m4vmul(trans1, vec4(5,-5,0,1)), vec4(15,0,7,1), "sequence of matrices");

  float scale1_quartrot[4][4];
  float chainm4[4][4];
  m4mul(scale1, quartrot, &scale1_quartrot);
  m4mul(trans1, scale1_quartrot, &chainm4);
  assert_v4(m4vmul(chainm4, vec4(1,0,1,1)), vec4(15,0,7,1), "chain of matrices");

  Ray r = {{ vec4(2,3,4,1), vec4(1,0,0,0) }};
  assert_v4(rpos(r, 0), vec4(2,3,4,1), "point along ray");
  assert_v4(rpos(r, 1), vec4(3,3,4,1), "point along ray");
  assert_v4(rpos(r, -1), vec4(1,3,4,1), "point along ray");
  assert_v4(rpos(r, 2.5), vec4(4.5,3,4,1), "point along ray");

  r.org = vec4(0,0,-5,1);
  r.dir = vec4(0,0,1,0);
  Sphere s = sphere();
  Intersections h = intersect(r, s);
  assert_f(h.count, 2, "ray hit sphere");
  assert_f(h.raw[0].t, 4, "ray hit sphere");
  assert_f(h.raw[1].t, 6, "ray hit sphere");
  assert_v4(h.raw[0].s.org, s.org, "ray hit sphere sets obj");
  assert_v4(h.raw[1].s.org, s.org, "ray hit sphere sets obj");

  r.org = vec4(0,1,-5,1);
  h = intersect(r, s);
  assert_f(h.count, 2, "ray hit sphere tangent");
  assert_f(h.raw[0].t, 5, "ray hit sphere tangent");
  assert_f(h.raw[1].t, 5, "ray hit sphere tangent");

  r.org = vec4(0,2,-5,1);
  h = intersect(r, s);
  assert_f(h.count, 0, "ray hit sphere miss");

  r.org = vec4(0,0,0,1);
  h = intersect(r, s);
  assert_f(h.count, 2, "ray hit sphere inside");
  assert_f(h.raw[0].t, -1, "ray hit sphere inside");
  assert_f(h.raw[1].t, 1, "ray hit sphere inside");

  r.org = vec4(0,0,5,1);
  h = intersect(r, s);
  assert_f(h.count, 2, "ray hit sphere behind");
  assert_f(h.raw[0].t, -6, "ray hit sphere behind");
  assert_f(h.raw[1].t, -4, "ray hit sphere behind");

  Intersections fake_hits;
  fake_hits.count = 2;
  fake_hits.raw[0] = (Hit){1, s};
  fake_hits.raw[1] = (Hit){2, s};
  assert_f(hit(&fake_hits).t, 1, "hit all pos");
  fake_hits.raw[0] = (Hit){-1, s};
  fake_hits.raw[1] = (Hit){1, s};
  assert_f(hit(&fake_hits).t, 1, "hit some neg");
  fake_hits.raw[0] = (Hit){-2, s};
  fake_hits.raw[1] = (Hit){-1, s};
  assert_f(hit(&fake_hits).t, FLT_MAX, "hit all neg");
  fake_hits.count = 4;
  fake_hits.raw[0] = (Hit){5, s};
  fake_hits.raw[1] = (Hit){7, s};
  fake_hits.raw[2] = (Hit){-3, s};
  fake_hits.raw[3] = (Hit){2, s};
  assert_f(hit(&fake_hits).t, 2, "hit with many");

  float transray[4][4] = {{0}};
  transm4(3,4,5, &transray);
  r.org = vec4(1,2,3,1);
  r.dir = vec4(0,1,0,0);
  r = rtrans(r, &transray);
  assert_v4(r.org, vec4(4,6,8,1), "translate ray");
  assert_v4(r.dir, vec4(0,1,0,0), "translate ray");

  scalem4(2,3,4, &transray);
  r.org = vec4(1,2,3,1);
  r = rtrans(r, &transray);
  assert_v4(r.org, vec4(2,6,12,1), "scale ray");
  assert_v4(r.dir, vec4(0,3,0,0), "scale ray");

  float transsphere[4][4] = {{0}};
  s = sphere();
  assert_m4(s.transform, (float[4][4]) I, "sphere transformation");
  transm4(2,3,4, &transsphere);
  transm4(2,3,4, &s.transform);
  assert_m4(s.transform, transsphere, "sphere transformation");

  r.org = vec4(0,0,-5,1);
  r.dir = vec4(0,0,1,0);
  s = sphere();
  scalem4(2,2,2, &s.transform);
  h = intersect(r, s);
  assert_f(h.count, 2, "scaled sphere hit");
  assert_f(h.raw[0].t, 3, "scaled sphere hit");
  assert_f(h.raw[1].t, 7, "scaled sphere hit");

  r.org = vec4(0,0,-5,1);
  r.dir = vec4(0,0,1,0);
  s = sphere();
  transm4(5,0,0, &s.transform);
  h = intersect(r, s);
  assert_f(h.count, 0, "translated sphere hit");

  s = sphere();
  assert_v4(snrm(s, vec4(1,0,0,1)), vec4(1,0,0,0), "normal of sphere");
  assert_v4(snrm(s, vec4(0,1,0,1)), vec4(0,1,0,0), "normal of sphere");
  assert_v4(snrm(s, vec4(0,0,1,1)), vec4(0,0,1,0), "normal of sphere");
  float tmp = sqrtf(3) / 3;
  assert_v4(snrm(s, vec4(tmp,tmp,tmp,1)), vec4(tmp,tmp,tmp,0), "normal of sphere");
  assert_v4(snrm(s, vec4(tmp,tmp,tmp,1)), vnrm(vec4(tmp,tmp,tmp,0)), "normal of sphere is normalized");

  transm4(0,1,0, &s.transform);
  assert_v4(snrm(s, vec4(0,1.70711,-0.70711,1)), vec4(0,0.70711,-0.70711,0), "normal of translated sphere");
  scalem4(1,0.5,1, &s.transform);
  rotzm4(PI/5, &transsphere);
  m4mul(s.transform, transsphere, &s.transform);
  assert_v4(snrm(s, vec4(0,sqrtf(2)/2,-sqrtf(2)/2,1)), vec4(0,0.97014,-0.24254,0), "normal of scaled+rotz sphere");

  assert_v4(reflect(vec4(1,-1,0,0), vec4(0,1,0,0)), vec4(1,1,0,0), "reflect vector");
  assert_v4(reflect(vec4(0,-1,0,0), vec4(sqrtf(2)/2,sqrtf(2)/2,0,0)), vec4(1,0,0,0), "reflect vector");

  Vec4 eye = vec4(0,0,-1,0);
  Vec4 nrm = vec4(0,0,-1,0);
  Ray light = {{ vec4(0,0,-10,1), vec4(1,1,1,1) }};
  Vec4 pos = vec4(0,0,0,1);
  assert_v4(lighting(s.material, light, pos, eye, nrm), vec4(1.9,1.9,1.9,1), "eye between light and surface");
  eye = vec4(0,sqrtf(2)/2,-sqrtf(2)/2,0);
  assert_v4(lighting(s.material, light, pos, eye, nrm), vec4(1,1,1,1), "eye 45deg between light and surface");
  eye = vec4(0,0,-1,0);
  light.pos = vec4(0,10,-10,1);
  assert_v4(lighting(s.material, light, pos, eye, nrm), vec4(0.7364,0.7364,0.7364,1), "eye between light 45deg and surface");
  eye = vec4(0,-sqrtf(2)/2,-sqrtf(2)/2,0);
  assert_v4(lighting(s.material, light, pos, eye, nrm), vec4(1.6364,1.6364,1.6364,1), "eye reflect light at surface");
  light.pos = vec4(0,0,10,1);
  assert_v4(lighting(s.material, light, pos, eye, nrm), vec4(0.1,0.1,0.1,1), "light behind surface");

  World w = default_world();
  assert_v4(w.light.pos, vec4(-10,10,-10,1), "world light");
  assert_v4(w.light.pow, vec4(1,1,1,1), "world light");
  assert_v4(w.obj[0].material.clr, vec4(0.8,1,0.6,1), "world sphere1 material");
  assert_f(w.obj[0].material.diffuse, 0.7, "world sphere1 material");
  assert_f(w.obj[0].material.specular, 0.2, "world sphere1 material");
  scalem4(0.5,0.5,0.5,&transsphere);
  assert_m4(w.obj[1].transform, transsphere, "world sphere2 transform");

  r.org = vec4(0,0,-5,1);
  r.dir = vec4(0,0,1,0);
  h = intersect_world(r, w);
  assert_f(h.count, 4, "world intersections");
  assert_f(h.raw[0].t, 4, "world intersections");
  assert_f(h.raw[1].t, 4.5, "world intersections");
  assert_f(h.raw[2].t, 5.5, "world intersections");
  assert_f(h.raw[3].t, 6, "world intersections");

  s = sphere();
  Computations comp = prepare_computations((Hit){{4, s}}, r);
  assert_f(comp.hit.t, 4, "pre computation");
  assert_v4(comp.hit.s.org, s.org, "pre computation");
  assert_f(comp.hit.s.r, s.r, "pre computation");
  assert_v4(comp.pos, vec4(0,0,-1,1), "pre computation");
  assert_v4(comp.eye, vec4(0,0,-1,0), "pre computation");
  assert_v4(comp.nrm, vec4(0,0,-1,0), "pre computation");
  assert_f(comp.inside, false, "pre computation");

  r.org = vec4(0,0,0,1);
  comp = prepare_computations((Hit){{1, s}}, r);
  assert_v4(comp.pos, vec4(0,0,1,1), "pre computation");
  assert_v4(comp.eye, vec4(0,0,-1,0), "pre computation");
  assert_v4(comp.nrm, vec4(0,0,-1,0), "pre computation");
  assert_f(comp.inside, true, "pre computation");

  r.org = vec4(0,0,-5,1);
  s = w.obj[0];
  comp = prepare_computations((Hit){{4, s}}, r);
  assert_v4(shade_hit(w, comp), vec4(0.38066, 0.47583, 0.2855, 1), "shade hit");
  r.org = vec4(0,0,0,1);
  w.light.pos = vec4(0,0.25,0,1);
  s = w.obj[1];
  comp = prepare_computations((Hit){{0.5, s}}, r);
  assert_v4(shade_hit(w, comp), vec4(0.90498, 0.90498, 0.90498, 1), "shade hit");

  r.org = vec4(0,0,-5,1);
  r.dir = vec4(0,1,0,0);
  w = default_world();
  assert_v4(colour_at(w, r), vec4(0,0,0,1), "colour_at");
  r.dir = vec4(0,0,1,0);
  assert_v4(colour_at(w, r), vec4(0.38066, 0.47583, 0.2855,1), "colour_at");
  r.org = vec4(0,0,0.75,1);
  r.dir = vec4(0,0,-1,0);
  w.obj[0].material.ambient = 1;
  w.obj[1].material.ambient = 1;
  assert_v4(colour_at(w, r), w.obj[1].material.clr, "colour_at");

  view_transform(vec4(0,0,0,1), vec4(0,0,-1,1), vec4(0,1,0,0), &trans1);
  assert_m4(trans1, (float[4][4])I, "default view transform");
  scalem4(-1,1,-1, &scale1);
  view_transform(vec4(0,0,0,1), vec4(0,0,1,1), vec4(0,1,0,0), &trans1);
  assert_m4(trans1, scale1, "pos-z view transform");
  transm4(0,0,-8, &scale1);
  view_transform(vec4(0,0,8,1), vec4(0,0,0,1), vec4(0,1,0,0), &trans1);
  assert_m4(trans1, scale1, "translate view transform");
  view_transform(vec4(1,3,2,1), vec4(4,-2,8,1), vec4(1,1,0,0), &trans1);
  float arb_view[4][4] = {{-0.50709,0.50709, 0.67612, -2.36643},{0.76772,0.60609,0.12122,-2.82843},{-0.35857,0.59761,-0.71714,0},{0,0,0,1}};
  assert_m4(trans1, arb_view, "arbitrary view transform");

  Camera cam = camera(160,120, PI/2);
  assert_f(cam.hsize, 160, "camera");
  assert_f(cam.vsize, 120, "camera");
  assert_f(cam.fov, PI/2, "camera");
  assert_m4(cam.transform, (float[4][4])I, "cam");

  cam = camera(200,125, PI/2);
  assert_f(cam.pixel_sz, 0.01, "hoz cam");
  cam = camera(125,200, PI/2);
  assert_f(cam.pixel_sz, 0.01, "vert cam");

  cam = camera(201,101,PI/2);
  r = ray_for_pixel(cam, 100, 50);
  assert_v4(r.org, vec4(0,0,0,1), "ray through pixel");
  assert_v4(r.dir, vec4(0,0,-1,0), "ray through pixel");
  r = ray_for_pixel(cam, 0, 0);
  assert_v4(r.org, vec4(0,0,0,1), "ray through pixel");
  assert_v4(r.dir, vec4(0.66519,0.33259,-0.66851,0), "ray through pixel");
  rotym4(PI/4, &cam.transform);
  transm4(0,-2,5, &trans1);
  m4mul(cam.transform, trans1, &cam.transform);
  r = ray_for_pixel(cam, 100, 50);
  assert_v4(r.org, vec4(0,2,-5,1), "ray through pixel");
  assert_v4(r.dir, vec4(sqrtf(2)/2,0,-sqrtf(2)/2,0), "ray through pixel");

  w = default_world();
  cam = camera(11, 11, PI/2);
  view_transform(vec4(0,0,-5,1), vec4(0,0,0,1), vec4(0,1,0,0), &cam.transform);
  c = render(cam, w);
  assert_v4(pixel_at(&c, 5, 5), vec4(0.38066, 0.47583, 0.2855, 1), "render");

  printf("Tests ran.\n");

  return 0;
}
