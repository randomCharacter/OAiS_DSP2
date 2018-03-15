/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 aic3204.c                                                               */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Setup functions for aic3204 codec on the EZDSP 5535 USB Stick.          */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Dejan Bokan                                                   */
/*****************************************************************************/
/*
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
*/

#include "ezdsp5535.h"
#include "ezdsp5535_gpio.h"
#include "ezdsp5535_i2c.h"
#include "ezdsp5535_i2s.h"
#include "csl_i2s.h"

#define AIC3204_I2C_ADDR 0x18

Int16 counter1; // Counters for monitoring real-time operation.
Int16 counter2;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  AIC3204_rget( regnum, *regval )                                         *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 Int16 AIC3204_rget(  Uint16 regnum, Uint16* regval )
{
    Int16  retcode = 0;
    Uint16 cmd[2];

    cmd[0] = regnum & 0x007F;       // 7-bit Device Register
    cmd[1] = 0;

    retcode |= EZDSP5535_I2C_write( AIC3204_I2C_ADDR, cmd, 1 );
    retcode |= EZDSP5535_I2C_read( AIC3204_I2C_ADDR, cmd, 1 );

    *regval = cmd[0];
    EZDSP5535_wait( 10 );
    return retcode;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  AIC3204_rset( regnum, regval )                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 AIC3204_rset( Uint16 regnum, Uint16 regval )
{
    Uint16 cmd[2];
    cmd[0] = regnum & 0x007F;       // 7-bit Device Register
    cmd[1] = regval;                // 8-bit Register Data

    EZDSP5535_waitusec( 300 );

    return EZDSP5535_I2C_write( AIC3204_I2C_ADDR, cmd, 2 );
}


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic3204_enable( )                                                       *
 *                                                                          *
 * ------------------------------------------------------------------------ */  

void aic3204_hardware_init(void)
{
	EZDSP5535_I2C_init( );
    EZDSP5535_wait( 100 );  // Wait    
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic3204_disable( )                                                      *
 *                                                                          *
 * ------------------------------------------------------------------------ */

void aic3204_disable(void)
{
    EZDSP5535_I2S_close();    // Disble I2S
    AIC3204_rset( 1, 0x01 );  // Reset codec
}



/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic3204_codec_read( )                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */

void aic3204_codec_read(Int16* left_input, Int16* right_input)
{ 	   
	 /* Read 16-bit left channel Data */
     EZDSP5535_I2S_readLeft(left_input);
                
     /* Read 16-bit right channel Data */
     EZDSP5535_I2S_readRight(right_input);
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic3204_codec_write( )                                                  *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 
void aic3204_codec_write(Int16 left_output, Int16 right_output)
{
	 /* Write 16-bit left channel Data */
     EZDSP5535_I2S_writeLeft(left_output);
                
     /* Write 16-bit right channel Data */
     EZDSP5535_I2S_writeRight(right_output);
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of aic3204.c                                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
