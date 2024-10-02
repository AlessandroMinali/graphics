// draw positions of clock face
#include "ray.h"

int main(int argc, char** argv) {
  int w = 600;
  int h = 400;
  Canvas c = canvas_init(w, h);

  int scale = h / 2.5;
  Vec4 p = vec4(0,-scale,0,1);
  Vec4 new_p;

  float rot[4][4];
  Vec4 clr = {{1,1,1}};
  for(uint8_t i = 0; i < 12; ++i) {
    rotzm4(PI/6 * i, &rot); // rot to next pos

    new_p = vadd(
      m4vmul(rot, p),   // transform
      vec4(w/2,h/2,0,0) // move to origin
    );

    write_pixel(&c, new_p.x, new_p.y, clr);
  }

  canvas_to_ppm(&c);

  return 0;
}