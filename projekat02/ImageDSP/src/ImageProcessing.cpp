#include "ImageProcessing.h"
#include "ImageInterpolation.h"

#include <QDebug>

int roundUp(int x)
{
    return (x + 3) & ~3;
}

void imageProcessingFun(const QString& progName, QImage* const outImgs, const QImage* const inImgs, const QVector<double>& params) 
{
	int X_SIZE = inImgs->width();
	int Y_SIZE = inImgs->height();

	/* NOTE: Calculate output image resolution and construct output image object */

	if(progName == "Sample and hold")
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
        /* Horizontal scale factor is params[1] */
        int newXSize = roundUp(X_SIZE * params[1]);
        int newYSize = roundUp(Y_SIZE * params[0]);

        /* Calculate output image resolution and construct output image object */
        new (outImgs) QImage(newXSize, newYSize, inImgs->format());

        /* Perform Sample and hold interpolation  */
        sampleAndHold(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), newXSize, newYSize);

	}
	else if (progName == "Bilinear")
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */
        int newXSize = roundUp(X_SIZE * params[1]);
        int newYSize = roundUp(Y_SIZE * params[0]);

        /* Calculate output image resolution and construct output image object */
        new (outImgs) QImage(newXSize, newYSize, inImgs->format());

        /* Perform Bilinear interpolation  */
        bilinearInterpolate(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), newXSize, newYSize);
	}
    else if (progName == "Bicubic")
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */
        int newXSize = roundUp(X_SIZE * params[1]);
        int newYSize = roundUp(Y_SIZE * params[0]);

        /* Calculate output image resolution and construct output image object */
        new (outImgs) QImage(newXSize, newYSize, inImgs->format());

        /* Perform Bicubic interpolation  */
        bicubicInterpolate(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), newXSize, newYSize);
	}
	else if(progName == "Swirl")
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* k1 factor is params[0]*/
		/* Center of rotation coordinates are (XSIZE/2, YSIZE/2) */
        int m = X_SIZE / 2;
        int n = Y_SIZE / 2;

        /* Construct output image object */
        new (outImgs) QImage(X_SIZE, Y_SIZE, inImgs->format());

        /* Perform image rotation */
        imageSwirl(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), m, n, params[0]);
	}
	else if (progName == "Swirl Bilinear")
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* k1 factor is params[0]*/
		/* Center of rotation coordinates are (XSIZE/2, YSIZE/2) */
        int m = X_SIZE / 2;
        int n = Y_SIZE / 2;

        /* Construct output image object */
        new (outImgs) QImage(X_SIZE, Y_SIZE, inImgs->format());

        /* Perform image rotation with bilinear interpolation */
        imageSwirlBilinear(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), m, n, params[0]);
	}

}

