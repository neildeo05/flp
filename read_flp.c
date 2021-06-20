#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t frames;
  uint8_t* data;
} FLP;

FLP new_FLP (uint32_t w, uint32_t h, uint32_t f) {
  FLP ret = (FLP) {.width = w, .height = h, .frames = f};
  /* Sample Data:
   * w = 1; h = 3; f = 2
   * FF 00 FF (First/First Frame) 
   * FF 00 FF (Second/First Frame)
   * FF 00 FF (Third/First Frame)
   * 00 FF 00 (First/Second Frame)
   * 00 FF 00 (Second/Second Frame)
   * 00 FF 00 (Third/Third Frame)
   */
  ret.data = malloc(sizeof(uint8_t) * ret.width * (ret.height * ret.frames));
  return ret;
}

int kill(FLP* a) {
  a->width = 0;
  a->height = 0;
  a->frames = 0;
  free(a->data);
  return 1;
}

int main() {
  FLP tmp = new_FLP(3,3,1);
  printf("%d %d %d\n", tmp.width, tmp.height, tmp.frames);
  kill(&tmp);
}
