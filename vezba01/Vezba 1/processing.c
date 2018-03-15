/*
 * processing.c
 *
 *  Created on: 29.02.2016.
 *      Author: Dejan Bokan (dejan.bokan@rt-rk.com)
 */

#include "processing.h"
#include "filter_coeff.h"
#include "fir.h"

Int16 history[N_COEFF];
Int16 buffer[2048];


void clear_history() {
    int i;
    for (i = 0; i < N_COEFF; i++) {
        history[i] = 0;
    }
}

void downsample(Int16 *input, Int16 *output, int M, int N)
{
    int i, j;
    j = 0;
	for (i = 0; i < N; i += M) {
	    output[j++] = input[i];
	}
}

void decimate(Int16 *input, Int16 *output, int M, int N)
{
    clear_history();
    fir_basic_decimate(input, output, N, M, lp_filter, history, N_COEFF);
}

void upsample(Int16 *input, Int16 *output, int L, int N)
{
    int i, j, k;
    j = 0;
    for (i = 0; i < N; i++) {
        output[j++] = input[i];
        for (k = 0; k < L-1; k++) {
            output[j++] = 0;
        }
    }
}

void interpolate(Int16 *input, Int16 *output, int L, int N)
{
	clear_history();
	fir_basic_interpolate(input, output, N, L, lp_filter, history, N_COEFF);
}

void resample(Int16 *input, Int16 *output, int L, int M, int N)
{
    int i;
    interpolate(input, buffer, L, N);
	decimate(buffer, output, M, L * N);
}
