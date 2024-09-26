// end of chapter test program
#include "ray.h"

#define PADDING 50
#define VEL 11.25

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

int main(int argc, char const *argv[]) {
  Projectile p = { vec4(0, 1, 0, 0), vmul(vnrm(vec4(1, 1.8, 0, 0)), VEL) };
  Environment e = { vec4(0, -0.1, 0, 0), vec4(-0.01, 0, 0, 0) };
  
  // dry run to determine canvas sz
  size_t max_x = 0;
  size_t max_y = 0;
  while(1) {
    p = tick(e, p);
    if (p.pos.y <= 0) { break; }
    if (max_x < p.pos.x) { max_x = p.pos.x; }
    if (max_y < p.pos.y) { max_y = p.pos.y; }
  }

  printf("%zu %zu\n", max_x, max_y);
  p = (Projectile){ vec4(0, 1, 0, 0), vmul(vnrm(vec4(1, 1.8, 0, 0)), VEL) };

  Canvas c = canvas_init(max_x, max_y + PADDING);
  Vec4 colour = vec4(1, 1, 1, 0);
  p.vel = vmul(p.vel, 1);
  while(1) {
    p = tick(e, p);
    if (p.pos.y <= 0) { break; }
    write_pixel(&c, p.pos.x, c.h-p.pos.y, colour);
  }
  canvas_to_ppm(&c);

  return 0;
}
