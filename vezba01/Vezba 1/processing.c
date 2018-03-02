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
    int i;
    clear_history();
    for (i = 0; i < N; i++) {
        input[i] = fir_basic(input[i], lp_filter, history, N_COEFF);
    }
	downsample(input, output, M, N);
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
    int i;
	upsample(input, output, L, N);
	clear_history();
	for (i = 0; i < N * L; i++) {
        output[i] = fir_basic(output[i], lp_filter, history, N_COEFF);
    }
}

void resample(Int16 *input, Int16 *output, int L, int M, int N)
{
    int i;
    upsample(input, buffer, L, N);
    clear_history();
    for (i = 0; i < L * N; i ++) {
        buffer[i] = fir_basic(buffer[i], lp_filter, history, N_COEFF);
    }
	downsample(buffer, output, M, L * N);
}
