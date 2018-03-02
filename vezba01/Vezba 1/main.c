//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *
// * Description:  Vezba 1 - Interpolacija i decimacija
// *
// * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
// * Copyright (C) 2011 Spectrum Digital, Incorporated
// *
//////////////////////////////////////////////////////////////////////////////

#include "tistdtypes.h"
#include <cstdio>
#include "ezdsp5535.h"
#include "math.h"
#include "signal.h"
#include "filter_coeff.h"
#include "processing.h"

/*
 *
 *  main( )
 *
 */

#define L 3
#define M 4

Int16 buff[SIGNAL2_LENGTH * L / M];

void main( void )
{

	printf("\n Vezba 1 - Interpolacija i decimacija \n");
	
	resample(signal2, buff, L, M, SIGNAL2_LENGTH);

	printf( "\n***Program has Terminated***\n" );
	SW_BREAKPOINT;
}

