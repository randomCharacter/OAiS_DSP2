#include "ImageFilter.h"


void convolve2D(uchar Y_buff[], int xSize, int ySize, double filterCoeff[], int N)
{
	int delta = (N - 1) / 2;
	uchar *Y_ext = new uchar[(xSize + 2 * delta) * (ySize + 2 * delta)];
	extendBorders(Y_buff, xSize, ySize, Y_ext, delta);

	for (int h = 0; h < xSize; h++)
	{
		for (int v = 0; v < ySize; v++)
		{
			double res = 0;
			for (int k = 0; k < N; k++)
			{
				for (int n = 0; n < N; n++)
				{
					res += filterCoeff[k + n * N] * Y_ext[h + k + (v + n) * (xSize + 2 * delta)];
				}
			}
			Y_buff[h + v * xSize] = abs(res);
		}
	}
	delete Y_ext;
}

void extendBorders(uchar input[], int xSize, int ySize, uchar output[], int delta)
{
	for (int i = 0; i < xSize + delta * 2; i++) {
		for (int d = 0; d < delta; d++)
		{
			output[i + (xSize + 2 * delta) * d] = 0;
			output[i + (ySize - 1 - d) * (xSize + 2 * delta)] = 0;
		}
	}

	for (int i = 0; i < ySize + delta * 2; i++)
	{
		for (int d = 0; d < delta; d++)
		{
			output[d + (xSize + 2 * delta) * i] = 0;
			output[ySize - 1 - d + (xSize + 2 * delta) * i] = 0;
		}
	}
	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			output[(i + delta) + (xSize + 2 * delta) * (j + delta)] = input[i + xSize * j];
		}
	}
}
	
void performNFFilter (uchar input[], int xSize, int ySize)
{
	double filterCoefs[9] = { 1.0 / 9, 1.0 / 9 , 1.0 / 9, 1.0 / 9, 1.0 / 9 , 1.0 / 9, 1.0 / 9, 1.0 / 9 , 1.0 / 9 };
	convolve2D(input, xSize, ySize, filterCoefs, 3);
}

void performVFFilter (uchar input[], int xSize, int ySize)
{
	double filterCoefs[9] = { 0, -1.0 / 4 , 0, -1.0 / 4, 1 , -1.0 / 4, 0, -1.0 / 4 , 0 };
	convolve2D(input, xSize, ySize, filterCoefs, 3);
}

void performSuccessiveNFFilter (uchar input[], int xSize, int ySize, int stages)
{
	double filterCoefs[9] = { 1.0 / 9, 1.0 / 9 , 1.0 / 9, 1.0 / 9, 1.0 / 9 , 1.0 / 9, 1.0 / 9, 1.0 / 9 , 1.0 / 9 };
	for (int i = 0; i < stages; i++)
	{
		convolve2D(input, xSize, ySize, filterCoefs, 3);
	}
}

void performSobelEdgeDetection(uchar input[], int xSize, int ySize, double threshold)
{
	uchar *Y_cpy = new uchar[xSize * ySize];
	memcpy(Y_cpy, input, xSize * ySize);

	double verticalCoefs[9] = { -1.0 / 4, 0, 1.0 / 4, -1.0 / 2, 0, 1.0 / 2, -1.0 / 4, 0, 1.0 / 4, };
	double horizontalCoefs[9] = { -1.0 / 4, -1.0 / 2, -1.0 / 4, 0, 0, 0, 1.0 / 4, 1.0 / 2, 1.0 / 4 };

	convolve2D(input, xSize, ySize, verticalCoefs, 3);
	convolve2D(Y_cpy, xSize, ySize, horizontalCoefs, 3);

	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
            if (sqrtf(pow(input[i + xSize * j], 2) + pow(Y_cpy[i + xSize * j], 2)) >= threshold * 255)
			{
				input[i + xSize * j] = 255;
			}
			else
			{
				input[i + xSize * j] = 0;
			}
		}
	}
}

void performNFplusSobelEdgeDetection(uchar input[], int xSize, int ySize, int stages, double threshold)
{
	performSuccessiveNFFilter(input, xSize, ySize, stages);
	performSobelEdgeDetection(input, xSize, ySize, threshold);
}
