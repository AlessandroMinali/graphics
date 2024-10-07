#include "ray.h"

int main(int argc, char** argv) {
  Ray r = { vec4(0,0,-5,1), vec4(0,0,1,0) };
  float wall_z = 10;
  float wall_size = 7;

  float canvas_pixels = 100;
  float pixel_size = wall_size / canvas_pixels;
  float half = wall_size / 2.0;

  Vec4 clr = vec4(1,0,0,1);
  Sphere s = sphere();

  // scalem4(1,0.5,1, &s.transform);

  scalem4(0.5,1,1, &s.transform);

  // float scale[4][4]; scalem4(0.5,1,1, &scale);
  float rotz[4][4]; rotzm4(PI/4, &rotz);
  m4mul(rotz, s.transform, &s.transform);

  // float scale[4][4]; scalem4(0.5,1,1, &scale);
  // float shear[4][4]; shearm4(1,0,0,0,0,0, &shear);
  // m4mul(shear, scale, &s.transform);

  Canvas c = canvas_init(canvas_pixels, canvas_pixels);
  for(int j = 0; j < canvas_pixels; ++j){
    float world_y = half - pixel_size * j;
    for(int i = 0; i < canvas_pixels; ++i) {
      float world_x = -half + pixel_size * i;

      Vec4 position = vec4(world_x, world_y, wall_z, 1);
      r.dir = vnrm(vsub(position, r.org));
      Intersections intersections = intersect(r, s);

      if (0 < intersections.count) {
        write_pixel(&c, i, j, vmul(clr, hit(&intersections).t / wall_z));
      }
    }
  }

  canvas_to_ppm(&c);

  return 0;
}
