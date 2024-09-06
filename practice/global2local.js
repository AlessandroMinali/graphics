/* HELPER FUNCTIONS */
// sub two vectors
function vsub(a, b) {
  return {x: a.x - b.x, y: a.y - b.y};
}
// dot product of two vectors
function vdot(a, b) {
  return (a.x * b.x) + (a.y * b.y);
}
// magnitude of a vector
function vmag(a) {
  return Math.sqrt(a.x * a.x + a.y * a.y);
}
// normalize a vector
function vnorm(a) {
  const mag = vmag(a);
  return {x: a.x / mag, y: a.y / mag};
}
/* END HELPER FUNCTIONS */

// This function should convert from global space to local. When correctly
// implemented a blue vector line from global origin will replicate the
// position of the cursor relative to the local frame.
//
// PARAMS
// globalCoord: { x: float, y: float }
// localOrigin: { x: float, y: float }
// localAngle: float
// RETURN
// { x: float, y: float }
//
function localFromGlobal(globalCoord, localOrigin, localAngle) {
  let u = {x: Math.cos(localAngle), y: -Math.sin(localAngle)}
  let v = {x: Math.sin(localAngle), y:  Math.cos(localAngle)}
  return {
    x: vdot(vsub(globalCoord, localOrigin), vnorm(u)),
    y: vdot(vsub(globalCoord, localOrigin), vnorm(v))
  };
}
