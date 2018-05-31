#include "ImageFilter.h"

void convolve2D (uchar Y_buff[], int xSize, int ySize, double filterCoeff[], int N)
{
	uchar* extended_Y = new uchar[(xSize+N-1) * (ySize+N-1)];

	extendBorders(Y_buff, xSize, ySize, extended_Y, N/2);

	for(int j = 0; j < ySize; j++)
	{
		for(int i = 0; i < xSize; i++)
		{
			double accum  = 0;
			
			for(int m = 0; m <N; m++)
			{
				for(int k = 0; k <N; k++)
				{
					accum += extended_Y[(j+k) * (xSize+N-1) + (i+m)] * filterCoeff[(N-k-1)*N+(N-m-1)];
				}
			}

			if(accum < 0)
				accum = 0;
			else if(accum > 255)
				accum = 255;

			Y_buff[j * xSize + i] = accum;
		
		}
	}

	delete extended_Y;
}

void extendBorders(uchar input[], int xSize, int ySize, uchar output[], int delta)
{
	int i;
	
	for (i=0; i<delta; i++)
	{
		memcpy(&output[i*(xSize+2*delta)+delta], &input[0], xSize);
		memcpy(&output[(ySize+delta+i)*(xSize+2*delta)+delta], &input[(ySize-1)*(xSize)], xSize);
	}
	
	for (i=0; i<ySize; i++)
	{
		memcpy(&output[(i+delta)*(xSize+2*delta)+delta], &input[i*(xSize)], xSize); 
	}
	
	for (i=0; i<ySize+2*delta; i++)
	{
		memset(&output[i*(xSize+2*delta)], output[i*(xSize+2*delta)+delta], delta);
		memset(&output[i*(xSize+2*delta)+xSize+delta], output[i*(xSize+2*delta)+xSize+delta-1], delta);
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

void performSobelEdgeDetection(uchar input[], int xSize, int ySize, uchar threshold)
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

void performNFplusSobelEdgeDetection(uchar input[], int xSize, int ySize, int stages, uchar threshold)
{
	performSuccessiveNFFilter(input, xSize, ySize, stages);
	performSobelEdgeDetection(input, xSize, ySize, threshold);
}
