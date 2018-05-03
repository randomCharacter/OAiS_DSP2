
#include "ImageProcessing.h"
#include "ColorSpaces.h"

#include <QDebug>

void imageProcessingFun(const QString& progName, QImage* const outImgs, const QImage* const inImgs, const QVector<double>& params) 
{
	uchar* Y_buff;
	char* U_buff;
	char* V_buff;

	*outImgs = *(new QImage(inImgs->width(), inImgs->height(), inImgs->format()));

	if(progName == "YUV444")
	{
		Y_buff = new uchar[inImgs->width()*inImgs->height()];
		U_buff = new char[inImgs->width()*inImgs->height()];
		V_buff = new char[inImgs->width()*inImgs->height()];

		RGBtoYUV444(inImgs->bits(), inImgs->width(), inImgs->height(), Y_buff, U_buff, V_buff);

		decimate_Y(Y_buff, inImgs->width(), inImgs->height());

		procesing_YUV444(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), params[0], params[1], params[2]);
		YUV444toRGB(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), outImgs->bits());

		delete[] Y_buff;
		delete[] U_buff;
		delete[] V_buff;

	}
	else if (progName == "YUV422")
	{
		Y_buff = new uchar[inImgs->width()*inImgs->height()];
		U_buff = new char[inImgs->width()*inImgs->height()/2];
		V_buff = new char[inImgs->width()*inImgs->height()/2];

		RGBtoYUV422(inImgs->bits(), inImgs->width(), inImgs->height(), Y_buff, U_buff, V_buff);
		procesing_YUV422(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), params[0], params[1], params[2]);
		YUV422toRGB(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), outImgs->bits());

		delete[] Y_buff;
		delete[] U_buff;
		delete[] V_buff;
	}
	else if (progName == "YUV420")
	{
		Y_buff = new uchar[inImgs->width()*inImgs->height()];
		U_buff = new char[inImgs->width()*inImgs->height()/4];
		V_buff = new char[inImgs->width()*inImgs->height()/4];

		RGBtoYUV420(inImgs->bits(), inImgs->width(), inImgs->height(), Y_buff, U_buff, V_buff);
		procesing_YUV420(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), params[0], params[1], params[2]);
		YUV420toRGB(Y_buff, U_buff, V_buff, inImgs->width(), inImgs->height(), outImgs->bits());

		delete[] Y_buff;
		delete[] U_buff;
		delete[] V_buff;
	}
	else if (progName == "RGB")
	{
		processing_RGB(inImgs->bits(), inImgs->width(), inImgs->height(), outImgs->bits(), params[0], params[1], params[2]);
	}

}

