#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

typedef struct wav_file_t {
  FILE *fd;
  uint8_t magic_number[4];
  uint32_t size;
  uint8_t header_type[4];
  uint8_t format_marker[4];
  uint32_t format_size;
  uint16_t format_type;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t data_rate; /* Avg bytes per second */
  uint16_t data_block_size; /* Block align */
  uint16_t bits_per_sample;
  uint8_t data_marker[4];
  uint32_t data_size;
  uint16_t num_samples;
  int16_t max;
  int16_t min;
  int8_t error; /* non-zero if this wav_file_t had a parse error */
  int16_t* data;
} wav_file_t;

wav_file_t parse_wav_file(char* filename);
