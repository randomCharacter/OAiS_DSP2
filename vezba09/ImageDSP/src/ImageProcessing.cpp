
#include "ImageProcessing.h"
#include "ColorSpaces.h"
#include "ImageFilter.h"
#include "NoiseReduction.h"
#include <QDebug>

#define MEDIAN_SIZE 7

void imageProcessingFun(const QString& progName, QImage* const outImgs, const QImage* const inImgs, const QVector<double>& params) 
{
	/* Create buffers for YUV image */
	uchar* Y_buff = new uchar[inImgs->width()*inImgs->height()];
	char* U_buff = new char[inImgs->width()*inImgs->height()/4];
	char* V_buff = new char[inImgs->width()*inImgs->height()/4];

	int X_SIZE = inImgs->width();
	int Y_SIZE = inImgs->height();

	/* Create empty output image */
	*outImgs = *(new QImage(X_SIZE, Y_SIZE, inImgs->format()));

	/* Convert input image to YUV420 image */
	RGBtoYUV420(inImgs->bits(), X_SIZE, Y_SIZE, Y_buff, U_buff, V_buff);

	/* Processing is done only on Y component (grayscale image) */
	if(progName == "Moving Average") 
	{	
		/* Perform Moving Average */
		performMovingAverage(Y_buff, X_SIZE, Y_SIZE);
	}
	else if (progName == "Gaussian") 
	{
		/* Perform Gaussian filter */
		performGaussFilter(Y_buff, X_SIZE, Y_SIZE, params[0]);
	}
	else if(progName == "Median") 
	{	
		/* Perform Median filter */
		performMedianFilter(Y_buff, X_SIZE, Y_SIZE, MEDIAN_SIZE);
	}

	/* Zero out U and V component. */
	procesing_YUV420(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), 1, 1, 1);

	/* Convert YUV image back to RGB */
	YUV420toRGB(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), outImgs->bits());

	/* Delete used memory buffers */
	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;

}

