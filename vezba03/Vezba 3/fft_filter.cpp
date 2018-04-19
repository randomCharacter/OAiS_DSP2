#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sr_fft.h"
#include "obrada.h"

#define PI 3.1415926535
#define BLOCK_SIZE (1<<(FFT_ORDER-1))

short wav_buffer[BLOCK_SIZE];
double window[FFT_SIZE];
double input_buffer[BLOCK_SIZE];
double output_buffer[BLOCK_SIZE];
extern short *fft_offsets_lut;
extern short bitreverse_split_radix[FFT_SIZE];
void init_sine_window(double *window, int len);
void init_vorbis_window(double *window, int len);
void init_kbd_window(double *window, int len);

void split_radix(int off, int size);

typedef struct _wavfileheader
{
  unsigned int chunk_id;
  unsigned int chunk_size;
  unsigned int format;
  unsigned int subchunk1_id;
  unsigned int subchunk1_size;
  unsigned short audio_format;
  unsigned short num_channels;
  unsigned int sample_rate;
  unsigned int byte_rate;
  unsigned short block_align;
  unsigned short bits_per_sample;
  unsigned int data;
  unsigned int length;
} WAV_FILE_HEADER;

void main()
{
  int i, n;
  int num_blocks;
  WAV_FILE_HEADER hdr;
  FILE *inf, *outf;
  
  inf = fopen("somebody.wav", "rb");
  if (inf == NULL) {
    printf("Cant open input file\n");
    return;
  }

  outf = fopen("out.wav", "wb");
  if (outf == NULL) {
    printf("Cant open output file\n");
    return;
  }

  for (i=0; i<(1<<FFT_ORDER); i++)
    bitreverse_split_radix[-split_radix_permutation(i, 1<<FFT_ORDER, 0) & ((1<<FFT_ORDER)-1)] = i;

  fft_offsets_lut = (short*)malloc(((0x2aab >> (16 - 12)) | 1)*sizeof(short));
  n = 0;
  init_fft_offsets_lut(fft_offsets_lut, 0, 1 << 12, &n);

  init_sine_window(window, BLOCK_SIZE);

  fread(&hdr, sizeof(WAV_FILE_HEADER), 1, inf);
  num_blocks = hdr.length/(sizeof(short)*BLOCK_SIZE);
  hdr.length = num_blocks * BLOCK_SIZE * sizeof(short);
  hdr.chunk_size = hdr.length + 36;
  fwrite(&hdr, sizeof(WAV_FILE_HEADER), 1, outf);

  while (fread(wav_buffer, sizeof(short), BLOCK_SIZE, inf) == BLOCK_SIZE) {
    for (i=0; i<BLOCK_SIZE; i++)
      input_buffer[i] = wav_buffer[i];
    obrada(input_buffer, output_buffer, BLOCK_SIZE);
    for (i=0; i<BLOCK_SIZE; i++) {
      if (output_buffer[i] > 32767)
        output_buffer[i] = 32767;
      else if (output_buffer[i] < -32768)
        output_buffer[i] = -32768;
      wav_buffer[i] = (short)(output_buffer[i]);
    }
    fwrite(wav_buffer, sizeof(short), BLOCK_SIZE, outf);
  }

  fclose(inf);
  fclose(outf);
  free(fft_offsets_lut);
}

void init_sine_window(double *window, int len)
{
  int i;

  for (i = 0; i < len; i++) {
    window[i] = sin(0.5 * PI * (i + 0.5) / len);
    window[2*len-1-i] = sin(0.5 * PI * (i + 0.5) / len);
  }
}

void init_vorbis_window(double *window, int len)
{
  int i;
  double t;

  for (i = 0; i < len; i++) {
    t = sin(0.5 * PI * (i + 0.5) / len);
    window[i] = sin(0.5 * PI * t * t);
    window[2*len-1-i] = sin(0.5 * PI * t * t);
  }
}

void init_kbd_window(double *window, int len, double alpha)
{
  int i, j;
  double sum = 0.0, bessel, tmp;
  double local_window[4096];
  double alpha2 = alpha * PI / len;
  alpha2 = alpha2*alpha2;

  for (i = 0; i < len; i++) {
    tmp = i * (len - i) * alpha2;
    bessel = 1.0;
    for (j = 50; j > 0; j--)
      bessel = bessel * tmp / (j * j) + 1;
    sum += bessel;
    local_window[i] = sum;
  }

  sum++;
  for (i = 0; i < len; i++) {
    window[i] = sqrt(local_window[i] / sum);
    window[2*len-1-i] = sqrt(local_window[i] / sum);
  }
}
