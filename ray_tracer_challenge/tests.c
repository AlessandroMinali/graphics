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
      if (actual[j][i] != expected[j][i]) {
        printf("FAILED %s@[%d][%d]:\n%0.2f got\n%0.2f\n", msg, j, i, expected[j][i], actual[j][i]);
      }
    }
  }
}
void assert_m3(float actual[3][3], float expected[3][3], char* msg) {
  for(uint8_t j = 0; j < 3; ++j) {
    for(uint8_t i = 0; i < 3; ++i) {
      if (actual[j][i] != expected[j][i]) {
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
  float prev[4][4] = (float[4][4]){{1,2,3,4},{5,6,7,8},{9,8,7,6},{5,4,3,2}};
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

  m4mul(prev, I, &result);
  assert_m4(
    result,
    prev,
    "multiply mat4 with identity");

  m4t((float[4][4]){{0,9,3,0},{9,8,0,8},{1,8,5,3},{0,0,5,8}}, &result);
  assert_m4(
    result,
    (float[4][4]){{0,9,1,0},{9,8,8,0},{3,0,5,5},{0,8,3,8}},
    "mat4 transpose");

  m4t(I, &result);
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
  assert_m4(result, I, "mat4 transpose identity");
  #pragma GCC diagnostic pop

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

  float tmp3[3][3] = (float[3][3]){{3,5,0},{2,-1,-7},{6,-1,5}};
  assert_f(m3minor(tmp3, 0, 0),-12, "m3 minor");
  assert_f(m3cof(tmp3, 0, 0),-12, "m3 cofactor");
  assert_f(m3minor(tmp3, 1, 0),25, "m3 minor");
  assert_f(m3cof(tmp3, 1, 0),-25, "m3 cofactor");

  assert_f(m3cof((float[3][3]){{1,2,6},{-5,8,-4},{2,6,4}}, 0, 0), 56, "m3 cofactor");
  assert_f(m3cof((float[3][3]){{1,2,6},{-5,8,-4},{2,6,4}}, 0, 1), 12, "m3 cofactor");
  assert_f(m3cof((float[3][3]){{1,2,6},{-5,8,-4},{2,6,4}}, 0, 2), -46, "m3 cofactor");
  // assert_f(m3det((float[3][3]){{1,2,6},{-5,8,-4},{2,6,4}}), -196, "m3 determinant");

  // assert_f(m4cof((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{1,2,-9,6},{-6,7,7,-9}}, 0, 0), 690, "m4 cofactor");
  // assert_f(m4cof((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{1,2,-9,6},{-6,7,7,-9}}, 0, 1), 447, "m4 cofactor");
  // assert_f(m4cof((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{1,2,-9,6},{-6,7,7,-9}}, 0, 2), 210, "m4 cofactor");
  // assert_f(m4cof((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{1,2,-9,6},{-6,7,7,-9}}, 0, 3), 51, "m4 cofactor");
  // assert_f(m4det((float[4][4]){{-2,-8,3,5},{-3,1,7,3},{1,2,-9,6},{1,2,-9,6},{-6,7,7,-9}}), -4071, "m4 determinant");

  printf("Tests ran.\n");
  return 0;
}