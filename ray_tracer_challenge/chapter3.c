#include "ray.h"
#include <time.h>

int main(int argc, char** argv) {

  // 1 inverse identity matrix: gives neg zeros(!) but really nothing
  float invI[4][4];
  m4inv(I, &invI);
  for(size_t j = 0; j < 4; ++j) {
    for(size_t i = 0; i < 4; ++i) {
      printf("%0.2f ", invI[j][i]);
    }
    printf("\n");
  }
  printf("\n");

  // 2 matrix x inverse: gives identity matrix!
  float a[4][4] = (float[4][4]){{3,-9,7,3},{3.-8,2,-9},{-4,4,4,1},{-6,5,-1,1}};
  float inva[4][4];
  float b[4][4];
  m4inv(a, &inva);
  m4mul(a, inva, &b);
  for(size_t j = 0; j < 4; ++j) {
    for(size_t i = 0; i < 4; ++i) {
      printf("%0.2f ", b[j][i]);
    }
    printf("\n");
  }
  printf("\n");

  // 3 A^-1^T vs A^T^-1: no difference!
  float transinva[4][4];
  m4t(inva, &transinva);

  float transa[4][4];
  m4t(a, &transa);
  float invtransa[4][4];
  m4inv(transa, &invtransa);

  for(size_t j = 0; j < 4; ++j) {
    for(size_t i = 0; i < 4; ++i) {
      if (EPS < fabsf(transinva[j][i] - invtransa[j][i])) {
        printf("difference found | A^-1^T:%0.2f, A^T^-1:%0.2f\n", transinva[j][i], invtransa[j][i]);
      }
    }
  }

  // 4 change one value of identity matrix: that row sum is the corresponding row value of vector!
  float fakeI[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
  srand(time(NULL));
  size_t x = rand() % 4;
  size_t y = rand() % 4;
  int value = rand() % 100;
  fakeI[y][x] = value;
  printf("placing %d at { row: %zu, col: %zu }\n", value, x+1, y+1);
  Vec4 v = m4vmul(fakeI, vec4(1,1,1,1));
  print_v4(v);
  printf("\n");
}