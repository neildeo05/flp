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
  uint64_t total_size;
  uint8_t* data;
} FLP;

FLP new() {
  FLP ret = {0};
  return ret;
}
FLP new_FLP (uint32_t w, uint32_t h, uint32_t f, uint32_t c) {
  FLP ret = (FLP) {.width = w, .height = h, .frames = f, .channels=c, .total_size = w * h * f * c};
  ret.data = malloc(sizeof(uint8_t) * ret.total_size);
  return ret;
}

int kill(FLP* a) {
  a->width = 0;
  a->height = 0;
  a->frames = 0;
  printf("Killing...\n");
  free(a->data);
  return 1;
}

void load_jpgs(char* path, FLP* flp, int frame_count, int frame_num) {
  int w, h, c;
  uint8_t* jpg = stbi_load(path, &w, &h, &c, 3);
  flp->width = w;
  flp->height = h;
  flp->channels = c;
  flp->frames = frame_count;
  flp->total_size = w * h * c * frame_count;
  flp->data = malloc(sizeof(uint8_t) * flp->total_size);
  memcpy(flp->data + (flp->total_size * frame_num), jpg, (w * h * c)); 
  stbi_image_free(jpg);
}

int main() {
  FLP tmp = new();
  load_jpgs("foo.jpg", &tmp, 1, 0);
  kill(&tmp);



}
