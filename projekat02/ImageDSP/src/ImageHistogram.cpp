#include "ImageHistogram.h"
#include "qcustomplot.h"


/*******************************************************************************************************************************/
/* Simple contrast improvement */
/*******************************************************************************************************************************/
void simpleContrastImprovement (const uchar input[], int xSize, int ySize, uchar output[])
{
	int maxValue = 0, minValue = 255;
	
	for(int j = 0; j < ySize; j++ )
	{
		for(int i = 0; i < xSize; i++ )
		{
			minValue = minValue<=input[j*xSize+i]?minValue:input[j*xSize+i];
			maxValue = maxValue>=input[j*xSize+i]?maxValue:input[j*xSize+i];
		}
	}
	  
	for(int j = 0; j < ySize; j++ )
	{
		for(int i = 0; i < xSize; i++ )
		{
			output[j*xSize+i] = (input[j*xSize+i] - minValue) * 255 / (maxValue - minValue);
		}
	}
}


/*******************************************************************************************************************************/
/* Calculate image histogram */
/*******************************************************************************************************************************/
void calculateHistogram(const uchar input[], int xSize, int ySize, int histogram[])
{
	for(int j = 0; j < ySize; j++ )
	{
		for(int i = 0; i < xSize; i++ )
		{
			histogram[input[j*xSize+i]]++;
		}
	}
}


/*******************************************************************************************************************************/
/* Equalize image histogram */
/*******************************************************************************************************************************/
void equalizeHistogram(const uchar input[], int xSize, int ySize, uchar output[])
{
	
}

/*******************************************************************************************************************************/
/* Modify colour saturation */
/*******************************************************************************************************************************/
void modifySaturation(const uchar inputRGB[], const uchar inputY[], int xSize, int ySize, uchar outputRGB[], double S)
{
	for(int j = 0; j < ySize; j++ )
	{
		for(int i = 0; i < xSize; i++ )
		{
			outputRGB[j*xSize*3 + i*3] = inputRGB[j*xSize*3 + i*3]*S + inputY[j*xSize+i] * (1-S);
			outputRGB[j*xSize*3 + i*3+1] = inputRGB[j*xSize*3 + i*3+1]*S + inputY[j*xSize+i] * (1-S);
			outputRGB[j*xSize*3 + i*3+2] = inputRGB[j*xSize*3 + i*3+2]*S + inputY[j*xSize+i] * (1-S);
		}
	}
}

