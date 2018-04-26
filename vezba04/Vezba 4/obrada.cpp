#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include "obrada.h"
#include "mdct.h"

double time_buffer[MDCT_SIZE];
double mdct_buffer[MDCT_SIZE/2];
double in_delay[MDCT_SIZE/2];
double out_delay[MDCT_SIZE/2];
extern double window[MDCT_SIZE];

#define B 8

int histogram[1 << B];

inline double sgn(double x) {
	if (x == 0)
	{
		return 0;
	}
	else
	{
		return fabs(x) / x;
	}
}

void obrada(double *in, double *out, int N)
{

	// Kodovanje
	for (int i = 0; i < N; i++)
	{
		time_buffer[i] = in_delay[i];
	}

	for (int i = 0; i < N; i++)
	{
		time_buffer[N + i] = in[i];
	}
	
	mdct(time_buffer, mdct_buffer);

	for (int i = 0; i < N; i++) {
		mdct_buffer[i] = sgn(mdct_buffer[i]) * sqrt(fabs(mdct_buffer[i]));
	}

	for (int i = 0; i < N; i++) {
		mdct_buffer[i] = round(mdct_buffer[i] / (1 << (13 - B)));
		histogram[(int)mdct_buffer[i] + (1 << (B-1))]++;
	}


	// Dekodovanje
	for (int i = 0; i < N; i++) {
		mdct_buffer[i] = round(mdct_buffer[i] * (1 << (13 - B)));
	}

	
	for (int i = 0; i < N; i++) {
		mdct_buffer[i] = sgn(mdct_buffer[i]) * mdct_buffer[i] * mdct_buffer[i];
	}

	imdct(mdct_buffer, time_buffer);
	
	for (int i = 0; i < N; ++i)
	{
		out[i] = (time_buffer[i] + out_delay[i]) / 2;
	}

	for (int i = 0; i < MDCT_SIZE / 2; i++) {
		out_delay[i] = time_buffer[MDCT_SIZE / 2 + i];
	}

	for (int i = 0; i < N; i++)
	{
		in_delay[i] = in[i];
	}
}

void statistika()
{
	double p[1 << B];
	double b[1 << B];
	double sum = 0;
	for (int i = 0; i < 1 << B; i++) {
		sum += histogram[i];
	}
	double resBefore = sum * 16;
	for (int i = 0; i < 1 << B; i++) {
		p[i] = histogram[i] / sum;
		if (p[i] != 0)
			b[i] = log2(1 / p[i]);
		else
			b[i] = 0;
	}
	double resAfter = 0;
	for (int i = 0; i < 1 << B; i++) {
		resAfter += b[i] * histogram[i];
	}

	double factor = resBefore / resAfter;

	printf("Potreban broj bita pre kompresije: %.2lf\n", resBefore);
	printf("Potreban broj bita nakon kompresije: %.2lf\n", resAfter);
	printf("Faktor kompresije: %.2lf\n", factor);
}
