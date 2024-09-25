// end of chapter test program
#include "ray.h"

typedef struct {
  Vec4 pos;
  Vec4 vel;
} Projectile;
typedef struct {
  Vec4 gravity;
  Vec4 wind;
} Environment;

Projectile tick(Environment e, Projectile p) {
  return (Projectile){ vadd(p.pos, p.vel), vadd(vadd(p.vel, e.gravity), e.wind) };
}

int main(int argc, char const *argv[])
{
  Projectile p = { vec4(0, 1, 0, 0), vnrm(vec4(1, 1, 0, 0)) };
  Environment e = { vec4(0, -0.1, 0, 0), vec4(-0.01, 0, 0, 0) };

  p.vel = vmul(p.vel, 1);
  int count = 1;
  while(1) {
    p = tick(e, p);
    printf("%d: ", count);
    print_v4(p.pos);
    printf("\n");
    ++count;
    if (p.pos.y <= 0) { break; }
  }

  return 0;
}
