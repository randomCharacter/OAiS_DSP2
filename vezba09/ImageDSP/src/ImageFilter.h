#ifndef IMAGEFILTER_H_
#define IMAGEFILTER_H_
#include <QDebug>
#include <cmath>

void convolve2D (uchar Y_buff[], int xSize, int ySize, double filterCoeff[], int N);

void extendBorders(uchar input[], int xSize, int ySize, uchar output[], int delta);
	
void performNFFilter (uchar input[], int xSize, int ySize);

void performVFFilter (uchar input[], int xSize, int ySize);

void performSuccessiveNFFilter (uchar input[], int xSize, int ySize, int stages);

void performSobelEdgeDetection(uchar input[], int xSize, int ySize, uchar threshold);

void performNFplusSobelEdgeDetection(uchar input[], int xSize, int ySize, int stages, uchar threshold);

#endif // IMAGEFILTER_H_
