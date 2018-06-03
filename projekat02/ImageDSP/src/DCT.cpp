#include "DCT.h"
#include "NxNDCT.h"

/* perform DCT */
void performDCT(uchar Y_buff[], int xSize, int ySize, int N)
{
	/* Create NxN buffer for one input block */
	uchar* inBlock = new uchar[N*N];

	/* Generate DCT kernel */
	double* DCTKernel = new double[N*N];
	GenerateDCTmatrix(DCTKernel, N);

	/* Create buffer for DCT coefficients */
	short* dctCoeffs = new short[N*N];

	/* Extend image borders */
	int xSize2, ySize2;
	uchar* Y_buff2;
	extendBorders(Y_buff, xSize , ySize, N, &Y_buff2, &xSize2, &ySize2);

	for (int y = 0; y < ySize2/N; y++)
	{
		for (int x = 0; x < xSize2/N; x++)
		{	

			/* Fill input block buffer */
			for (int j=0; j<N; j++)
			{
				for (int i=0; i<N; i++)
				{
					inBlock[j*N+i] = Y_buff2[(N*y+j)*(xSize2)+N*x+i];
				}
			}

			/* Invoke DCT */
			DCT(inBlock, dctCoeffs, N, DCTKernel);	

			/* Prepere DCT coefficients for drawing and put them in Y_buff */
			for(int i = 0; i < N*N; i++)
			{
				dctCoeffs[i] = abs(dctCoeffs[i]);
				if(dctCoeffs[i] < 0)
				{
					dctCoeffs[i] = 0;
				}
				else if(dctCoeffs[i] > 255)
				{
					dctCoeffs[i] = 255;
				}

				inBlock[i] = dctCoeffs[i];
			}

			/* Write output values to output image */
			for (int j=0; j<N; j++)
			{
				for (int i=0; i<N; i++)
				{
					Y_buff2[(N*y+j)*(xSize2)+N*x+i] = inBlock[j*N+i];
				}
			}
		}
	}

	cropImage(Y_buff2, xSize2, ySize2, Y_buff, xSize, ySize);

	/* Delete used memory buffers coefficients */
	delete[] dctCoeffs;
	delete[] inBlock;
	delete[] DCTKernel;
	delete[] Y_buff2;
}

/* perform DCT and IDCT */
void performDCTandIDCT(uchar* Y_buff, int xSize, int ySize, int N)
{
	/* Create NxN buffer for one input block */
	uchar* inBlock = new uchar[N*N];

	/* Generate DCT kernel */
	double* DCTKernel = new double[N*N];
	GenerateDCTmatrix(DCTKernel, N);

	/* Create buffer for DCT coefficients */
	short* dctCoeffs = new short[N*N];

	/* Extend image borders */
	int xSize2, ySize2;
	uchar* Y_buff2;
	extendBorders(Y_buff, xSize , ySize, N, &Y_buff2, &xSize2, &ySize2);

	for (int y = 0; y < ySize2/N; y++)
	{
		for (int x = 0; x < xSize2/N; x++)
		{	

			/* Fill input block buffer */
			for (int j=0; j<N; j++)
			{
				for (int i=0; i<N; i++)
				{
					inBlock[j*N+i] = Y_buff2[(N*y+j)*(xSize2)+N*x+i];
				}
			}

			/* Invoke DCT */
			DCT(inBlock, dctCoeffs, N, DCTKernel);	



			/* Invoke IDCT */
			IDCT(dctCoeffs, inBlock, N, DCTKernel);	

			/* Write output values to output image */
			for (int j=0; j<N; j++)
			{
				for (int i=0; i<N; i++)
				{
					Y_buff2[(N*y+j)*(xSize2)+N*x+i] = inBlock[j*N+i];
				}
			}
		}
	}

	cropImage(Y_buff2, xSize2, ySize2, Y_buff, xSize, ySize);

	/* Delete used memory buffers coefficients */
	delete[] dctCoeffs;
	delete[] inBlock;
	delete[] DCTKernel;
	delete[] Y_buff2;
}

/* perform masking of DCT coeffs */
void performMaskDCTCoeffs(uchar input[], int xSize, int ySize, int N, int B)
{
	//TO DO
}

/* perform masking of Y image component */
void performMaskYCoeffs(uchar input[], int xSize, int ySize, int N, int B)
{
	//TO DO
}

/* perform quantization of DCT coeffs using Q matrix*/
void performDCTQuantization(uchar input[], int xSize, int ySize)
{
	//TO DO
}