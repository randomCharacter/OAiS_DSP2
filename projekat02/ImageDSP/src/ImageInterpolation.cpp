#include "ImageInterpolation.h"
#include <math.h>


void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
    double scaleX = ((double) xSize) / newXSize;
    double scaleY = ((double) ySize) / newYSize;
    for (int i = 0; i < newXSize; i++)
    {
        for (int j = 0; j < newYSize; j++)
        {
            int newI = floor((i - 1) * scaleX + 1);
            int newJ = floor((j - 1) * scaleY + 1);
            if (newI >= xSize) {
                newI = xSize - 1;
            }
            if (newJ >= ySize) {
                newJ = ySize - 1;
            }
            output[3 * i + j * newXSize * 3] = input[3 * newI + newJ * xSize * 3];
            output[3 * i + 1 + j * newXSize * 3] = input[3 * newI + 1 + newJ * xSize * 3];
            output[3 * i + 2 + j * newXSize * 3] = input[3 * newI + 2 + newJ * xSize * 3];
        }
    }
}

void bilinearInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
    double scaleX = ((double) xSize) / newXSize;
    double scaleY = ((double) ySize) / newYSize;
    for (int i = 0; i < newXSize; i++)
    {
        for (int j = 0; j < newYSize; j++)
        {
            int newI = (i) * scaleX;
            int newJ = (j) * scaleY;
            double a = j * scaleY - floor(j * scaleY);
            double b = i * scaleX - floor(i * scaleX);

            output[3 * i + j * newXSize * 3] =
                    (1 - a) * (1 - b) * input[3 * newI + newJ * xSize * 3] +
                    (1 - a) * b * input[3 * (newI + 1) + newJ * xSize * 3] +
                    a * (1 - b) * input[3 * newI + (newJ + 1) * xSize * 3] +
                    a * b * input[3 * (newI + 1) + (newJ + 1) * xSize * 3];
            output[3 * i + 1 + j * newXSize * 3] =
                    (1 - a) * (1 - b) * input[3 * newI + 1 + newJ * xSize * 3] +
                    (1 - a) * b * input[3 * (newI + 1) + 1 + newJ * xSize * 3] +
                    a * (1 - b) * input[3 * newI + 1 + (newJ + 1) * xSize * 3] +
                    a * b * input[3 * (newI + 1) + 1 + (newJ + 1) * xSize * 3];
            output[3 * i + 2 + j * newXSize * 3] =
                    (1 - a) * (1 - b) * input[3 * newI + 2 + newJ * xSize * 3] +
                    (1 - a) * b * input[3 * (newI + 1) + 2 + newJ * xSize * 3] +
                    a * (1 - b) * input[3 * newI + 2 + (newJ + 1) * xSize * 3] +
                    a * b * input[3 * (newI + 1) + 2 + (newJ + 1) * xSize * 3];
        }
    }
}

double cubicInterpolate(uchar points[5], double d)
{
    auto w = [](double d)
    {
        if (fabs(d) < 1)
        {
            return 1.5 * pow(fabs(d), 3) - 2.5 * pow(fabs(d), 2) + 1;
        }
        else if (fabs(d) < 2)
        {
            return -0.5 * pow(fabs(d), 3) + 2.5 * pow(fabs(d), 2) - 4 * fabs(d) + 2;
        }
        else
        {
            return 0.;
        }
    };

    int res = 0;
    for (int m = 0; m < 5; m++)
    {
        double newD = d - m;
        res += points[m] * w(newD);
    }

    if (res > 255)
    {
        return 255;
    }
    else if (res < 0)
    {
        return 0;
    }
    else
    {
        return res;
    }
}

void bicubicInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
    double scaleX = ((double) xSize) / newXSize;
    double scaleY = ((double) ySize) / newYSize;

    for (int i = 0; i < newXSize; i++)
    {
        for (int j = 0; j < newYSize; j++)
        {

            int newI = i * scaleX;
            int newJ = j * scaleY;

            uchar pointsR[5] = { 0 };
            uchar pointsG[5] = { 0 };
            uchar pointsB[5] = { 0 };
            double d = 0;

            for (int m = -2; m < 3; m++)
            {
                if (d == 0)
                {
                    d = abs(j * scaleY - (newJ + m));
                }
                uchar pointsR2[5] = { 0 };
                uchar pointsG2[5] = { 0 };
                uchar pointsB2[5] = { 0 };
                double d2 = 0;
                for (int n = -2; n < 3; n++)
                {
                    if (d2 == 0)
                    {
                        d2 = abs(i * scaleX - (newI + n));
                    }
                    if (newI + n >= 0 && newI + n < xSize && newJ + m >= 0 && newJ + m < ySize)
                    {
                        pointsR2[n + 2] = input[3 * (newI + n) + (newJ + m) * xSize * 3];
                        pointsG2[n + 2] = input[3 * (newI + n) + 1 + (newJ + m) * xSize * 3];
                        pointsB2[n + 2] = input[3 * (newI + n) + 2 + (newJ + m) * xSize * 3];
                    }
                    else
                    {
                        pointsR2[n + 2] = 0;
                        pointsG2[n + 2] = 0;
                        pointsB2[n + 2] = 0;
                    }
                }
                pointsR[m + 2] = cubicInterpolate(pointsR2, d2);
                pointsG[m + 2] = cubicInterpolate(pointsG2, d2);
                pointsB[m + 2] = cubicInterpolate(pointsB2, d2);
            }
            output[3 * i + j * newXSize * 3] = cubicInterpolate(pointsR, d);
            output[3 * i + 1 + j * newXSize * 3] = cubicInterpolate(pointsG, d);
            output[3 * i + 2 + j * newXSize * 3] = cubicInterpolate(pointsB, d);
        }
    }
}

void imageSwirl(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double k1)
{
    for (int i = 0; i < xSize; i++)
    {
        for (int j = 0; j < ySize; j++)
        {
            double di = i - m;
            double dj = j - n;
            double r = sqrt(pow(di, 2) + pow(dj, 2));
            double theta = k1 * M_PI * r;
            int newI = cos(theta) * di - sin(theta) * dj + m;
            int newJ = sin(theta) * di + cos(theta) * dj + n;

            if (newI >= 0 && newI < xSize && newJ >= 0 && newJ < ySize)
            {
                output[3 * i + j * xSize * 3] = input[3 * newI + newJ * xSize * 3];
                output[3 * i + 1 + j * xSize * 3] = input[3 * newI + 1 + newJ * xSize * 3];
                output[3 * i + 2 + j * xSize * 3] = input[3 * newI + 2 + newJ * xSize * 3];
            }
            else
            {
                output[3 * i + j * xSize * 3] = 0;
                output[3 * i + 1 + j * xSize * 3] = 0;
                output[3 * i + 2 + j * xSize * 3] = 0;
            }
        }
    }
}

void imageSwirlBilinear(const uchar input[], int xSize, int ySize, uchar output[], int m, int n, double k1)
{
    for (int i = 0; i < xSize; i++)
    {
        for (int j = 0; j < ySize; j++)
        {
            double di = i - m;
            double dj = j - n;
            double r = sqrt(pow(di, 2) + pow(dj, 2));
            double theta = k1 * M_PI * r;

            double disI = cos(theta) * di - sin(theta) * dj + m;
            double disJ = sin(theta) * di + cos(theta) * dj + n;

            int newI = floor(disI);
            int newJ = floor(disJ);

            double a = disJ - newJ;
            double b = disI - newI;

            if (newI >= 0 && newI + 1 < xSize && newJ >= 0 && newJ + 1 < ySize)
            {
                output[3 * i + j * xSize * 3] =
                        (1 - a) * (1 - b) * input[3 * newI + newJ * xSize * 3] +
                        (1 - a) * b * input[3 * (newI + 1) + newJ * xSize * 3] +
                        a * (1 - b) * input[3 * newI + (newJ + 1) * xSize * 3] +
                        a * b * input[3 * (newI + 1) + (newJ + 1) * xSize * 3];
                output[3 * i + 1 + j * xSize * 3] =
                        (1 - a) * (1 - b) * input[3 * newI + 1 + newJ * xSize * 3] +
                        (1 - a) * b * input[3 * (newI + 1) + 1 + newJ * xSize * 3] +
                        a * (1 - b) * input[3 * newI + 1 + (newJ + 1) * xSize * 3] +
                        a * b * input[3 * (newI + 1) + 1 + (newJ + 1) * xSize * 3];
                output[3 * i + 2 + j * xSize * 3] =
                        (1 - a) * (1 - b) * input[3 * newI + 2 + newJ * xSize * 3] +
                        (1 - a) * b * input[3 * (newI + 1) + 2 + newJ * xSize * 3] +
                        a * (1 - b) * input[3 * newI + 2 + (newJ + 1) * xSize * 3] +
                        a * b * input[3 * (newI + 1) + 2 + (newJ + 1) * xSize * 3];
            }
            else
            {
                output[3 * i + j * xSize * 3] = 0;
                output[3 * i + 1 + j * xSize * 3] = 0;
                output[3 * i + 2 + j * xSize * 3] = 0;
            }
        }
    }
}
