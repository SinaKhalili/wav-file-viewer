#include"wav_reader.h"

int compare_bytes(uint8_t *s1, uint8_t *s2, size_t size) {
  for (int i = 0; i < size; i++) {
    if (s1[i] != s2[i])
      return 0;
  }
  return 1;
}

wav_file_t parse_wav_file(char* filename){
  wav_file_t wav_file;
  wav_file.error = 1; /*  Sets error, if parse makes it to the end, set error to 0 */
  FILE *fd = fopen(filename, "r");
  if(!fd){
    printf("Error: Invalid file\n");
    return wav_file;
  }
  wav_file.fd = fd;

  /*  Begin reading the file  */
  size_t res = fread(wav_file.magic_number, 4, 1, wav_file.fd);
  if (!res)
    return wav_file;

  /*  Check the magic number */
  uint8_t riff_header[4] = {'R', 'I', 'F', 'F'};
  if (!compare_bytes(riff_header, wav_file.magic_number, 4)){
    printf("Error: Invalid file type\n");
    return wav_file;
  }

  /*  Get the size of the file - 32 bits */
  res = fread(&wav_file.size, 4, 1, wav_file.fd);
  if (!res)
    return wav_file;

  printf("File size is %d bytes\n", wav_file.size);

  /*  Get the header type of the file */
  res = fread(wav_file.header_type, 4, 1, wav_file.fd);
  if (!res)
    return wav_file;

  /*  Check the header type */
  uint8_t header_type[4] = {'W', 'A', 'V', 'E'};
  if (!compare_bytes(header_type, wav_file.header_type, 4)){
    printf("Error: Invalid header type\n");
    return wav_file;
  }

  /*  Beginning of the data format information */
  res = fread(wav_file.format_marker, 4, 1, wav_file.fd);
  if (!res)
    return wav_file;

  /*  Check for the fmt marker */
  uint8_t format_marker[4] = {'f', 'm', 't', ' '};
  if (!compare_bytes(format_marker, wav_file.format_marker, 4)){
    printf("Error: No format marker\n");
    return wav_file;
  }

  /*  Get the size of the metadata - 32 bits */
  res = fread(&wav_file.format_size, 4, 1, wav_file.fd);
  if (!res)
    return wav_file;
  printf("Format data size is %d bytes\n", wav_file.format_size);

  /*  Get the type of format - 16 bits */
  res = fread(&wav_file.format_type, 2, 1, wav_file.fd);
  if (!res)
    return wav_file;
  printf("Format type is %d (1 is PCM not compressed)\n", wav_file.format_type);

  /*  Get the number of channels - 16 bits */
  res = fread(&wav_file.num_channels, 2, 1, wav_file.fd);
  if (!res)
    return wav_file;
  printf("Number of channels: %d \n", wav_file.num_channels);

  /*  Get the sample rate - 32 bits */
  res = fread(&wav_file.sample_rate, 4, 1, wav_file.fd);
  if (!res)
    return wav_file;
  printf("Sample rate: %d samples per second \n", wav_file.sample_rate);

  /*  Get the average bytes per second - 32 bits (Sample Rate * BitsPerSample * Channels)/8 */
  res = fread(&wav_file.data_rate, 4, 1, wav_file.fd);
  if (!res)
    return wav_file;
  printf("Data rate: %d average bytes per second \n", wav_file.data_rate);

  /*  Get the channel type - 16 bits (BitsPerSample * Channels)/8 */
  res = fread(&wav_file.data_block_size, 2, 1, wav_file.fd);
  if (!res)
    return wav_file;
  printf("Block align: %d bytes\n", wav_file.data_block_size);

  /*  Get the bits per sample */
  res = fread(&wav_file.bits_per_sample, 2, 1, wav_file.fd);
  if (!res)
    return wav_file;
  printf("Bits per sample: %d\n", wav_file.bits_per_sample);

  /*  Beginning of the data marker */
  res = fread(wav_file.data_marker, 4, 1, wav_file.fd);
  if (!res)
    return wav_file;

  /*  Get the data marker */
  uint8_t data_marker[4] = {'d', 'a', 't', 'a'};
  if (!compare_bytes(data_marker, wav_file.data_marker, 4)){
    printf("Error: No data marker\n");
    return wav_file;
  }

  /*  Get the size of the data */
  res = fread(&wav_file.data_size, 4, 1, wav_file.fd);
  if (!res)
    return wav_file;
  printf("Data size: %d bytes (uint32)\n", wav_file.data_size);

  /* Store the number of samples (computed value) */
  wav_file.num_samples = (wav_file.data_size*8) / wav_file.bits_per_sample;
  printf("Number of samples is: %d\n", wav_file.num_samples);

  /* Malloc some data and copy it in */
  wav_file.data = malloc( (wav_file.bits_per_sample/8) * wav_file.num_samples);
  res = fread(wav_file.data, wav_file.bits_per_sample/8, wav_file.num_samples, wav_file.fd);

  if (!res)
    return wav_file;
  if(feof(wav_file.fd))
    printf("We've reached the end of the file\n");

  wav_file.max = -32768;
  wav_file.min = 32767;

  for(int i = 0; i < wav_file.num_samples; i++){
    if(wav_file.data[i] > wav_file.max)
      wav_file.max = wav_file.data[i];
    if(wav_file.data[i] < wav_file.min)
      wav_file.min = wav_file.data[i];
  }

  printf("The max sample is: %d\n", wav_file.max);
  printf("The min sample is: %d\n", wav_file.min);

  wav_file.error = 0; /* Successful parse */
  return wav_file;
}
