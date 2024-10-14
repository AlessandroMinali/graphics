#include "ray.h"

void push(World* w, Sphere* s) {
  w->obj[w->count] = *s;
  ++w->count;
}

int main(int argc, char** argv) {
  float tmp[4][4];
  World w = {{{vec4(-10,10,-10,1), vec4(1,1,1,1)}}, 0};

  Sphere floor = sphere();
  scalem4(10,0.1,10, &floor.transform);
  floor.material.clr = vec4(1,0.9,0.9,1);
  floor.material.specular = 0;
  push(&w, &floor);

  Sphere left_wall = sphere();
  transm4(0,0,5, &left_wall.transform);
  rotym4(-PI/4, &tmp);
  m4mul(left_wall.transform, tmp, &left_wall.transform);
  rotxm4(PI/2, &tmp);
  m4mul(left_wall.transform, tmp, &left_wall.transform);
  scalem4(10,0.01,10, &tmp);
  m4mul(left_wall.transform, tmp, &left_wall.transform);
  left_wall.material = floor.material;
  push(&w, &left_wall);

  Sphere right_wall = sphere();
  transm4(0,0,5, &right_wall.transform);
  rotym4(PI/4, &tmp);
  m4mul(right_wall.transform, tmp, &right_wall.transform);
  rotxm4(PI/2, &tmp);
  m4mul(right_wall.transform, tmp, &right_wall.transform);
  scalem4(10,0.01,10, &tmp);
  m4mul(right_wall.transform, tmp, &right_wall.transform);
  right_wall.material = floor.material;
  push(&w, &right_wall);

  Sphere middle = sphere();
  transm4(-0.5,1,0.5, &middle.transform);
  middle.material.clr = vec4(0.1,1,0.5,1);
  middle.material.diffuse = 0.7;
  middle.material.specular = 0.3;
  push(&w, &middle);

  Sphere right = sphere();
  transm4(1.5,0.6,-0.5, &right.transform);
  scalem4(0.5,0.5,0.5, &tmp);
  m4mul(right.transform, tmp, &right.transform);
  right.material.clr = vec4(0.5, 1, 0.1,1);
  right.material.diffuse = 0.7;
  right.material.specular = 0.3;
  push(&w, &right);

  Sphere left = sphere();
  transm4(-1.5,0.4,-0.75, &left.transform);
  scalem4(0.33,0.33,0.33, &tmp);
  m4mul(left.transform, tmp, &left.transform);
  left.material.clr = vec4(1, 0.8, 0.1,1);
  left.material.diffuse = 0.7;
  left.material.specular = 0.3;
  push(&w, &left);

  Camera cam = camera(1980, 1080, PI/3);
  view_transform(vec4(0,1.5,-5,1), vec4(0,1,0,1), vec4(0,1,0,0), &cam.transform);

  Canvas c = render(cam, w);
  canvas_to_ppm(&c);

  return 0;
}
