
#include "ImageProcessing.h"
#include "ColorSpaces.h"
#include "ImageFilter.h"

#include <QDebug>

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
	if(progName == "NF Filter") 
	{	
		/* Perform NF filter */
		performNFFilter(Y_buff, X_SIZE, Y_SIZE);
	}
	else if (progName == "VF Filter") 
	{
		/* Perform VF filter */
		performVFFilter(Y_buff, X_SIZE, Y_SIZE);
	}
	else if(progName == "Successive NF Filter") 
	{	
		/* Perform successive NF filter */
		performSuccessiveNFFilter (Y_buff, X_SIZE, Y_SIZE, params[0]);
	
	}
	else if (progName == "Edge Detection") 
	{
		performSobelEdgeDetection(Y_buff, X_SIZE, Y_SIZE, params[0] * 180);
	}
	else if (progName == "NF Filter + Edge Detection") 
	{
		performNFplusSobelEdgeDetection(Y_buff, X_SIZE, Y_SIZE, params[0], params[1] * 180);
	}

	/* Zero out U and V component. */
	procesing_YUV420(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), 1, 0, 0);

	/* Convert YUV image back to RGB */
	YUV420toRGB(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), outImgs->bits());

	/* Delete used memory buffers */
	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;

}

