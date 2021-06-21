#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/* Sample Data:
 * w = 1; h = 3; f = 2
 * FF 00 FF (First/First Frame) 
 * FF 00 FF (Second/First Frame)
 * FF 00 FF (Third/First Frame)
 * 00 FF 00 (First/Second Frame)
 * 00 FF 00 (Second/Second Frame)
 * 00 FF 00 (Third/Third Frame)
 */

typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t frames;
  uint32_t channels;
  uint8_t* data;
} FLP;

FLP new_FLP (uint32_t w, uint32_t h, uint32_t f, uint32_t c) {
  FLP ret = (FLP) {.width = w, .height = h, .frames = f, .channels=c};
  ret.data = malloc(sizeof(uint8_t) * ret.width * (ret.height * ret.frames) * ret.channels);
  return ret;
}

int kill(FLP* a) {
  a->width = 0;
  a->height = 0;
  a->frames = 0;
  printf("Killing...");
  free(a->data);
  return 1;
}

//FIXME: Missing one pixel from the loop
FLP load(char* path) {
  int w, h, c;
  uint8_t* jpg = stbi_load(path, &w, &h, &c, 3);
  FLP ret = new_FLP(w, h, 1, c);
  for (int i = 0; i < (w * h * c); i+=3) {
    *(ret.data+i) = jpg[i];
    *(ret.data+i+1) = jpg[i+1];
    *(ret.data+i+2) = jpg[i+2];
  }
  stbi_image_free(jpg);
  return ret;
}

int main() {
  FLP tmp = load("foo.jpg");
  printf("%x\n", *(tmp.data+7155));
  kill(&tmp);
}
