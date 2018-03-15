/*
 * processing.c
 *
 *  Created on: 29.02.2016.
 *      Author: Dejan Bokan (dejan.bokan@rt-rk.com)
 */

#include "processing.h"
#include "filter_coeff.h"
#include "fir.h"

Int16 buffer[2048];

void downsample(Int16 *input, Int16 *output, int M, int N)
{
    int i, j;
    j = 0;
	for (i = 0; i < N; i += M) {
	    output[j++] = input[i];
	}
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
