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

#define USAGE() \
    fprintf(stderr, "Usage: read [options]\n"); \
    fprintf(stderr, "-s [filename]           load jpg file into FLP\n");
#define DISP_FLP(flp, size, fc) \
  for(int i = 0; i < size/fc; i+=3) {\
    printf("%d         %x %x %x\n", i, *(flp + i), *(flp + i + 1), *(flp + i + 2));\
  }\
  printf("=================================\n");\
  for(int i = (size / fc) + 1; i < size; i+=3) {\
    printf("%d         %x %x %x\n", i, *(flp + i), *(flp + i + 1), *(flp + i + 2));\
  }
typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t frames;
  uint32_t channels;
  uint32_t total_size;
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

void create_flp_from_jpg(FLP* flp, int w, int h, int c, int frame_count) {
  flp->width = w;
  flp->height = h;
  flp->channels = c;
  flp->frames = frame_count;
  flp->total_size = w * h * c * frame_count;
  flp->data = malloc(sizeof(uint8_t) * flp->total_size);
}

void load_jpg_into_flp_frame(FLP* flp, uint8_t* jpg, int frame_num, int w, int h, int c) {
  memcpy(flp->data + (((flp->total_size/flp->frames) * frame_num)), jpg, (w * h * c)); 
}

char* next_arg(int* argc, char*** argv) {
  if(argc <= 0) {
    fprintf(stderr, "No arguments provided\n");
    exit(1);
  }
  else {
    *argc -= 1;
    char* arg = **argv;
    *argv += 1;
    return arg;
  }
}

int main(int argc, char** argv) {
  if(argc >= 2) {
    if (strcmp(*(argv+1), "-s") == 0) {
      argc -= 2;
      argv += 2;
      int num_args = argc;
      FLP tmp = new();
      int w, h, c;
      uint8_t* jpg = stbi_load(next_arg(&argc, &argv), &w, &h, &c, 3);
      create_flp_from_jpg(&tmp, w, h, c, 2); //ONLY CALL 1 time

      load_jpg_into_flp_frame(&tmp, jpg, 0, w, h, c);
      load_jpg_into_flp_frame(&tmp, jpg, 1, w, h, c);
      int half = tmp.total_size / tmp.frames;
      stbi_write_jpg("gray.jpg", w, h, 3, tmp.data+half, 100);
      DISP_FLP(tmp.data, tmp.total_size, tmp.frames);
      for(int i = 0; i < tmp.total_size / tmp.frames; i+=3) {
        printf("%d      %x %x %x\n", i, *(tmp.data+half+i), *(tmp.data + half + i + 1), *(tmp.data + half + i + 2)); 
      }


      stbi_image_free(jpg);
      kill(&tmp);
    }
    else {
      fprintf(stderr, "ERROR: Unkown token '%s''\n", *(argv+1));
      USAGE();
      exit(1);
    }
  }
  else {
    fprintf(stderr, "Expected source file\n");
    USAGE();
    exit(1);
  }
}





      /*while(argc > 0) {*/
        /*int current_arg = num_args - argc;*/
        /*printf("NEXT ARG:%s\nNUM ARGS:%d\nCURRENT ARG:%d\n", next_arg(&argc, &argv), num_args, current_arg);*/
        /*load_jpgs(next_arg(&argc, &argv), num_args, num_args - argc);*/
      /*}*/
