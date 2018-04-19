#define MIN_LOG2_NFFT 2
#define MAX_LOG2_NFFT 12

#define MAX_FFT_SIZE (1 << MAX_LOG2_NFFT)
#define MIN_FFT_SIZE (1 << MIN_LOG2_NFFT)

#define FFT_ORDER 10

#define FFT_SIZE (1<<FFT_ORDER)

void fft(double *in, double *out, int order);
void ifft(double *in, double *out, int order);

int split_radix_permutation(int i, int n, int inverse);
void init_fft_offsets_lut(short *table, int off, int size, int *index);
