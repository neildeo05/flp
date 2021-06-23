#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define USAGE(stream) \
    fprintf(stream, "Usage: read [options]\n"); \
    fprintf(stream, "-s [filename]           load jpg file into FLP\n"); \
    fprintf(stream, "-h                      print out this help message to stdout\n");


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

//FIXME: This is a duplicate function of new_FLP - refactoring necessary
void create_flp_from_jpg(FLP* flp, int w, int h, int c, int frame_count) {
  flp->width = w;
  flp->height = h;
  flp->channels = c;
  flp->frames = frame_count;
  flp->total_size = w * h * c * frame_count;
  flp->data = malloc(sizeof(uint8_t) * flp->total_size);
}

void load_jpg_into_frame(FLP* flp, uint8_t* jpg, int frame_num, int w, int h, int c) {
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
      //Event loop
      uint8_t* jpg = stbi_load(next_arg(&argc, &argv), &w, &h, &c, 3);
      create_flp_from_jpg(&tmp, w, h, c, num_args); //ONLY CALL 1 time
      load_jpg_into_frame(&tmp, jpg, 0, w, h, c);
      stbi_image_free(jpg);
      int counter = 1;
      while(argc > 0) {
        if (counter == 10) {
          break;
        }
        jpg = stbi_load(next_arg(&argc, &argv), &w, &h, &c, 3);
        load_jpg_into_frame(&tmp, jpg, counter, w, h, c);
        stbi_image_free(jpg);
        counter++;
      }
      char buf[13];
      int half = tmp.total_size / tmp.frames;
      for (int i = 1; i <= num_args; i++) {
        printf("%d\n", half * (i-1));
        snprintf(buf, 13, "frame%03d.jpg", i);
        stbi_write_jpg(buf, w, h, 3, tmp.data+(half*(i-1)), 100);
      }

      kill(&tmp);
    }
    else if (strcmp(*(argv+1), "-h") == 0) {
      USAGE(stdout);
      exit(0);
    }
    else {
      fprintf(stderr, "ERROR: Unkown token '%s''\n", *(argv+1));
      USAGE(stderr);
      exit(1);
    }
  }
  else {
    fprintf(stderr, "Expected source file\n");
    USAGE(stderr);
    exit(1);
  }
  return 0;
}

