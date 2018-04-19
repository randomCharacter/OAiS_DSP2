#include "sr_fft.h"
#include "fft_tables.h"

short *fft_offsets_lut;
short bitreverse_split_radix[FFT_SIZE];

int split_radix_permutation(int i, int n, int inverse)
{
  int m;
  if(n <= 2) return i&1;
  m = n >> 1;
  if(!(i&m))            return split_radix_permutation(i, m, inverse)*2;
  m >>= 1;
  if(inverse == !(i&m)) return split_radix_permutation(i, m, inverse)*4 + 1;
  else                  return split_radix_permutation(i, m, inverse)*4 - 1;
}

void init_fft_offsets_lut(short *table, int off, int size, int *index)
{
  if (size < 16) {
    table[*index] = off >> 2;
    (*index)++;
  }
  else {
    init_fft_offsets_lut(table, off, size>>1, index);
    init_fft_offsets_lut(table, off+(size>>1), size>>2, index);
    init_fft_offsets_lut(table, off+3*(size>>2), size>>2, index);
  }
}

extern double w_tab_sr[];
double temp_buff[FFT_SIZE];

void fft(double *in, double *out, int order)
{
  int nbits, i, n, num_transforms, offset, step;
  int n4, n2, n34;
  double tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
  double *tmp_re, *tmp_im;
  double w_re, w_im;
  double *w_re_ptr, *w_im_ptr;
  const int fft_size = 1 << order;

  for (i=0; i<fft_size; i++) {
    out[bitreverse_split_radix[i]] = in[i];
    temp_buff[i] = 0;
  }

  num_transforms = (0x2aab >> (16 - order)) | 1;
  for (n=0; n<num_transforms; n++) {
    offset = fft_offsets_lut[n] << 2;
    tmp_re = out + offset;
    tmp_im = temp_buff + offset;

    tmp1 = tmp_re[0] + tmp_re[1];
    tmp5 = tmp_re[2] + tmp_re[3];
    tmp2 = tmp_im[0] + tmp_im[1];
    tmp6 = tmp_im[2] + tmp_im[3];
    tmp3 = tmp_re[0] - tmp_re[1];
    tmp8 = tmp_im[2] - tmp_im[3];
    tmp4 = tmp_im[0] - tmp_im[1];
    tmp7 = tmp_re[2] - tmp_re[3];

    tmp_re[0] = tmp1 + tmp5;
    tmp_re[2] = tmp1 - tmp5;
    tmp_im[0] = tmp2 + tmp6;
    tmp_im[2] = tmp2 - tmp6;
    tmp_re[1] = tmp3 + tmp8;
    tmp_re[3] = tmp3 - tmp8;
    tmp_im[1] = tmp4 - tmp7;
    tmp_im[3] = tmp4 + tmp7;
  }

  num_transforms = (num_transforms >> 1) | 1;
  for (n=0; n<num_transforms; n++)
  {
    offset = fft_offsets_lut[n] << 3;
    tmp_re = out + offset;
    tmp_im = temp_buff + offset;

    tmp1 = tmp_re[4] + tmp_re[5];
    tmp3 = tmp_re[6] + tmp_re[7];
    tmp2 = tmp_im[4] + tmp_im[5];
    tmp4 = tmp_im[6] + tmp_im[7];
    tmp5 = tmp1 + tmp3;
    tmp7 = tmp1 - tmp3;
    tmp6 = tmp2 + tmp4;
    tmp8 = tmp2 - tmp4;

    tmp1 = tmp_re[4] - tmp_re[5];
    tmp2 = tmp_im[4] - tmp_im[5];
    tmp3 = tmp_re[6] - tmp_re[7];
    tmp4 = tmp_im[6] - tmp_im[7];

    tmp_re[4] = tmp_re[0] - tmp5;
    tmp_re[0] = tmp_re[0] + tmp5;
    tmp_im[4] = tmp_im[0] - tmp6;
    tmp_im[0] = tmp_im[0] + tmp6;
    tmp_re[6] = tmp_re[2] - tmp8;
    tmp_re[2] = tmp_re[2] + tmp8;
    tmp_im[6] = tmp_im[2] + tmp7;
    tmp_im[2] = tmp_im[2] - tmp7;

    tmp5 = 0.7071067812 * (tmp1 + tmp2);
    tmp7 = 0.7071067812 * (tmp3 - tmp4);
    tmp6 = 0.7071067812 * (tmp2 - tmp1);
    tmp8 = 0.7071067812 * (tmp3 + tmp4);
    tmp1 = tmp5 + tmp7;
    tmp3 = tmp5 - tmp7;
    tmp2 = tmp6 + tmp8;
    tmp4 = tmp6 - tmp8;

    tmp_re[5] = tmp_re[1] - tmp1;
    tmp_re[1] = tmp_re[1] + tmp1;
    tmp_im[5] = tmp_im[1] - tmp2;
    tmp_im[1] = tmp_im[1] + tmp2;
    tmp_re[7] = tmp_re[3] - tmp4;
    tmp_re[3] = tmp_re[3] + tmp4;
    tmp_im[7] = tmp_im[3] + tmp3;
    tmp_im[3] = tmp_im[3] - tmp3;
  }

  step = 1 << (MAX_LOG2_NFFT - 4);
  n4 = 4;
  for (nbits=4; nbits<=order; nbits++) {
    n2  = 2*n4;
    n34 = 3*n4;
    num_transforms = (num_transforms >> 1) | 1;
    for (n=0; n<num_transforms; n++) {
      offset = fft_offsets_lut[n] << nbits;
      tmp_re = out + offset;
      tmp_im = temp_buff + offset;

      tmp5 = tmp_re[ n2] + tmp_re[n34];
      tmp1 = tmp_re[ n2] - tmp_re[n34];
      tmp6 = tmp_im[ n2] + tmp_im[n34];
      tmp2 = tmp_im[ n2] - tmp_im[n34];

      tmp_re[ n2] = tmp_re[ 0] - tmp5;
      tmp_re[  0] = tmp_re[ 0] + tmp5;
      tmp_im[ n2] = tmp_im[ 0] - tmp6;
      tmp_im[  0] = tmp_im[ 0] + tmp6;
      tmp_re[n34] = tmp_re[n4] - tmp2;
      tmp_re[ n4] = tmp_re[n4] + tmp2;
      tmp_im[n34] = tmp_im[n4] + tmp1;
      tmp_im[ n4] = tmp_im[n4] - tmp1;

      w_re_ptr = w_tab_sr + step;
      w_im_ptr = w_tab_sr + MAX_FFT_SIZE/4 - step;
      for (i=1; i<n4; i++) {
        w_re = w_re_ptr[0];
        w_im = w_im_ptr[0];
        tmp1 = w_re*tmp_re[ n2+i] + w_im*tmp_im[ n2+i];
        tmp2 = w_re*tmp_im[ n2+i] - w_im*tmp_re[ n2+i];
        tmp3 = w_re*tmp_re[n34+i] - w_im*tmp_im[n34+i];
        tmp4 = w_re*tmp_im[n34+i] + w_im*tmp_re[n34+i];

        tmp5 = tmp1 + tmp3;
        tmp1 = tmp1 - tmp3;
        tmp6 = tmp2 + tmp4;
        tmp2 = tmp2 - tmp4;

        tmp_re[ n2+i] = tmp_re[   i] - tmp5;
        tmp_re[    i] = tmp_re[   i] + tmp5;
        tmp_im[ n2+i] = tmp_im[   i] - tmp6;
        tmp_im[    i] = tmp_im[   i] + tmp6;
        tmp_re[n34+i] = tmp_re[n4+i] - tmp2;
        tmp_re[ n4+i] = tmp_re[n4+i] + tmp2;
        tmp_im[n34+i] = tmp_im[n4+i] + tmp1;
        tmp_im[ n4+i] = tmp_im[n4+i] - tmp1;

        w_re_ptr += step;
        w_im_ptr -= step;
      }
    }
    step >>= 1;
    n4   <<= 1;
  }

  tmp1 = out[fft_size/2];
  for (i=fft_size/2-1; i>0; i--) {
    out[2*i] = out[i];
    out[2*i+1] = temp_buff[i];
  }
  out[1] = tmp1;
}

