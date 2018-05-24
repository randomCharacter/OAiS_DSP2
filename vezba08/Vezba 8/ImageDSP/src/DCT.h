#ifndef DCT_H_
#define DCT_H_
#include <QDebug>

/* perform DCT */
void performDCT(uchar Y_buff[], int xSize, int ySize, int N);

/* perform DCT and IDCT */
void performDCTandIDCT(uchar* Y_buff, int xSize, int ySize, int N);

/* perform masking of DCT coeffs */
void performMaskDCTCoeffs(uchar input[], int xSize, int ySize, int N, int B);

/* perform masking of Y image component */
void performMaskYCoeffs(uchar input[], int xSize, int ySize, int N, int B);

/* perform quantization of DCT coeffs using Q matrix*/
void performDCTQuantization(uchar input[], int xSize, int ySize);

#endif // DCT_H_
