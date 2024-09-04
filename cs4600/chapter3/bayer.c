#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// take a ppm photo, separate color channels, and form bayer mosaic

int main(int argc, char* argv[]) {
  if (argc == 1) { printf("Please specify ppm image.\n"); return 1; };

  size_t bufsz = 64;
  char buf[bufsz];
  int width  = 0;
  int height = 0;

  FILE* f = fopen(argv[1], "r");
  fgets(buf, bufsz, f); // magic number
  fgets(buf, bufsz, f); // width height
  for(size_t i = 0; i < bufsz; ++i) {
    if (buf[i] == ' ') {
      buf[i] = '\0';
      width  = strtol(buf, 0, 10);
      height = strtol(&buf[i+1], 0, 10);
      break;
    }
  }
  fgets(buf, bufsz, f); // max value
  int max = strtol(buf, 0, 10);

  // separate channels
  unsigned char* red   = calloc(height * width, sizeof(unsigned char));
  unsigned char* green = calloc(height * width, sizeof(unsigned char));
  unsigned char* blue  = calloc(height * width, sizeof(unsigned char));
  unsigned char pixel[3] = {0};
  unsigned char offset;
  unsigned char color; // red: 0, green: 1, blue: 2
  for(int j = 0; j < height; ++j) {
    color = !((j + 1) % 2 == 0); // odd lines color toggle
    offset = 0;
    for(int i = 0; i < width; ++i) {
      fread(&pixel, sizeof(char), 3, f);
      switch(color + offset) {
        case(0): {
          red[i + j * width] = pixel[0];
          break;
        }
        case(1): {
          green[i + j * width + 1] = pixel[1];
          break;
        }
        case(2): {
          blue[i + j * width + 2] = pixel[2];
          break;
        }
      }
      offset ^= 0x1;
    }
  }
  fclose(f);

  char header[64] = {0};
  sprintf(header, "P5\n%u %u\n%u\n", width, height, max);
  FILE* fr = fopen("red.pgm", "wb");
  fwrite(&header, strlen(header), 1, fr);
  fwrite(red, 1, height * width, fr);
  fclose(fr);
  FILE* fg = fopen("green.pgm", "wb");
  fwrite(&header, strlen(header), 1, fg);
  fwrite(green, 1, height * width, fg);
  fclose(fg);
  FILE* fb = fopen("blue.pgm", "wb");
  fwrite(&header, strlen(header), 1, fb);
  fwrite(blue, 1, height * width, fb);
  fclose(fb);

  // recombine channels
  FILE* finale = fopen("mosaic.ppm", "wb");
  sprintf(header, "P6\n%u %u\n%u\n", width, height, max);
  fwrite (&header, strlen(header), 1, finale);
  for(int j = 0; j < height; ++j) {
    color = !((j + 1) % 2 == 0);
    offset = 0;
    for(int i = 0; i < width; ++i) {
      pixel[0] = 0; pixel[1] = 0; pixel[2] = 0;
      switch(color + offset) {
        case(0): {
          pixel[0] = red[i + j * width];
          break;
        }
        case(1): {
          pixel[1] = green[i + j * width + 1];
          break;
        }
        case(2): {
          pixel[2] = blue[i + j * width + 2];
          break;
        }
      }
      fwrite(&pixel, sizeof(char), 3, finale);
      offset ^= 0x1;
    }
  }

  return 0;
}