void ifft(double *in, double *out, int order)
{
  int nbits, i, n, num_transforms, offset, step;
  int n4, n2, n34;
  double tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
  double *tmp_re, *tmp_im;
  double w_re, w_im;
  double *w_re_ptr, *w_im_ptr;
  const int fft_size = 1 << order;

  for (i=1; i<fft_size/2; i++) {
    out[bitreverse_split_radix[i]] = in[2*i] / FFT_SIZE;
    temp_buff[bitreverse_split_radix[i]] = in[2*i+1] / FFT_SIZE;
    out[bitreverse_split_radix[fft_size-i]] = in[2*i] / FFT_SIZE;
    temp_buff[bitreverse_split_radix[fft_size-i]] = -in[2*i+1] / FFT_SIZE;
  }
  out[bitreverse_split_radix[0]] = in[0] / FFT_SIZE;
  temp_buff[bitreverse_split_radix[0]] = 0;
  out[bitreverse_split_radix[fft_size/2]] = in[1] / FFT_SIZE;
  temp_buff[bitreverse_split_radix[fft_size/2]] = 0;

  /*for (i=1; i<fft_size/2; i++) {
    out[i] = in[2*i];
    temp_buff[i] = in[2*i+1];
    out[fft_size-i] = in[2*i];
    temp_buff[fft_size-i] = -in[2*i+1];
  }
  out[0] = in[0];
  temp_buff[0] = 0;
  out[fft_size/2] = in[1];
  temp_buff[fft_size/2] = 0;*/

  num_transforms = (0x2aab >> (16 - order)) | 1;
  for (n=0; n<num_transforms; n++)
  {
    offset = fft_offsets_lut[n] << 2;
    tmp_re = out + offset;
    tmp_im = temp_buff + offset;

    tmp1 = tmp_re[0] + tmp_re[1];
    tmp5 = tmp_re[2] + tmp_re[3];
    tmp2 = tmp_im[0] + tmp_im[1];
    tmp6 = tmp_im[2] + tmp_im[3];
    tmp3 = tmp_re[0] - tmp_re[1];
    tmp8 = tmp_im[2] - tmp_im[3];
    tmp4 = tmp_im[0] - tmp_im[1];
    tmp7 = tmp_re[2] - tmp_re[3];

    tmp_re[0] = tmp1 + tmp5;
    tmp_re[2] = tmp1 - tmp5;
    tmp_im[0] = tmp2 + tmp6;
    tmp_im[2] = tmp2 - tmp6;
    tmp_re[1] = tmp3 - tmp8;
    tmp_re[3] = tmp3 + tmp8;
    tmp_im[1] = tmp4 + tmp7;
    tmp_im[3] = tmp4 - tmp7;
  }

  if (fft_size < 8)
    return;

  num_transforms = (num_transforms >> 1) | 1;
  for (n=0; n<num_transforms; n++)
  {
    offset = fft_offsets_lut[n] << 3;
    tmp_re = out + offset;
    tmp_im = temp_buff + offset;

    tmp1 = tmp_re[4] + tmp_re[5];
    tmp3 = tmp_re[6] + tmp_re[7];
    tmp2 = tmp_im[4] + tmp_im[5];
    tmp4 = tmp_im[6] + tmp_im[7];
    tmp5 = tmp1 + tmp3;
    tmp7 = tmp1 - tmp3;
    tmp6 = tmp2 + tmp4;
    tmp8 = tmp2 - tmp4;

    tmp1 = tmp_re[4] - tmp_re[5];
    tmp2 = tmp_im[4] - tmp_im[5];
    tmp3 = tmp_re[6] - tmp_re[7];
    tmp4 = tmp_im[6] - tmp_im[7];

    tmp_re[4] = tmp_re[0] - tmp5;
    tmp_re[0] = tmp_re[0] + tmp5;
    tmp_im[4] = tmp_im[0] - tmp6;
    tmp_im[0] = tmp_im[0] + tmp6;
    tmp_re[6] = tmp_re[2] + tmp8;
    tmp_re[2] = tmp_re[2] - tmp8;
    tmp_im[6] = tmp_im[2] - tmp7;
    tmp_im[2] = tmp_im[2] + tmp7;

    tmp5 = 0.7071067812 * (tmp1 - tmp2);
    tmp7 = 0.7071067812 * (tmp3 + tmp4);
    tmp6 = 0.7071067812 * (tmp1 + tmp2);
    tmp8 = 0.7071067812 * (tmp4 - tmp3);
    tmp1 = tmp5 + tmp7;
    tmp3 = tmp5 - tmp7;
    tmp2 = tmp6 + tmp8;
    tmp4 = tmp6 - tmp8;

    tmp_re[5] = tmp_re[1] - tmp1;
    tmp_re[1] = tmp_re[1] + tmp1;
    tmp_im[5] = tmp_im[1] - tmp2;
    tmp_im[1] = tmp_im[1] + tmp2;
    tmp_re[7] = tmp_re[3] + tmp4;
    tmp_re[3] = tmp_re[3] - tmp4;
    tmp_im[7] = tmp_im[3] - tmp3;
    tmp_im[3] = tmp_im[3] + tmp3;
  }

  step = 1 << (MAX_LOG2_NFFT - 4);
  n4 = 4;
  for (nbits=4; nbits<=order; nbits++)
  {
    n2  = 2*n4;
    n34 = 3*n4;
    num_transforms = (num_transforms >> 1) | 1;
    for (n=0; n<num_transforms; n++)
    {
      offset = fft_offsets_lut[n] << nbits;
      tmp_re = out + offset;
      tmp_im = temp_buff + offset;

      tmp5 = tmp_re[ n2] + tmp_re[n34];
      tmp1 = tmp_re[ n2] - tmp_re[n34];
      tmp6 = tmp_im[ n2] + tmp_im[n34];
      tmp2 = tmp_im[ n2] - tmp_im[n34];

      tmp_re[ n2] = tmp_re[ 0] - tmp5;
      tmp_re[  0] = tmp_re[ 0] + tmp5;
      tmp_im[ n2] = tmp_im[ 0] - tmp6;
      tmp_im[  0] = tmp_im[ 0] + tmp6;
      tmp_re[n34] = tmp_re[n4] + tmp2;
      tmp_re[ n4] = tmp_re[n4] - tmp2;
      tmp_im[n34] = tmp_im[n4] - tmp1;
      tmp_im[ n4] = tmp_im[n4] + tmp1;

      w_re_ptr = w_tab_sr + step;
      w_im_ptr = w_tab_sr + MAX_FFT_SIZE/4 - step;
      for (i=1; i<n4; i++)
      {
        w_re = w_re_ptr[0];
        w_im = w_im_ptr[0];
        tmp1 = w_re*tmp_re[ n2+i] - w_im*tmp_im[ n2+i];
        tmp2 = w_re*tmp_im[ n2+i] + w_im*tmp_re[ n2+i];
        tmp3 = w_re*tmp_re[n34+i] + w_im*tmp_im[n34+i];
        tmp4 = w_re*tmp_im[n34+i] - w_im*tmp_re[n34+i];

        tmp5 = tmp1 + tmp3;
        tmp1 = tmp1 - tmp3;
        tmp6 = tmp2 + tmp4;
        tmp2 = tmp2 - tmp4;

        tmp_re[ n2+i] = tmp_re[   i] - tmp5;
        tmp_re[    i] = tmp_re[   i] + tmp5;
        tmp_im[ n2+i] = tmp_im[   i] - tmp6;
        tmp_im[    i] = tmp_im[   i] + tmp6;
        tmp_re[n34+i] = tmp_re[n4+i] + tmp2;
        tmp_re[ n4+i] = tmp_re[n4+i] - tmp2;
        tmp_im[n34+i] = tmp_im[n4+i] - tmp1;
        tmp_im[ n4+i] = tmp_im[n4+i] + tmp1;

        w_re_ptr += step;
        w_im_ptr -= step;
      }
    }
    step >>= 1;
    n4   <<= 1;
  }
}
