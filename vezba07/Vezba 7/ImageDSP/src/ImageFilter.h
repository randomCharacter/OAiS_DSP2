#ifndef DCT_H_
#define DCT_H_
#include <QDebug>
#include <math.h>

void convolve2D (uchar Y_buff[], int xSize, int ySize, double filterCoeff[], int N);

void extendBorders(uchar input[], int xSize, int ySize, uchar output[], int delta);
	
void performNFFilter (uchar input[], int xSize, int ySize);

void performVFFilter (uchar input[], int xSize, int ySize);

void performSuccessiveNFFilter (uchar input[], int xSize, int ySize, int stages);

void performSobelEdgeDetection(uchar input[], int xSize, int ySize, double threshold);

void performNFplusSobelEdgeDetection(uchar input[], int xSize, int ySize, int stages, double threshold);

#endif // DCT_H_
