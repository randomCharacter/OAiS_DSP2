
#include "ImageProcessing.h"
#include "ColorSpaces.h"
#include "imageHistogram.h"

#include <QDebug>

void imageProcessingFun(const QString& progName, QImage* const outImgs, const QImage* const inImgs, const QVector<double>& params) 
{
	/* Create buffers for YUV image */
	uchar* Y_buff = new uchar[inImgs->width()*inImgs->height()];
	char* U_buff = new char[inImgs->width()*inImgs->height()/4];
	char* V_buff = new char[inImgs->width()*inImgs->height()/4];

	int X_SIZE = inImgs->width();
	int Y_SIZE = inImgs->height();

	/* Convert input image to YUV420 image */
	RGBtoYUV420(inImgs->bits(), X_SIZE, Y_SIZE, Y_buff, U_buff, V_buff);

	/* Processing is done only on Y component (grayscale image) */
	if(progName == "Improve Contrast") 
	{	
		simpleContrastImprovement(Y_buff, X_SIZE, Y_SIZE, Y_buff);

		/* Create empty output image */
		*outImgs = *(new QImage(X_SIZE, Y_SIZE, inImgs->format()));

		/* Convert YUV image back to RGB */
		YUV420toRGB(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), outImgs->bits());
	}
	else if(progName == "Histogram") 
	{	
		int* histogram = new int[256];
		for(int i = 0; i<255; i++)
			histogram[i] = 0;

		calculateHistogram(Y_buff, X_SIZE, Y_SIZE, histogram);

		*outImgs = createHistogramPlot(histogram, 0, 255);
		
		delete[] histogram;
	}
	else if(progName == "ImproveContrast (Histogram)") 
	{	
		int* histogram = new int[256];
		for(int i = 0; i<255; i++)
			histogram[i] = 0;
		
		simpleContrastImprovement(Y_buff, X_SIZE, Y_SIZE, Y_buff);

		calculateHistogram(Y_buff, X_SIZE, Y_SIZE, histogram);

		*outImgs = createHistogramPlot(histogram, 0, 255);
		
		delete[] histogram;
	}
	else if(progName == "Equalize histogram") 
	{	
		equalizeHistogram(Y_buff, X_SIZE, Y_SIZE, Y_buff);

		/* Create empty output image */
		*outImgs = *(new QImage(X_SIZE, Y_SIZE, inImgs->format()));

		/* Convert YUV image back to RGB */
		YUV420toRGB(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), outImgs->bits());
		
	}
	else if(progName == "Equalize histogram (Histogram)") 
	{	
		int* histogram = new int[256];
		for(int i = 0; i<255; i++)
			histogram[i] = 0;
		
		equalizeHistogram(Y_buff, X_SIZE, Y_SIZE, Y_buff);

		calculateHistogram(Y_buff, X_SIZE, Y_SIZE, histogram);

		*outImgs = createHistogramPlot(histogram, 0, 255);
		
		delete[] histogram;
	}
	else if(progName == "Color Saturation") 
	{	
		double S = params[0];
		
		/* Create empty output image */
		*outImgs = *(new QImage(X_SIZE, Y_SIZE, inImgs->format()));

		equalizeHistogram(Y_buff, X_SIZE, Y_SIZE, Y_buff);
		YUV420toRGB(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), outImgs->bits());

		modifySaturation(outImgs->bits(), Y_buff, X_SIZE, Y_SIZE, outImgs->bits(), S);
	}


	/* Delete used memory buffers */

	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;

}

