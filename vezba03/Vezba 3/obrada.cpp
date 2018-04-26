#define _USE_MATH_DEFINES
#include <math.h>
#include "obrada.h"
#include "sr_fft.h"

#define LOW 300
#define HIGH 4000
#define FS 44100

double time_buffer[FFT_SIZE];
double fft_buffer[FFT_SIZE];
double in_delay[FFT_SIZE / 2];
double out_delay[FFT_SIZE / 2];

extern double window[FFT_SIZE];

void obrada(double *in, double *out, int N)
{
    int k1 = LOW * FFT_SIZE / FS;
    int k2 = HIGH * FFT_SIZE / FS;

    for (int i = 0; i < N; i++)
    {
        time_buffer[i] = in_delay[i];
    }

    for (int i = 0; i < N; i++)
    {
        time_buffer[N + i] = in[i];
    }


    // Prozoriranje
    for (int i = 0; i < 2*N; ++i)
    {
        time_buffer[i] *= window[i];
    }

    fft(time_buffer, fft_buffer, FFT_ORDER);

    // Filtriranje
    for (int i = 0; i < 2 * k1; ++i) {
        fft_buffer[i] = 0;
    }
    for (int i = 2 * k2; i < 2 * N; ++i) {
        fft_buffer[i] = 0;
    }

    // Promena faze koeficijenata
    for (int i = 0; i < N; ++i) {
        fft_buffer[2*i] = sqrt(pow(fft_buffer[2*i], 2) + pow(fft_buffer[2*i + 1], 2))*cos(M_PI*i*i*0.25);
        fft_buffer[2*i + 1] = sqrt(pow(fft_buffer[2*i], 2) + pow(fft_buffer[2*i + 1], 2))*sin(M_PI*i*i*0.25);
    }

    ifft(fft_buffer, time_buffer, FFT_ORDER);

    // Prozoriranje
    for (int i = 0; i < 2*N; ++i)
    {
        time_buffer[i] *= window[i];
    }

    for (int i = 0; i < N; ++i)
    {
        out[i] = time_buffer[i] + out_delay[i];
    }

    for (int i = 0; i < FFT_SIZE / 2; i++) {
        out_delay[i] = time_buffer[FFT_SIZE / 2 + i];
    }

    for (int i = 0; i < N; i++)
    {
        in_delay[i] = in[i];
    }

}
