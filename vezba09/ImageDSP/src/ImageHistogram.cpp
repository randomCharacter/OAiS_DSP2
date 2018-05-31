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

/*******************************************************************************************************************************/
/* Plot image histogram */
/*******************************************************************************************************************************/
QImage createHistogramPlot(int histogram[], int start, int end)
{
     QCustomPlot customPlot = QCustomPlot();
	 
	 int vectorSize = end-start+1;
	 int maxValue = 0;
	 QVector<double> x(vectorSize), y(vectorSize);
	 for (int i=0; i<vectorSize; i++)
	 {
	   x[i] = i + start;  
	   y[i] = histogram[i];
	   maxValue = maxValue>=histogram[i]?maxValue:histogram[i];
	 }
	 
	 // create graph and assign data to it:
	 customPlot.addGraph();
	 customPlot.graph(0)->setData(x, y);
	 // give the axes some labels:
	 customPlot.xAxis->setLabel("x");
	 customPlot.yAxis->setLabel("y");
	 // set axes ranges, so we see all data:
	 customPlot.xAxis->setRange(start, end);
	 customPlot.yAxis->setRange(0, maxValue);
	 return customPlot.toPixmap().toImage();
}


