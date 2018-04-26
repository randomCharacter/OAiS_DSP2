#include <stdlib.h>
#include <math.h>
#include "mdct.h"
#include "fft_tables.h"

short *fft_offsets_lut;
short bitreverse_split_radix[MDCT_SIZE/4];

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
    out[2*bitreverse_split_radix[i]] = in[2*i];
    out[2*bitreverse_split_radix[i]+1] = in[2*i+1];
  }

  num_transforms = (0x2aab >> (16 - order)) | 1;
  for (n=0; n<num_transforms; n++) {
    offset = fft_offsets_lut[n] << 3;
    tmp_re = out + offset;
    tmp_im = tmp_re + 1;

    tmp1 = tmp_re[0] + tmp_re[2];
    tmp5 = tmp_re[4] + tmp_re[6];
    tmp2 = tmp_im[0] + tmp_im[2];
    tmp6 = tmp_im[4] + tmp_im[6];
    tmp3 = tmp_re[0] - tmp_re[2];
    tmp8 = tmp_im[4] - tmp_im[6];
    tmp4 = tmp_im[0] - tmp_im[2];
    tmp7 = tmp_re[4] - tmp_re[6];

    tmp_re[0] = tmp1 + tmp5;
    tmp_re[4] = tmp1 - tmp5;
    tmp_im[0] = tmp2 + tmp6;
    tmp_im[4] = tmp2 - tmp6;
    tmp_re[2] = tmp3 + tmp8;
    tmp_re[6] = tmp3 - tmp8;
    tmp_im[2] = tmp4 - tmp7;
    tmp_im[6] = tmp4 + tmp7;
  }

  num_transforms = (num_transforms >> 1) | 1;
  for (n=0; n<num_transforms; n++)
  {
    offset = fft_offsets_lut[n] << 4;
    tmp_re = out + offset;
    tmp_im = tmp_re + 1;

    tmp1 = tmp_re[ 8] + tmp_re[10];
    tmp3 = tmp_re[12] + tmp_re[14];
    tmp2 = tmp_im[ 8] + tmp_im[10];
    tmp4 = tmp_im[12] + tmp_im[14];
    tmp5 = tmp1 + tmp3;
    tmp7 = tmp1 - tmp3;
    tmp6 = tmp2 + tmp4;
    tmp8 = tmp2 - tmp4;

    tmp1 = tmp_re[ 8] - tmp_re[10];
    tmp2 = tmp_im[ 8] - tmp_im[10];
    tmp3 = tmp_re[12] - tmp_re[14];
    tmp4 = tmp_im[12] - tmp_im[14];

    tmp_re[ 8] = tmp_re[0] - tmp5;
    tmp_re[ 0] = tmp_re[0] + tmp5;
    tmp_im[ 8] = tmp_im[0] - tmp6;
    tmp_im[ 0] = tmp_im[0] + tmp6;
    tmp_re[12] = tmp_re[4] - tmp8;
    tmp_re[ 4] = tmp_re[4] + tmp8;
    tmp_im[12] = tmp_im[4] + tmp7;
    tmp_im[ 4] = tmp_im[4] - tmp7;

    tmp5 = 0.7071067812 * (tmp1 + tmp2);
    tmp7 = 0.7071067812 * (tmp3 - tmp4);
    tmp6 = 0.7071067812 * (tmp2 - tmp1);
    tmp8 = 0.7071067812 * (tmp3 + tmp4);
    tmp1 = tmp5 + tmp7;
    tmp3 = tmp5 - tmp7;
    tmp2 = tmp6 + tmp8;
    tmp4 = tmp6 - tmp8;

    tmp_re[10] = tmp_re[2] - tmp1;
    tmp_re[ 2] = tmp_re[2] + tmp1;
    tmp_im[10] = tmp_im[2] - tmp2;
    tmp_im[ 2] = tmp_im[2] + tmp2;
    tmp_re[14] = tmp_re[6] - tmp4;
    tmp_re[ 6] = tmp_re[6] + tmp4;
    tmp_im[14] = tmp_im[6] + tmp3;
    tmp_im[ 6] = tmp_im[6] - tmp3;
  }

  step = 1 << (MAX_LOG2_NFFT - 4);
  n4 = 8;
  for (nbits=4; nbits<=order; nbits++) {
    n2  = 2*n4;
    n34 = 3*n4;
    num_transforms = (num_transforms >> 1) | 1;
    for (n=0; n<num_transforms; n++) {
      offset = fft_offsets_lut[n] << nbits;
      tmp_re = out + 2*offset;
      tmp_im = tmp_re + 1;

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
      for (i=2; i<n4; i+=2) {
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
}

mdct_plan m_plan;

void mdct_init(int N)
{
  double alpha, omiga, scale;
  int n;

  m_plan.N = N;
  m_plan.twiddle = (double*) malloc(sizeof(double) * N >> 1);
  m_plan.twiddle_scaled = (double*) malloc(sizeof(double) * N >> 1);
  alpha = 2. * PI / (8 * N);
  omiga = 2. * PI / N;
  scale = 2. / N;
  for (n = 0; n < (N >> 2); n++) {
    m_plan.twiddle[2*n+0] = cos(omiga * n + alpha);
    m_plan.twiddle[2*n+1] = sin(omiga * n + alpha);
    m_plan.twiddle_scaled[2*n+0] = (scale * cos(omiga * n + alpha));
    m_plan.twiddle_scaled[2*n+1] = (scale * sin(omiga * n + alpha));
  }

  m_plan.fft_in   = (double*) malloc(sizeof(double) * N >> 1);
  m_plan.fft_out  = (double*) malloc(sizeof(double) * N >> 1);
}

void mdct_free()
{
  free(m_plan.fft_in);
  free(m_plan.fft_out);
  free(m_plan.twiddle);
  free(m_plan.twiddle_scaled);
}

void mdct(double* time_signal, double* mdct_line)
{
  double *xr, *xi, r0, i0;
  double *cos_tw, *sin_tw, c, s;
  int   N4, N2, N34, N54, n;

  N4  = (m_plan.N) >> 2;
  N2  = 2 * N4;
  N34 = 3 * N4;
  N54 = 5 * N4;

  cos_tw = m_plan.twiddle;
  sin_tw = cos_tw + 1;
  
  /* odd/even folding and pre-twiddle */
  xr = m_plan.fft_in;
  xi = xr + 1;
  for (n = 0; n < N4; n += 2) {
    r0 = time_signal[N34-1-n] + time_signal[N34+n];    
    i0 = time_signal[N4+n]    - time_signal[N4-1-n];    

    c = cos_tw[n];
    s = sin_tw[n];

    xr[n] = r0 * c + i0 * s;
    xi[n] = i0 * c - r0 * s;
  }

  for (; n < N2; n += 2) {
    r0 = time_signal[N34-1-n] - time_signal[-N4+n];    
    i0 = time_signal[N4+n]    + time_signal[N54-1-n];    
      
    c = cos_tw[n];
    s = sin_tw[n];

    xr[n] = r0 * c + i0 * s;
    xi[n] = i0 * c - r0 * s;
  }

  /* complex FFT of N/4 long */
  fft(m_plan.fft_in, m_plan.fft_out, MDCT_ORDER-2);

  /* post-twiddle */
  xr = m_plan.fft_out;
  xi = xr + 1;
  for (n = 0; n < N2; n += 2) {
    r0 = xr[n];
    i0 = xi[n];

    c = cos_tw[n];
    s = sin_tw[n];    

    mdct_line[n]      = - r0 * c - i0 * s;
    mdct_line[N2-1-n] = - r0 * s + i0 * c;
  }
}

void imdct(double* mdct_line, double* time_signal)
{
  double *xr, *xi, r0, i0, r1, i1;
  double *cos_tw, *sin_tw, *cos_tw_s, *sin_tw_s, c, s;
  int   N4, N2, N34, N54, n;

  N4  = (m_plan.N) >> 2;
  N2  = 2 * N4;
  N34 = 3 * N4;
  N54 = 5 * N4;

  cos_tw = m_plan.twiddle;
  sin_tw = cos_tw + 1;
  cos_tw_s = m_plan.twiddle_scaled;
  sin_tw_s = cos_tw_s + 1;
  
  /* pre-twiddle */
  xr = m_plan.fft_in;
  xi = xr + 1;
  for (n = 0; n < N2; n += 2) {
    r0 =  mdct_line[n];
    i0 =  mdct_line[N2-1-n];
      
    c = cos_tw[n];
    s = sin_tw[n];    
      
    xr[n] = -2. * (i0 * s + r0 * c);
    xi[n] = -2. * (i0 * c - r0 * s);
  }
  
  /* complex FFT of N/4 long */
  fft(m_plan.fft_in, m_plan.fft_out, MDCT_ORDER-2);

  /* odd/even expanding and post-twiddle */
  xr = m_plan.fft_out;
  xi = xr + 1;
  for (n = 0; n < N4; n += 2) {
    r0 = xr[n];
    i0 = xi[n];    
      
    c = cos_tw_s[n];
    s = sin_tw_s[n];

    r1 = r0 * c + i0 * s;
    i1 = r0 * s - i0 * c;

    time_signal[N34-1-n] =  r1;
    time_signal[N34+n]   =  r1;
    time_signal[N4+n]    =  i1;
    time_signal[N4-1-n]  = -i1;
  }

  for(; n < N2; n += 2) {
    r0 = xr[n];
    i0 = xi[n];    
      
    c = cos_tw_s[n];
    s = sin_tw_s[n];
      
    r1 = r0 * c + i0 * s;
    i1 = r0 * s - i0 * c;
      
    time_signal[N34-1-n] =  r1;
    time_signal[-N4+n]   = -r1;
    time_signal[N4+n]    =  i1;
    time_signal[N54-1-n] =  i1;
  }
}
