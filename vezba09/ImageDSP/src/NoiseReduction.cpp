#include "NoiseReduction.h"
#include "ImageFilter.h"
	
void performMovingAverage (uchar input[], int xSize, int ySize)
{
	double filter[25] = { 1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25,
		1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25, 
		1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25, 
		1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25, 
		1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25, 1.0 / 25 };
	convolve2D(input, xSize, ySize, filter, 5);
}

void calculateGaussKernel(double kernel[], int N, double sigma)
{
	double C = 0;
	for (int n = 0; n < N; n++)
	{
		for (int k = 0; k < N; k++)
		{
			kernel[n*N + k] = exp(-((n - N / 2)*(n - N / 2) + (k - N / 2)*(k - N / 2))
				/ (2 * sigma * sigma));
			C += kernel[n*N + k];
		}
	}
	C = 1.0 / C;
	for (int n = 0; n < N; n++)
	{
		for (int k = 0; k < N; k++)
		{
			kernel[n*N + k] *= C;
		}
	}
}

void performGaussFilter (uchar input[], int xSize, int ySize, double sigma)
{
	double kernel[25] = { 0 };
	calculateGaussKernel(kernel, 5, sigma);

	convolve2D(input, xSize, ySize, kernel, 5);
}

void performMedianFilter (uchar input[], int xSize, int ySize, int SIZE)
{
	uchar* extended = new uchar[(xSize + SIZE - 1) * (ySize + SIZE - 1)];
	extendBorders(input, xSize, ySize, extended, SIZE / 2);

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			int* med = new int[SIZE * SIZE];
			
			for (int k = 0; k < SIZE; k++) {
				for (int m = 0; m < SIZE; m++) {
					med[k + SIZE * m] = extended[(i + k) + (j + m) * (xSize + SIZE - 1)];
				}
			}
			
			for (int k = 0; k < SIZE * SIZE - 1; k++) {
				for (int m = k; m < SIZE * SIZE; m++) {
					if (med[k] > med[m]) {
						int t = med[k];
						med[k] = med[m];
						med[m] = t;
					}
				}
			}
			input[i + j * xSize] = med[SIZE * SIZE / 2];
			delete med;
		}
	}

	delete extended;
}
