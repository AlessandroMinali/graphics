// bgImg is the background image to be modified.
// fgImg is the foreground image.
// fgOpac is the opacity of the foreground image.
// fgPos is the position of the foreground image in pixels. It can be negative and (0,0) means the top-left pixels of the foreground and background are aligned.
function composite( bgImg, fgImg, fgOpac, fgPos )
{
  x_clip_left   = Math.abs(Math.min(0, fgPos.x));
  x_clip_right  = Math.min(fgImg.width, bgImg.width - fgPos.x);
  y_clip_top    = Math.abs(Math.min(0, fgPos.y));
  y_clip_bottom = Math.min(fgImg.height, bgImg.height - fgPos.y);
  for(let j = y_clip_top; j < y_clip_bottom; ++j) {
    for(let i = x_clip_left; i < x_clip_right; ++i) {
      let fgel = i * 4 + j * fgImg.width * 4;
      let bgel = (i + fgPos.x) * 4 + (j + fgPos.y) * bgImg.width * 4;

      fga = fgImg.data[fgel + 3] / 255 * fgOpac;
      bga = bgImg.data[bgel + 3] / 255;
      a = fga + (1 - fga) * bga;
      for(let k = 0; k < 3; ++k) {
        fgc = fgImg.data[fgel + k];
        bgc = bgImg.data[bgel + k];
        bgImg.data[bgel + k] = (fga * fgc + (1 - fga) * bga * bgc) / a;
      }
    }
  }
}
