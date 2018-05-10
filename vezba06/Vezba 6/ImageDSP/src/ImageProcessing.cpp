
#include "ImageProcessing.h"
#include "ColorSpaces.h"
#include "DCT.h"

#include <QDebug>

#define N 32

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
	if(progName == "DCT") 
	{	
		/* Perform NxN DCT with block size 8 */
		performDCT(Y_buff, X_SIZE, Y_SIZE, N);
		
	}
	else if (progName == "DCT + IDCT") 
	{
		/* Perform NxN DCT + IDCT with block size 8 */
		performDCTandIDCT(Y_buff, X_SIZE, Y_SIZE, N);
	}
	else if (progName == "Mask DCT") 
	{
		/* Apply BxB mask to DCT coeffs */
		int B = params[0] * N / 100;
		performMaskDCTCoeffs(Y_buff, X_SIZE, Y_SIZE, N, B);
	}
	else if (progName == "DCT Quantization") 
	{
		/* Perform qantization of DCT coeffs using quantization matrix */
		performDCTQuantization(Y_buff, X_SIZE, Y_SIZE);
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

