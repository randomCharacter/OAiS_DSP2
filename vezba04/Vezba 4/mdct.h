#define MIN_LOG2_NFFT 2
#define MAX_LOG2_NFFT 12

#define MAX_FFT_SIZE (1 << MAX_LOG2_NFFT)
#define MIN_FFT_SIZE (1 << MIN_LOG2_NFFT)

//#define FFT_ORDER 10
//#define FFT_SIZE (1<<FFT_ORDER)
#define MDCT_ORDER 10
#define MDCT_SIZE (1 << MDCT_ORDER)

#define PI 3.1415926535

int split_radix_permutation(int i, int n, int inverse);
void init_fft_offsets_lut(short *table, int off, int size, int *index);

typedef struct {
  int            N;              // Number of time data points
  double*         twiddle;        // Twiddle factor
  double*         twiddle_scaled; // Twiddle factor (scaled)
  double* fft_in;         // fft workspace, input
  double* fft_out;        // fft workspace, output
} mdct_plan;

void mdct_init(int N);
void mdct_free();
void mdct(double* time_signal, double* mdct_line);
void imdct(double* mdct_line, double* time_signal);
